#include "Card.hpp"
#include "../../game/GameStructs.h"

class Fireball : public Card {
public:
	Fireball();
	void on_play(const Vector2D* player_position, const Vector2D* target_position) override;
};
class Minigun : public Card {
public:
	Minigun();
	void on_play( const Vector2D* player_position, const Vector2D* target_position) override;
	void update(uint32_t) override;
private:
	GameStructs::BulletProperties _bullets_properties;
	const Vector2D* _aim_vec;
	const Vector2D* _pl_vec;
	bool _playing;
	int _time_since_played;
	int _number_of_bullets_shot = 0;
	int _number_of_shots = 3;
	int _shooting_duration = 500;
};