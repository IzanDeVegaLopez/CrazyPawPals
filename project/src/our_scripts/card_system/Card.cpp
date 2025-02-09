#include "Card.hpp"

Card::Card(char* card_name)
	:name(card_name)
{
}

Card::~Card()
{
}

bool Card::useCard()
{
	//Do Things
	//Returns if it could be used (because of mana and that sort of things)
	return true;
}

Card* Card::mill()
{
	//TODO: mill effect
	return this;
}

char* Card::getName()
{
	return name;
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
	os << card.name;
	return os;
}