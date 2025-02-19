#pragma once
#include "../game/GameObject.h"
#include <string>
class Bullet: public GameObject
{
public:
	Bullet(ecs::Manager* mngr,Vector2D& pos, Vector2D& dir, Vector2D& prevDir, float speed, float lifeTime, const std::string& texName, int width, int height);
	virtual ~Bullet() {};
private:
	Transform* _tr;
};


