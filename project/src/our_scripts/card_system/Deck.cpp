#include "Deck.hpp"


void Deck::_putNewCardOnHand()
{
	if (_hand != nullptr) {
		_discard_pile.addCard(std::move(_hand));
	}
	if (!_draw_pile.empty()) {
		_hand = _draw_pile.popFirst();
	}
	else {
		_hand = nullptr;
		reload();
	}
}
//For testing Purposes
Deck::Deck()
{
	std::list<Card*> my_card_list{new Card("1"),new Card("2"),new Card("3"),new Card("4")};
	_draw_pile = CardList(my_card_list);
	_hand = nullptr;
	_discard_pile = CardList();
	_draw_pile.shuffle();
	_putNewCardOnHand();
}

Deck::Deck(CardList&& starterDeck)
{
	_discard_pile = CardList();
	_hand = nullptr;
	_draw_pile = starterDeck;
	_putNewCardOnHand();
}

bool Deck::useCard()
{
	if (_hand->useCard()) {
		//Se pudo usar la carta
		_putNewCardOnHand();
		return true;
	}
	else {
		//No se pudo usar la carta, dar indicación visual y auditiva al player
		return false;
	}
}

bool Deck::discardCard()
{
	if (_hand != nullptr) {
		_putNewCardOnHand();
		return true;
	}
	else {
		return false;
	}
}

void Deck::mill()
{
	if (!_draw_pile.empty()) {
		_discard_pile.addCard(_draw_pile.popFirst()->mill());
	}
}

void Deck::reload()
{
	//TODO
	//TODO -> block player action
	//Puts all cards on discard
	if (_hand != nullptr) {
		_discard_pile.addCard(std::move(_hand));
		_hand = nullptr;
	}
	_draw_pile.moveFromThisTo(_discard_pile);
	//TODO -> waits X time
	//Puts all cards on draw pile shuffled
	_discard_pile.moveFromThisTo(_draw_pile);
	_draw_pile.shuffle();
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
	// TODO: insert return statement here
}
