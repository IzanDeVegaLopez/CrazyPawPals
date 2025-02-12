#pragma once
#include "../../ecs/Component.h"
#include "../../utils/Vector2D.h" 

class Transform;
class ShootComponent : public ecs::Component {
protected:
	float _shootCooldown;
	float _lastShoot;
	float _maxSpeed;

	Transform* _tr;
	//std::vector<ecs::Entity*>* _bulletPool;
public:
	__CMPID_DECL__(ecs::cmp::SHOOTCOMPONENT);
	ShootComponent();
	virtual ~ShootComponent();
	void initComponent() override;
	void shoot(const Vector2D& target); 
	
};