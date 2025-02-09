#pragma once
#include <string>
#include <ostream>
class Card {
private:
	char* name;
public:
	Card(char*);
	~Card();
	bool useCard();
	Card* mill();
	friend std::ostream& operator << (std::ostream& os, const Card& card);
	//Debug purposes
	char* getName();
};
