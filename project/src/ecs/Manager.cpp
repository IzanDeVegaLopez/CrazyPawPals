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


#ifndef DBG_COLLISIONS
#define DBG_COLLISIONS_DEFAULT false
#define DBG_COLLISIONS DBG_COLLISIONS_DEFAULT
#endif

#if DBG_COLLISIONS
#define DBG_COLLISIONS_BUFFER_SIZE 4096
struct dbg_collisions {
	SDL_Rect dbg_rect_col[DBG_COLLISIONS_BUFFER_SIZE];
	size_t dbg_rect_col_size;

	position2_f32 dbg_pos[2][DBG_COLLISIONS_BUFFER_SIZE];
	size_t dbg_pos_size;
};

static dbg_collisions dbg_col = {0};
static void dbg_collision_fill_rect(ecs::Manager &manager, const collision_body &body0, const collision_body &body1) {
	auto &&camera = *manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA));
	
	auto &&dbg_rect_col = dbg_col.dbg_rect_col;
	auto &&dbg_rect_col_size = dbg_col.dbg_rect_col_size;
	
	assert(dbg_rect_col_size < sizeof(dbg_rect_col) / sizeof(dbg_rect_col[0]));
	dbg_rect_col[dbg_rect_col_size] = SDL_Rect_screen_rect_from_global(
		rect_f32{
			.position = {
				.x = body0.body.body.position.x + body0.body.space.position.x - body0.body.body.size.x * 0.5f,
				.y = body0.body.body.position.y + body0.body.space.position.y + body0.body.body.size.y * 0.5f,
			},
			.size = {
				.x = body0.body.body.size.x,
				.y = body0.body.body.size.y,
			},
		},
		camera.cam
	);
	++dbg_rect_col_size;
	dbg_rect_col[dbg_rect_col_size] = SDL_Rect_screen_rect_from_global(
		rect_f32{
			.position = {
				.x = body1.body.body.position.x + body1.body.space.position.x - body1.body.body.size.x * 0.5f,
				.y = body1.body.body.position.y + body1.body.space.position.y + body1.body.body.size.y * 0.5f,
			},
			.size = {
				.x = body1.body.body.size.x,
				.y = body1.body.body.size.y,
			},
		},
		camera.cam
	);
	++dbg_rect_col_size;

	dbg_rect_col[dbg_rect_col_size] = SDL_Rect_screen_rect_from_global(
		rect_f32{
			.position = {
				.x = body1.body.body.position.x + body1.body.space.position.x - (body1.body.body.size.x + body0.body.body.size.x) * 0.5f,
				.y = body1.body.body.position.y + body1.body.space.position.y + (body1.body.body.size.y + body0.body.body.size.y) * 0.5f,
			},
			.size = {
				.x = body1.body.body.size.x + body0.body.body.size.x,
				.y = body1.body.body.size.y + body0.body.body.size.y,
			},
		},
		camera.cam
	);
	++dbg_rect_col_size;

	dbg_rect_col[dbg_rect_col_size] = SDL_Rect_screen_rect_from_global(
		rect_f32{
			.position = {
				.x = - (body1.body.body.size.x + body0.body.body.size.x) * 0.5f,
				.y = + (body1.body.body.size.y + body0.body.body.size.y) * 0.5f,
			},
			.size = {
				.x = body1.body.body.size.x + body0.body.body.size.x,
				.y = body1.body.body.size.y + body0.body.body.size.y,
			},
		},
		camera.cam
	);
	++dbg_rect_col_size;

	dbg_rect_col[dbg_rect_col_size] = SDL_Rect_screen_rect_from_global(
		rect_f32{
			.position = {
				.x = (body0.body.space.previous_position.x + body0.body.body.position.x - (body1.body.body.position.x + body1.body.space.previous_position.x)),
				.y = (body0.body.space.previous_position.y + body0.body.body.position.y - (body1.body.body.position.y + body1.body.space.previous_position.y)),
			},
			.size = {
				.x = 0.0f,
				.y = 0.0f,
			},
		},
		camera.cam
	);
	++dbg_rect_col_size;
}

static void dbg_collsion_fill_point_pair(ecs::Manager &manager, const collision_contact &contact, const collision_body &body0, const collision_body &body1) {
	(void)manager;
	(void)body1;

	auto &&dbg_pos = dbg_col.dbg_pos;
	auto &&dbg_pos_size = dbg_col.dbg_pos_size;

	dbg_pos[0][dbg_pos_size] = body0.body.space.position;
	dbg_pos[1][dbg_pos_size] = position2_f32{
		.x = body0.body.space.position.x + contact.penetration.penetration.x * 20.0f,
		.y = body0.body.space.position.y + contact.penetration.penetration.y * 20.0f,
	};
	++dbg_pos_size;
}

static void dbg_collision_render_drain(ecs::Manager &manager) {
	auto &&dbg_rect_col = dbg_col.dbg_rect_col;
	auto &&dbg_rect_col_size = dbg_col.dbg_rect_col_size;

	auto &&dbg_pos = dbg_col.dbg_pos;
	auto &&dbg_pos_size = dbg_col.dbg_pos_size;

	for (size_t i = 0; i < dbg_rect_col_size; i++) {
		SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdlutils().renderer(), &dbg_rect_col[i]);
	}
	dbg_rect_col_size = 0;

	const camera_screen &camera = manager.getComponent<camera_component>(manager.getHandler(ecs::hdlr::CAMERA))->cam;
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
#endif

static position2_f32 position2_f32_from_vec2(const Vector2D vec) {
	return position2_f32{
		.x = vec.getX(),
		.y = vec.getY(),
	};
}
static collision_body collision_body_from_collisionable(const collisionable c) {
	return collision_body{
		.body{
			.body = rect_f32{
				.position = position2_f32{
					.x = c.rigidbody.rect.position.x + c.rect.rect.position.x,
					.y = c.rigidbody.rect.position.y + c.rect.rect.position.y,
				},
				.size = size2_f32{
					.x = c.rigidbody.rect.size.x * c.rect.rect.size.x,
					.y = c.rigidbody.rect.size.y * c.rect.rect.size.y,
				},
			},
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
#if DBG_COLLISIONS
	dbg_collision_fill_rect(manager, body0, body1);
#endif
	if (!collision_body_check_broad(body0, body1)) {
		return false;
	}

	collision_contact contact;
	if (collision_body_check(body0, body1, delta_time, contact)) {
#if DBG_COLLISIONS
		dbg_collsion_fill_point_pair(manager, contact, body0, body1);
#endif
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

			manager.addComponent<contact_manifold>(entity0, contact, entity0, entity1);
			manager.addComponent<contact_manifold>(entity1, contact, entity0, entity1);
			break;
		}
		case collision_response_option_body0_trigger: {
			manager.addComponent<trigger_manifold>(entity0, contact, entity0, entity1);
			break;
		}
		case collision_response_option_body1_trigger: {
			manager.addComponent<trigger_manifold>(entity1, contact, entity0, entity1);
			break;
		}
		case collision_response_option_body0_trigger | collision_response_option_body1_trigger: {
			manager.addComponent<trigger_manifold>(entity0, contact, entity0, entity1);
			manager.addComponent<trigger_manifold>(entity1, contact, entity0, entity1);
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

		constexpr static const size_t max_collision_passes = 8;
		size_t last_pass_collision_count;
		size_t pass_count = 0;
		do {
			last_pass_collision_count = 0;
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
								const vec2_f32 displacement = vec2_f32{
									.x = body.body.space.position.x - body.body.space.previous_position.x,
									.y = body.body.space.position.y - body.body.space.previous_position.y,
								};
								const float displacement_length_sqr = 
									displacement.x * displacement.x + displacement.y * displacement.y;
								constexpr static const float epsilon_displacement_length_sqr = 0.0000001f;
								if (displacement_length_sqr > epsilon_displacement_length_sqr) {
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
	
									++last_pass_collision_count;
								} 
							}
						}
					}
				}
			}
			++pass_count;
		} while (last_pass_collision_count > 0 && pass_count < max_collision_passes);
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
#if DBG_COLLISIONS
	dbg_collision_render_drain(*this);
#endif
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
