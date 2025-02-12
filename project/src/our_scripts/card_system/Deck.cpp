#include "Deck.hpp"
#include <iostream>

void Deck::_put_new_card_on_hand()
{
	if (_hand != nullptr) {
		_discard_pile.add_card(std::move(_hand));
	}
	if (!_draw_pile.empty()) {
		_hand = _draw_pile.pop_first();
	}
	else {
		_hand = nullptr;
		reload();
	}
}
//For testing Purposes
Deck::Deck() noexcept
{
	_draw_pile = CardList();
	_hand = nullptr;
	_mana = new Mana(); // REMOVE AFTER IMPLEMENTING PLAYER
	_discard_pile = CardList();
	_draw_pile.shuffle();
	_put_new_card_on_hand();
}

Deck::Deck(std::list<Card*>& starterDeck) noexcept
{
	_discard_pile = CardList();
	_hand = nullptr;
	_draw_pile = CardList(starterDeck);
	_put_new_card_on_hand();
}

Deck::Deck(CardList&& starterDeck) noexcept
{
	_discard_pile = CardList();
	_hand = nullptr;
	_draw_pile = starterDeck;
	_put_new_card_on_hand();
}

Deck::~Deck()
{
	//Hand es un puntero a una carta
	if(_hand!=nullptr)
		delete _hand;
	//_draw_pile y _discard_pile llamarán a su destructor cuando esto se destruya al salir de ámbito
}

bool Deck::use_card() noexcept
{
	if (_can_play_hand_card()) {
		//Se pudo usar la carta
		_mana->change_mana(_hand->mana_cost());
		_hand->on_play();
		_put_new_card_on_hand();
		return true;
	}
	else {
		//No se pudo usar la carta, dar indicación visual y auditiva al player
		return false;
	}
}

bool Deck::discard_card() noexcept
{
	if (_hand != nullptr) {
		_put_new_card_on_hand();
		return true;
	}
	else {
		return false;
	}
}

void Deck::mill() noexcept
{
	if (!_draw_pile.empty()) {
		_discard_pile.add_card(_draw_pile.pop_first()->on_mill());
	}
}

void Deck::reload() noexcept
{
	if (!_is_reloading) {
		//TODO
		//TODO -> block player action
		
		_is_reloading = true;
		_time_till_reload_finishes = reload_time;

		//Puts all cards on discard
		if (_hand != nullptr) {
			_discard_pile.add_card(std::move(_hand));
			_hand = nullptr;
		}
		_draw_pile.move_from_this_to(_discard_pile);
	}

}
void Deck::_finish_realoading()
{
	//this is called after waiting
	//this moves everything to draw_pile, shuffles and return control to player
	_is_reloading = false;
	_discard_pile.move_from_this_to(_draw_pile);
	_draw_pile.shuffle();

	std::cout << *this;
}
bool Deck::_can_finish_reloading()
{
	return _is_reloading && _time_till_reload_finishes <= 0;
}

bool Deck::_can_play_hand_card()
{
	return (!_is_reloading && _mana->mana_count() >= _hand->mana_cost());
	//TODO: card checks mana and life costs
	
}

void Deck::update(float deltaTime) noexcept
{
	//TODO
	//Counts time down for reload time and do the rest of things needed for finishing reload
	_time_till_reload_finishes -= deltaTime;
	//std::cout << _time_till_reload_finishes << std::endl;
	if (_can_finish_reloading()) {
		_finish_realoading();
	}
}

void Deck::render() noexcept
{
	//TODO
	//Mostrar carta en la mano
	//nº cartas draw_pile and discard_pile
}

void Deck::add_card_to_deck(Card* c)
{
	assert(c != nullptr);
	_draw_pile.add_card(std::move(c));
}

void Deck::remove_card(std::list<Card*>::iterator)
{
}

std::ostream& operator<<(std::ostream& os, const Deck& deck)
{
	os << "Draw Pile: " << std::endl;
	os << deck._draw_pile;
	os << std::endl;

	if(deck._hand!=nullptr)
		os << "Hand: "  << std::endl << *deck._hand << std::endl;

	os << "DiscardPile: " << std::endl;
	os << deck._discard_pile;
	os<<std::endl;
	
	return os;
}
