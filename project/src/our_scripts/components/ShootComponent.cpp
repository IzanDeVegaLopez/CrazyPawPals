#include "ShootComponent.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Manager.h"
#include "../../our_scripts/Bullet.h"
#include "../../game/Game.h"
#include "Transform.h"

using namespace ecs;

ShootComponent::ShootComponent(): _shootCooldown(0.5f), _lastShoot(0.0f), _maxSpeed(10.0f) {}

//ShootComponent::ShootComponent(std::vector<Bullet*>* b):_bulletPool(b), _shootCooldown(0.5f), _lastShoot(0.0f), _maxSpeed(10.0f) {}

ShootComponent::~ShootComponent() {}

void 
ShootComponent::initComponent() {
	_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}
void
ShootComponent::shoot(const Vector2D& target) {

	Vector2D pos = _tr->getPos() + Vector2D(_tr->getWidth()/2,_tr->getHeight()/2);
	if (sdlutils().currRealTime() >= _lastShoot + _shootCooldown) { 
		Vector2D shootDir = (target - pos).normalize(); 
		Vector2D shootPos = { pos.getX() + (_tr->getWidth() / 2), pos.getY() + (_tr->getHeight() / 2) };
		//_bulletPool.push_back(new Bullet(shootPos , shootDir, 5.0f));
		BulletProperties bp = BulletProperties();
		bp.init_pos = pos;
		bp.dir = target;
		bp.speed = 5.0f;
		Bullet::generate_proyectile(bp);
		_lastShoot = sdlutils().currRealTime(); 
	}
}