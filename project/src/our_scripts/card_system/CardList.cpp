#include "CardList.h"

CardList::CardList()
{
	_my_card_list = std::list<Card*>();
}

CardList::~CardList()
{

}

CardList::CardList(std::list<Card*>& l)
{
	_my_card_list = std::list<Card*>(l);
}

void CardList::moveFromThisTo(CardList& my_new_list)
{
	my_new_list._my_card_list.splice(my_new_list._my_card_list.end(), _my_card_list, _my_card_list.begin(), _my_card_list.end());
}

void CardList::addCard(Card*&& c)
{
	_my_card_list.emplace_back(c);
}

//
void CardList::eraseCard(std::list<Card*>::iterator it)
{
	delete* it;
	_my_card_list.erase(it);
}

Card* CardList::popFirst()
{
	Card* my_card = std::move(*_my_card_list.begin());
	_my_card_list.pop_front();
	return my_card;
}

int CardList::numberOfCards()
{
	return _my_card_list.size();
}

bool CardList::empty()
{
	return _my_card_list.size()==0;
}

void CardList::shuffle()
{
	std::vector<std::reference_wrapper<Card*>> vec(_my_card_list.begin(),_my_card_list.end());
	std::shuffle(vec.begin(), vec.end(), std::mt19937{ std::random_device{}()});
	_my_card_list = {vec.begin(),vec.end()};
}

std::ostream& operator<<(std::ostream& os, const CardList& card_list)
{
	for (Card* c : card_list._my_card_list) {
		os << c->getName() << " -> ";
	}
	return os;
}
