#pragma once
#include <vector>
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "GameStructs.h"
#include "../ecs/ecs.h"
#include "../game/Game.h"

class Transform;
class Scene {
public:
	Scene(ecs::sceneId_t id);
	virtual ~Scene() {};
	//Each scene
	virtual void initScene() = 0;
	virtual void enterScene() = 0;
	virtual void exitScene() = 0;

	virtual void update(uint32_t delta_time) = 0;
	virtual void render() = 0;

	template <typename ... Cmps>
	ecs::entity_t create_entity(ecs::grpId_t gid, ecs::sceneId_t sid,Cmps ... components) {
		ecs::entity_t ent = Game::Instance()->get_mngr()->addEntity(sid, gid);
		Game::Instance()->get_mngr()->addExistingComponent(ent, components...);
		return ent;
	}
	
	struct rendering {
		enum camera_creation_descriptor_options {
			camera_creation_descriptor_options_none = 0, // 0
			camera_creation_descriptor_options_follow = 1 << 0, // 1
			camera_creation_descriptor_options_set_handler = 1 << 1, // 2
			camera_creation_descriptor_options_clamp = 1 << 2, // 4
		};
		using camera_creation_descriptor_flags = uint8_t;
		
		#define CZPP_NULLABLE
		static ecs::entity_t create_camera(
			const ecs::sceneId_t scene,
			const camera_creation_descriptor_flags flags,
			CZPP_NULLABLE const Transform *optional_follow_target
		);
	};
protected:
	ecs::sceneId_t _scene_ID;

	ecs::entity_t create_button(const GameStructs::ButtonProperties& bp);
};