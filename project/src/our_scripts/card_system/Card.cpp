#include "Card.hpp"

Card::Card(char* card_name)
	:name(card_name)
{
}

bool Card::useCard()
{
	//Do Things
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