#include "Card.hpp"

class Fireball : public Card /*, public event_system::event_receiver*/ {
public:
	Fireball();
	void on_play(Deck& d,const Vector2D* player_position, const Vector2D* target_position) override;
	//void event_callback0(const Msg& m) override;
};

class Lighting : public Card {
public:
	Lighting();
	void on_play(Deck& d, const Vector2D* player_position, const Vector2D* target_position) override;
};

class Kunai : public Card {
public:
	Kunai();
	void on_play(Deck& d, const Vector2D* player_position, const Vector2D* target_position) override;
};


class Minigun : public Card {
public:
	Minigun();
	void on_play(Deck& d, const Vector2D* player_position, const Vector2D* target_position) override;
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

class CardSpray : public Card {
public:
	CardSpray();
	void on_play(Deck& d, const Vector2D* player_position, const Vector2D* target_position) override;
};

class EldritchBlast : public Card { // TODO: Reset on round end
public: 
	EldritchBlast();
	void on_play(Deck& d, const Vector2D* player_position, const Vector2D* target_position) override;
	Card* on_mill(Deck& d, const Vector2D* player_position) override;
private:
	int _shot_count = 1;
	const int _amplitude = 10;
};
