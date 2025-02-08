#pragma once
#include "CardList.h"
#include "Card.hpp"


class Deck {
protected:
	CardList _draw_pile;
	CardList _discard_pile;
	Card* _hand;
	void _putNewCardOnHand();
public:
	Deck();
	Deck(CardList&& starterDeck);
	//returns true if the card can be used, calls the useCard function of the card
	//and puts the top card of deck on hand, if there's non left it reloads
	bool useCard();
	//puts the card on hand on discard pile
	//and puts the top card of deck on hand, if there's non left it reloads
	//returns true, if a card could be discarded
	bool discardCard();
	//Puts the top card of draw pile on discard and fires its mill effect
	//If there's no cards left on deck this does nothing
	void mill();
	//Puts all cards on discard pile and sets player unable to use any action outside moving
	//Then puts all cards on drawPile and shuffles
	void reload();
	void render();
	friend std::ostream& operator << (std::ostream& os, const Deck& deck);
	/*
	class const_iterator {
	private:
		friend class Deck;
	};
	*/
};