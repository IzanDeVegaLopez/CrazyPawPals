#pragma once
#include <list>
#include <vector>
#include <algorithm>
#include <random>
#include <ostream>
#include "Card.hpp"

class CardList {
protected:
	std::list<Card*> _my_card_list;
public:
	CardList();
	CardList(std::list<Card*>&);
	void moveFromThisTo(CardList&);
	void addCard(Card*&&);
	Card* popFirst();
	int numberOfCards();
	bool empty();
	void shuffle();

	friend std::ostream& operator << (std::ostream& os, const CardList& card_list);

};