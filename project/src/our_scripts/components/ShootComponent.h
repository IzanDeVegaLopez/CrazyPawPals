#pragma once
#include "../../ecs/Component.h"
#include "../../utils/Vector2D.h" 

#include <vector>


class Transform;
class Weapon;
class ShootComponent : public ecs::Component {
protected:
	float _shootCooldown;
	float _lastShoot;

	Transform* _tr;
	Weapon* _w;

public:
	__CMPID_DECL__(ecs::cmp::SHOOTCOMPONENT);

	ShootComponent();
	//ShootComponent(std::vector<Bullet*>* b);

	virtual ~ShootComponent();
	void initComponent() override;
	void shoot(const Vector2D& target); 
	
};