#pragma once
#include "../game/GameObject.h"

class Transform;
class Image;
class Enemy : public GameObject
{
public:
	Enemy(ecs::Manager* mngr);
	virtual ~Enemy() {};

	Image* getImage();
	ecs::entity_t getEntity();
	Transform* getTransform();
	void init(const Vector2D& pos, float width, float height, float speed, const std::string& imageName);

private:
	Transform* _tr;
	Image* _img;
};
