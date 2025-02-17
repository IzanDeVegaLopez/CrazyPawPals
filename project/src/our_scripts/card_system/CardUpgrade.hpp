#include "Card.hpp"


//Decorator Patron --> https://refactoring.guru/design-patterns/decorator/cpp/example
class BaseCardUpgrade : public Card {
protected:
	//reference to the card it upgrades
	std::unique_ptr<Card> _card;
public:
	BaseCardUpgrade(std::unique_ptr<Card>&& my_card, Resources& res_mod = Resources(0,0))
		:_card(std::move(my_card)) {
		Resources& res = get_costs();
		res = res + res_mod;
		//my_card = this;
	}

	void on_play() override{
		_card->on_play();
		//call CardUpgrade::on_play and then 
		//add to the function whatever you need
	}
	Card* on_mill() override {
		_card->on_mill();
		return this;
		//add to the function whatever you need and then call CardUpgrade::on_mill
	}
	virtual Resources& get_costs() {
		return _card->get_costs();
	}
	virtual std::string get_written_info() override {
		return _card->get_written_info();
	}
};


//Upgrades-----------------------------------------------------------
class PlayItTwice : public BaseCardUpgrade {
public:
	PlayItTwice(std::unique_ptr<Card>&& c) :BaseCardUpgrade(c, Resources(1,0)) {}
	void on_play() override {
		BaseCardUpgrade::on_play();
		_card->on_play();
	}
};
class CheaperBy1 : public BaseCardUpgrade {
public:
	CheaperBy1(std::unique_ptr<Card>&& c) :BaseCardUpgrade(c, Resources(-1, 0)) {}
};