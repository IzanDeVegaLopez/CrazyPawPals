// This file is part of the course TPV2@UCM - Samir Genaim

#include "Manager.h"
#include "../our_scripts/components/render_ordering.hpp"
#include "../our_scripts/components/rigidbody_component.hpp"
#include "../physics/collision.hpp"
#include "../our_scripts/components/camera_component.hpp"
#include <cstdlib>

namespace ecs {

Manager::Manager() :
		_hdlrs(), //
		_entsByGroup() //
{

	// for each group we reserve space for 100 entities,
	// just to avoid copies
	//
	for (auto &groupEntities : _entsByGroup) {
		groupEntities.reserve(100);
	}
}

Manager::~Manager() {

	// delete all entities
	//
	for (auto &ents : _entsByGroup) {
		for (auto e : ents)
			delete e;
	}
}

static position2_f32 position2_f32_from_vec2(const Vector2D vec) {
	return position2_f32{
		.x = vec.getX(),
		.y = vec.getY(),
	};
}
static collision_body collision_body_from_collisionable(const collisionable c) {
	return collision_body{
		.body{
			.body = c.rect.rect,
			.space{
				.position = position2_f32_from_vec2(c.transform.getPos()),
				.previous_position = position2_f32_from_vec2(c.transform.get_previous_position()),
			},
			.mass_rcp = c.rigidbody.inverse_mass,
		},
		.restitution = c.rigidbody.restitution_coefficient,
	};
}

enum collision_response_options {
	collision_response_option_none = 0,
	collision_response_option_body0_trigger = 1 << 0,
	collision_response_option_body1_trigger = 1 << 1,
};
typedef uint8_t collision_response_flags;

static bool manager_handle_collision_bodies(
	Manager &manager,
	const ecs::entity_t entity0,
	const ecs::entity_t entity1,
	collision_body &body0,
	collision_body &body1,
	seconds_f32 delta_time,
	const collision_response_flags flags
) {
	(void)entity0;
	(void)entity1;

	auto &&camera = *manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA));
	if (!collision_body_check_broad(body0, body1)) {
		return false;
	}

	collision_contact contact;
	if (collision_body_check(body0, body1, delta_time, contact)) {
		switch (flags & (collision_response_option_body0_trigger | collision_response_option_body1_trigger)) {
		case collision_response_option_none: {
			const collision_response_pairs responses = collision_body_resolve(body0, body1, contact);
			auto &response0_separation = responses.penetration_responses[0];
			auto &response1_separation = responses.penetration_responses[1];
	
			auto &response0_restitution = responses.restitution_responses[0];
			auto &response1_restitution = responses.restitution_responses[1];
			{
				auto &space0 = body0.body.space;
				auto &space1 = body1.body.space;
				
				space0.position.x += response0_separation.separation.x;
				space0.position.y += response0_separation.separation.y;
	
				space1.position.x += response1_separation.separation.x;
				space1.position.y += response1_separation.separation.y;
	
				space0.previous_position.x = space0.position.x - response0_restitution.restitution_displacement.x;
				space0.previous_position.y = space0.position.y - response0_restitution.restitution_displacement.y;
				
				space1.previous_position.x = space1.position.x - response1_restitution.restitution_displacement.x;
				space1.previous_position.y = space1.position.y - response1_restitution.restitution_displacement.y;
			}
			break;
		}
		case collision_response_option_body0_trigger: {
			// TODO: trigger body0
			break;
		}
		case collision_response_option_body1_trigger: {
			// TODO: trigger body1
			break;
		}
		case collision_response_option_body0_trigger | collision_response_option_body1_trigger: {
			// TODO: trigger body0 and body1
			break;
		}
		default: {
			assert(false && "unreachable");
			std::exit(EXIT_FAILURE);
		}
		}
		return true;
	} else {
		return false;
	}
}

static void manager_update_collisions(Manager &manager, const std::vector<ecs::entity_t> &entities, const uint32_t delta_time_milliseconds) {
	switch (entities.size()) {
	case 0:
		break;
	case 1:
		break;
	default: {
		const float delta_time_seconds = delta_time_milliseconds / 1000.0f;
		for (size_t i = 0; i < entities.size(); i++) {
			auto entity = entities[i];
			auto entity_collisionable = manager.getComponent<collisionable>(entity);
			if (entity_collisionable != nullptr) {
				collision_body body = collision_body_from_collisionable(*entity_collisionable);

				for (size_t j = i + 1; j < entities.size(); j++) {
					auto other_entity = entities[j];
					auto other_collisionable = manager.getComponent<collisionable>(other_entity);
					if (other_collisionable != nullptr) {
						collision_body other_body = collision_body_from_collisionable(*other_collisionable);

						bool collided = manager_handle_collision_bodies(manager, entity, other_entity, body, other_body, delta_time_seconds, (
							((entity_collisionable->options & collisionable_option_trigger) != 0)
								| (((other_collisionable->options & collisionable_option_trigger) != 0) << 1)
						));

						if (collided) {
							entity_collisionable->transform.getPos() = Vector2D{
								body.body.space.position.x,
								body.body.space.position.y,
							};
							other_collisionable->transform.getPos() = Vector2D{
								other_body.body.space.position.x,
								other_body.body.space.position.y,
							};
	
							entity_collisionable->transform.getPos() += Vector2D{
								(body.body.space.position.x - body.body.space.previous_position.x),
								(body.body.space.position.y - body.body.space.previous_position.y),
							};
							other_collisionable->transform.getPos() += Vector2D{
								(other_body.body.space.position.x - other_body.body.space.previous_position.x),
								(other_body.body.space.position.y - other_body.body.space.previous_position.y),
							};
						}
					}
				}
			}
		}
	}
	}
}

void Manager::update(sceneId_t sId, Uint32 dt) {
	auto& entities = getEntitiesByScene(sId);
	for (auto &ents : entities) {
		update(ents, dt);
	}
	manager_update_collisions(*this, entities, dt);
}

void Manager::render(sceneId_t sId) {
    auto& _entity = getEntitiesByScene(sId);
	switch (_entity.size()) {
	case 0:
		break;
	case 1:
		render(_entity.front());
		break;
	default: {
		auto last_ordered_entity = _entity.front();
		for (size_t i = 1; i < _entity.size(); i++) {
			auto current_entity = _entity[i];
			
			render_ordering *last_ordered = getComponent<render_ordering>(last_ordered_entity);
			render_ordering *current_ordered = getComponent<render_ordering>(current_entity);

			if (last_ordered != nullptr && current_ordered != nullptr) {
				if (current_ordered->ordinal < last_ordered->ordinal) {
					render(current_entity);
				} else {
					render(last_ordered_entity);
					last_ordered_entity = current_entity;
				}
			} else {
				render(last_ordered_entity);
				last_ordered_entity = current_entity;
			}
		}
		assert(last_ordered_entity != nullptr);
		render(last_ordered_entity);
	}
	}

	for (size_t i = 0; i < dbg_rect_col_size; i++) {
		SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdlutils().renderer(), &dbg_rect_col[i]);
	}
	dbg_rect_col_size = 0;

	const camera_screen &camera = getComponent<camera_component>(getHandler(ecs::hdlr::CAMERA))->cam;
	for (size_t i = 0; i < dbg_pos_size; i++) {
		SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 255, 0, 255);

		auto a =SDL_Rect_screen_rect_from_global(
			rect_f32{
				.position = {
					.x = dbg_pos[0][i].x,
					.y = dbg_pos[0][i].y,
				},
				.size = {
					.x = 0.1f,
					.y = 0.1f,
				},
			},
			camera
		);
		auto b = SDL_Rect_screen_rect_from_global(
			rect_f32{
				.position = {
					.x = dbg_pos[1][i].x,
					.y = dbg_pos[1][i].y,
				},
				.size = {
					.x = 0.1f,
					.y = 0.1f,
				},
			},
			camera
		);
		SDL_RenderDrawLine(
			sdlutils().renderer(),
			a.x,
			a.y,
			b.x,
			b.y
		);
	}
	dbg_pos_size = 0;
}

void Manager::refresh()
{

    // remove dead entities from the groups lists, and also those
	// do not belong to the group anymore
	for (ecs::grpId_t gId = 0; gId < ecs::maxGroupId; gId++) {
		auto &groupEntities = _entsByGroup[gId];
		groupEntities.erase(
				std::remove_if(groupEntities.begin(), groupEntities.end(),
						[this](Entity *e) {
							if (isAlive(e)) {
								return false;
							} 
							else {
								auto& sceneEntities = _entsByScene[e->_sId];
								sceneEntities.erase(
									std::remove(sceneEntities.begin(), sceneEntities.end(), e),
									sceneEntities.end()
								);
								delete e;
								return true;
							}
						}), groupEntities.end());
	}

	for (auto e : _pendingEntities) {
		_entsByGroup[e->_gId].push_back(e);
		_entsByScene[e->_sId].push_back(e);
	}
	_pendingEntities.clear();
}
} // end of namespace
