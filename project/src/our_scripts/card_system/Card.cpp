#include "Card.hpp"
class Resources {
private:
	int _mana = 0;
	int _health = 0;
public:
	Resources() = default;
	void set_mana(int mana) { _mana = mana; }
	int mana() const { return _mana; }
	void set_health(int health) { _health = health; }
	int health() const { return _health; }
};

Card::Card(char* card_name)
	:_name(card_name)
{
}

Card::~Card() { delete _name; }


Card* Card::mill()
{
	// Mill effect
	return this;
}

void Card::on_play() {
	// Deploy effect
	std::cout << _name << " deployed" << std::endl;
}

char* Card::getName()
{
	return _name;
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
	os << card._name;
	return os;
}

/*bool Card::can_play()
{
	// Returns if it could be used
	int mana_bank = _resources->mana();
	if (_mana_cost >= mana_bank) {
		_resources->set_mana(mana_bank-_mana_cost);
		return true;
	}
	return false;
}*/