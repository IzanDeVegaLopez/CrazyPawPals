#include "ShootComponent.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Manager.h"

#include "Weapon.h"
#include "../../game/Game.h"

#include "Transform.h"

using namespace ecs;

ShootComponent::ShootComponent(): _shootCooldown(0.5f), _lastShoot(0.0f) {}

//ShootComponent::ShootComponent(std::vector<Bullet*>* b):_bulletPool(b), _shootCooldown(0.5f), _lastShoot(0.0f), _maxSpeed(10.0f) {}

ShootComponent::~ShootComponent() {}

void 
ShootComponent::initComponent() {
	_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}
void
ShootComponent::shoot(const Vector2D& target) {

	auto& pos = _tr->getPos();
	auto* weapon =Game::Instance()->get_mngr()->getComponent<Weapon>(_ent);

	if (sdlutils().virtualTimer().currRealTime() >= _lastShoot + weapon->cooldown()) {
		Vector2D shootPos = { pos.getX() + (_tr->getWidth() / 2), pos.getY() + (_tr->getHeight() / 2) };
		Vector2D shootDir = (target - shootPos).normalize(); 

		weapon->callback(shootPos, shootDir);
		_lastShoot = sdlutils().virtualTimer().currRealTime();
	}
}