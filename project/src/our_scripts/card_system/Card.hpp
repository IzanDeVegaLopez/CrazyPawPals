#pragma once
#include <string>
#include <ostream>
#include <iostream>
class Resources; // temporary class until resource management system is decided
class Card {
protected:
	char* _name;
	int _mana_cost = 0; // Deducted from the player's mana pool upon successfully playing the card. Cards cannot be played if the player's mana is insufficient.
	int _health_cost = 0; // Deducted from the player's health pool upon successfully playing the card. If a card requires a health cost, it cannot lethally damage the player.

	Resources* _resources;

	// OPTIONAL ATTRIBUTES
	// These can be declared, modified and utilized within specific subclasses if necessary.
	// Optional attributes are custom-made and can be implemented in a multitude of ways.
	// Don't hesitate to make your own on the spot if needed.
	// 
	// int _boost = 0; // Unique counter for specific cards that are enhanced under certain conditions.
	// int _boost_threshold = 0; //Represents the value at which _boost enhances the card. 
public:
	Card(char*);
	virtual ~Card(); 

	int mana_cost() const { return _mana_cost; }
	int health_cost() const { return _health_cost; }

	//bool can_play();
	virtual void on_play();
	virtual Card* on_mill();
	friend std::ostream& operator << (std::ostream& os, const Card& card);
	//Debug purposes
	char* getName();
};
