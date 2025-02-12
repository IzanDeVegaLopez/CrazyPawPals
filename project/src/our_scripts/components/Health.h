#pragma once
#include "../ecs/Component.h"

class Texture;
class Health:public ecs::Component
{
public: 
	__CMPID_DECL__(ecs::cmp::HEALTH);
	Health();
	virtual ~Health();
	void render() override;
	void release();
	int Lives()const;
	void reset();
	
protected:
	int _lives;
	Texture* _heartTexture;
	const int DEFAULT_LIVES = 3;
};
inline int 
Health::Lives()const {
	return _lives;
}
inline void 
Health::reset() {
	_lives = DEFAULT_LIVES;
}

