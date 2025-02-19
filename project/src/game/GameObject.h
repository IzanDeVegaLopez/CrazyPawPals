// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL.h>

#include "../utils/Vector2D.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../our_scripts/components/Transform.h"
#include "../sdlutils/SDLUtils.h"

class GameObject {
protected:
	ecs::entity_t _entity;

public:
	GameObject(ecs::Manager* mngr, ecs::grpId_t gId = ecs::grp::DEFAULT) {
		_entity = mngr->addEntity(gId);
		auto tr = mngr->addComponent<Transform>(_entity);
		tr->init(Vector2D(0, 0), Vector2D(), Vector2D(0, 0), 1.0f, 1.0f, 0.0f, 0.0f);
	}
	virtual ~GameObject() {
	}

};

