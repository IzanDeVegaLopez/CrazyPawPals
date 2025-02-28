#include "PlayableCards.hpp"
#include "../../game/Game.h"
#include "../../game/GameScene.h"
#include "ShootPatrons.hpp"


Fireball::Fireball():Card("fireball", Resources(1))
{
}
void Fireball::on_play(Deck& d, const Vector2D* player_position,const Vector2D* target_position) {
	Card::on_play(d,player_position,target_position);
	GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	bp.dir = ((*target_position) - (*player_position)).normalize();
	bp.init_pos = *player_position;
	bp.speed = 0.1f;
	bp.height = 1.3;
	bp.width = 1.3;
	bp.life_time = 1;
	bp.sprite_key = "fireball";
	static_cast<GameScene*>(Game::Instance()->get_currentScene())->generate_proyectile(bp, ecs::grp::BULLET);
}

Minigun::Minigun()
	: Card("minigun", Resources(2)), _pl_vec(), _playing(false), _time_since_played(0)
{
	_bullets_properties = GameStructs::BulletProperties();
	_bullets_properties.speed = 0.5;
	_bullets_properties.height = 0.5;
	_bullets_properties.width = 0.5;
	_bullets_properties.life_time = 0.2f;
	_bullets_properties.sprite_key = "minigun";
}
void Minigun::on_play(Deck& d, const Vector2D* player_position, const Vector2D* target_position)
{
	Card::on_play(d,player_position,target_position);
	_playing = true;
	_time_since_played = 0;
	_aim_vec = target_position;
	_pl_vec = player_position;
	_number_of_bullets_shot = 0;
}

void Minigun::update(uint32_t dt)
{
	if (_playing) {
		_time_since_played += dt;
		if (_time_since_played >= _number_of_bullets_shot * (_shooting_duration / (_number_of_shots-1))) {
			_bullets_properties.dir = ((*_aim_vec) - (*_pl_vec)).normalize();
			_bullets_properties.init_pos = *_pl_vec;
			//std::cout <<_bullets_properties.init_pos << "--" << _bullets_properties.dir << std::endl;
			static_cast<GameScene*>(Game::Instance()->get_currentScene())->generate_proyectile(_bullets_properties, ecs::grp::BULLET);
			++_number_of_bullets_shot;
			if (_number_of_bullets_shot == _number_of_shots)
				_playing = false;
		}
	}
}

Lighting::Lighting()
	:Card("lighting", Resources(2))
{
}

void Lighting::on_play(Deck& d, const Vector2D* player_position, const Vector2D* target_position)
{
	Card::on_play(d, player_position, target_position);
	GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	bp.dir = ((*target_position) - (*player_position)).normalize();
	bp.init_pos = *target_position;
	bp.speed = 0;
	bp.height = 1.4;
	bp.width = 1.8;
	bp.life_time = 0.1;
	bp.sprite_key = "lighting";
	static_cast<GameScene*>(Game::Instance()->get_currentScene())->generate_proyectile(bp, ecs::grp::BULLET);
}

CardSpray::CardSpray()
	:Card("card_spray", Resources(0))
{
}

void CardSpray::on_play(Deck& d, const Vector2D* player_position, const Vector2D* target_position)
{
	Card::on_play(d, player_position, target_position);
	GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	bp.dir = ((*target_position) - (*player_position)).normalize();
	bp.init_pos = *player_position;
	bp.speed = 0.05;
	bp.height = 0.7;
	bp.width = 0.7;
	bp.life_time = 3;
	bp.sprite_key = "card_spray";

	patrons::ShotgunPatron(bp, ecs::grp::BULLET, 75, 3);
	d.mill();
}
