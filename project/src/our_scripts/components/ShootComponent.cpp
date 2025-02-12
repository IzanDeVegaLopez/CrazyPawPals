#include "ShootComponent.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Manager.h"

#include "Transform.h"
using namespace ecs;

ShootComponent::ShootComponent(): _shootCooldown(0.5f), _lastShoot(0.0f), _maxSpeed(10.0f) {}

//ShootComponent::ShootComponent(std::vector<Entity*>* b): _bulletPool(b) {}

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

		std::cout << "shot" << std::endl;
		//_bulletPool->push_back(bullet);
		_lastShoot = sdlutils().currRealTime(); 
	}
}