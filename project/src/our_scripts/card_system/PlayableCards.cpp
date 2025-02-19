#include "PlayableCards.hpp"
#include "../../game/Game.h"
#include "../../game/GameScene.h"
Fireball::Fireball():Card("Fireball", Resources(1))
{
}
void Fireball::on_play(Vector2D& player_position, Vector2D& target_position) {
	Card::on_play(player_position,target_position);
	GameStructs::BulletProperties bp = GameStructs::BulletProperties();
	bp.dir = target_position;
	bp.init_pos = player_position;
	bp.speed = 1;
	bp.height = 30;
	bp.width = 80;
	bp.life_time = 2;
	Game::Instance()->get_gameScene()->generate_proyectile(bp, ecs::grp::BULLET);
}

Minigun::Minigun():Card("Minigun", Resources(2))
{
}
void Minigun::on_play(Vector2D& player_position, Vector2D& target_position)
{
	Card::on_play(player_position,target_position);
	//do shit
}




