#include "Card.hpp"


Card::Card(std::string card_name, Resources r)
	:_name(card_name), _my_costs(r)
{
}


Card::~Card() { }

Resources& Card::get_costs()
{
	return _my_costs;
}


Card* Card::on_mill()
{
	// Mill effect
	return this;
}

void Card::on_play(Vector2D& player_position, Vector2D& target_position) {
	// Deploy effect
	std::cout << _name << " Played" << std::endl;
}

std::string Card::get_written_info()
{
	return std::to_string(_my_costs.get_mana()) + '-' + _name;
}