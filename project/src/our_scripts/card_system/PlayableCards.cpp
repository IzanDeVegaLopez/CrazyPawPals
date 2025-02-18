#include "PlayableCards.hpp"

Fireball::Fireball():Card("Fireball", Resources(1))
{
}
void Fireball::on_play(Vector2D& player_position, Vector2D& target_position) {
	Card::on_play(player_position,target_position);
	BulletProperties bp = BulletProperties();
	bp.dir = target_position;
	bp.init_pos = player_position;
	bp.speed = 1;
	bp.height = 30;
	bp.width = 80;
	bp.life_time = 2;
	Bullet::generate_proyectile(bp);
}

Minigun::Minigun():Card("Minigun", Resources(2))
{
}
void Minigun::on_play(Vector2D& player_position, Vector2D& target_position)
{
	Card::on_play(player_position,target_position);
	//do shit
}




