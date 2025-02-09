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
Deck::Deck() noexcept
{
	_draw_pile = CardList();
	_hand = nullptr;
	_discard_pile = CardList();
	_draw_pile.shuffle();
	_putNewCardOnHand();
}

Deck::Deck(std::list<Card*>& starterDeck) noexcept
{
	_discard_pile = CardList();
	_hand = nullptr;
	_draw_pile = CardList(starterDeck);
	_putNewCardOnHand();
}

Deck::Deck(CardList&& starterDeck) noexcept
{
	_discard_pile = CardList();
	_hand = nullptr;
	_draw_pile = starterDeck;
	_putNewCardOnHand();
}

Deck::~Deck()
{
	//Hand es un puntero a una carta
	if(_hand!=nullptr)
		delete _hand;
	//_draw_pile y _discard_pile llamarán a su destructor cuando esto se destruya al salir de ámbito
}

bool Deck::useCard() noexcept
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

bool Deck::discardCard() noexcept
{
	if (_hand != nullptr) {
		_putNewCardOnHand();
		return true;
	}
	else {
		return false;
	}
}

void Deck::mill() noexcept
{
	if (!_draw_pile.empty()) {
		_discard_pile.addCard(_draw_pile.popFirst()->mill());
	}
}

void Deck::reload() noexcept
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

void Deck::render() noexcept
{
	//TODO
	//Mostrar carta en la mano
	//nº cartas draw_pile and discard_pile
}

void Deck::addCardToDeck(Card* c)
{
	assert(c != nullptr);
	_draw_pile.addCard(std::move(c));
}

void Deck::removeCard(std::list<Card*>::iterator)
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
	// TODO: insert return statement here
}
