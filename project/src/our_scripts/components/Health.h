#pragma once
#include "../../ecs/Component.h"

const int DEFAULT_LIVES = 100;

class Texture;
class Health :public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::HEALTH);
	Health();
	virtual ~Health();
	void onHit();
	void render()override;
	void update() override; //solo para depuracion
	int Lives()const;
	void reset();

protected:
	int _lives;
	Texture* _tex;
};
inline int
Health::Lives()const {
	return _lives;
}
inline void
Health::reset() {
	_lives = DEFAULT_LIVES;
}
