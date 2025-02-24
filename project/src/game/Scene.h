#pragma once
#include <vector>
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "GameStructs.h"
#include "../ecs/ecs.h"
#include "../game/Game.h"

class Scene {
public:
	Scene() {};
	virtual ~Scene() {};
	//Each scene
	virtual void initScene() = 0;
	virtual void enterScene() = 0;
	virtual void exitScene() = 0;

	virtual void update(uint32_t delta_time) = 0;
	virtual void render() = 0;

	template <typename ... Cmps>
	ecs::entity_t create_entity(ecs::grpId_t gid, ecs::sceneId_t sid,Cmps ... components) {
		ecs::entity_t ent = Game::Instance()->get_mngr()->addEntity(sid);
		Game::Instance()->get_mngr()->addExistingComponent(ent, components...);
		return ent;
	}
};