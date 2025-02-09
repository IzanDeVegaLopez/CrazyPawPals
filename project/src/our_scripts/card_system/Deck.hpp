#pragma once
#include "CardList.h"
#include "Card.hpp"
#include <list>
#include <cassert>

class Deck {
protected:
	CardList _draw_pile;
	CardList _discard_pile;
	Card* _hand;
	void _putNewCardOnHand();
public:
	Deck() noexcept;
	//Creates a starter with a list of cards
	Deck(CardList&& starterDeck) noexcept;
	Deck(std::list<Card*>&) noexcept;
	~Deck();
	//returns true if the card can be used, calls the useCard function of the card
	//and puts the top card of deck on hand, if there's non left it reloads
	bool useCard() noexcept;
	//puts the card on hand on discard pile
	//and puts the top card of deck on hand, if there's non left it reloads
	//returns true, if a card could be discarded
	bool discardCard() noexcept;
	//Puts the top card of draw pile on discard and fires its mill effect
	//If there's no cards left on deck this does nothing
	void mill() noexcept;
	//Puts all cards on discard pile and sets player unable to use any action outside moving
	//Then puts all cards on drawPile and shuffles
	void reload() noexcept;
	void render() noexcept;
	friend std::ostream& operator << (std::ostream& os, const Deck& deck);

	void addCardToDeck(Card*);
	//removeCard, must only be used during menu time
	//Whenever this happens the iterator passed to this function must
	//belong to _draw_pile (during rewards menu all cards are exclusively in
	//the draw pile)
	void removeCard(std::list<Card*>::iterator);
	/*
	class const_iterator {
	private:
		friend class Deck;
	};
	*/
};