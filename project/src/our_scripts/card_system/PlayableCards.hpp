#include "Card.hpp"
class Fireball : public Card {
public:
	Fireball();
	void on_play(Vector2D& player_position, Vector2D& target_position) override;
};
class Minigun : public Card {
public:
	Minigun();
	//void on_play() override;
};