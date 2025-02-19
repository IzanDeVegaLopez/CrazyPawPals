#include "PlayableCards.hpp"

Fireball::Fireball():Card("Fireball", Resources(1))
{
}
void Fireball::on_play(const Vector2D* player_position,const Vector2D* target_position) {
	Card::on_play(player_position,target_position);
	BulletProperties bp = BulletProperties();
	bp.dir = *target_position;
	bp.init_pos = *player_position;
	bp.speed = 1;
	bp.height = 30;
	bp.width = 80;
	bp.life_time = 2;
	Bullet::generate_proyectile(bp);
}

Minigun::Minigun()
	: Card("Minigun", Resources(2)), _playing(false), _time_since_played(0), _pl_vec()
{
	_bullets_properties = BulletProperties();
	_bullets_properties.speed = 5;
	_bullets_properties.height = 20;
	_bullets_properties.width = 20;
	_bullets_properties.life_time = 0.5;
}
void Minigun::on_play(const Vector2D* player_position, const Vector2D* target_position)
{
	Card::on_play(player_position,target_position);
	_playing = true;
	_time_since_played = 0;
	_aim_vec = target_position;
	_pl_vec = player_position;
	_number_of_bullets_shot = 0;
}

void Minigun::update(Uint32 dt)
{
	if (_playing) {
		_time_since_played += dt;
		if (_time_since_played >= _number_of_bullets_shot * (_shooting_duration / (_number_of_shots-1))) {
			_bullets_properties.dir = *_aim_vec;
			_bullets_properties.init_pos = *_pl_vec;
			//std::cout << *_aim_vec << std::endl;
			Bullet::generate_proyectile(_bullets_properties);
			++_number_of_bullets_shot;
			if (_number_of_bullets_shot == _number_of_shots)
				_playing = false;
		}
	}
}




