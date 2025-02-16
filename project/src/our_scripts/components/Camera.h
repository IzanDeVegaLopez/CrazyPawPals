#pragma once
#include "../../game/GameObject.h"

class Camera : public GameObject {

protected:
	Transform* _tr;

public:
	Camera(ecs::Manager* mngr);
	virtual ~Camera() {};
};