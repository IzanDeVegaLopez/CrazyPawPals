#include "ShootComponent.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Manager.h"
#include "../../our_scripts/Bullet.h"

#include "Transform.h"

using namespace ecs;

ShootComponent::ShootComponent(): _shootCooldown(0.5f), _lastShoot(0.0f), _maxSpeed(10.0f) {}

//ShootComponent::ShootComponent(std::vector<Bullet*>* b):_bulletPool(b), _shootCooldown(0.5f), _lastShoot(0.0f), _maxSpeed(10.0f) {}

ShootComponent::~ShootComponent() {}

void 
ShootComponent::initComponent() {
	auto* mngr = _ent->getMngr();
	_tr = mngr->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}
void
ShootComponent::shoot(const Vector2D& target) {

	auto& pos = _tr->getPos();
	if (sdlutils().currRealTime() >= _lastShoot + _shootCooldown) { 
		Vector2D shootDir = (target - pos).normalize(); 
		Vector2D shootPos = { pos.getX() + (_tr->getWidth() / 2), pos.getY() + (_tr->getHeight() / 2) };
		_bulletPool.push_back(new Bullet(_ent->getMngr(),shootPos , shootDir, 5.0f));
		_lastShoot = sdlutils().currRealTime(); 
	}
}