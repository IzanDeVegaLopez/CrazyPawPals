#pragma once
#include "Transform.h"
#include "../Health.h"
#include "../../../ecs/Manager.h"
#include "../../../game/Game.h"

struct Follow 
{
	Transform* search_closest_player(Transform* _tr){
		Transform* player = nullptr;
		float min_distance = 1000.0f;
		auto&& manager = *Game::Instance()->get_mngr();
		auto playerEntities = manager.getEntities(ecs::grp::PLAYER);
		for(auto playerEntity : playerEntities) {
			auto tr = manager.getComponent<Transform>(playerEntity);
			float distance = (tr->getPos() - _tr->getPos()).magnitude();
			if (distance < min_distance) {
				min_distance = distance;
				player = tr;
			}
		}
		return player;
	}

	Transform* search_furthest_player(Transform* _tr){
		Transform* player = nullptr;
		float max_distance = 0.0f;
		auto&& manager = *Game::Instance()->get_mngr();
		auto playerEntities = manager.getEntities(ecs::grp::PLAYER);
		for(auto playerEntity : playerEntities) {
			auto tr = manager.getComponent<Transform>(playerEntity);
			float distance = (tr->getPos() - _tr->getPos()).magnitude();
			if (distance > max_distance) {
				max_distance = distance;
				player = tr;
			}
		}
		return player;
	}

	Transform* search_lowest_life_player(){
		Transform* player = nullptr;
		float min_life = 1000.0f;
		auto&& manager = *Game::Instance()->get_mngr();
		auto playerEntities = manager.getEntities(ecs::grp::PLAYER);
		for(auto playerEntity : playerEntities) {
			auto tr = manager.getComponent<Transform>(playerEntity);
			auto health = manager.getComponent<Health>(playerEntity);
			if (health->getHealth() < min_life) {
				min_life = health->getHealth();
				player = tr;
			}
		}
		return player;
	}

	Transform* search_highest_life_player(){
		Transform* player = nullptr;
		float max_life = 0.0f;
		auto&& manager = *Game::Instance()->get_mngr();
		auto playerEntities = manager.getEntities(ecs::grp::PLAYER);
		for(auto playerEntity : playerEntities) {
			auto tr = manager.getComponent<Transform>(playerEntity);
			auto health = manager.getComponent<Health>(playerEntity);
			if (health->getHealth() > max_life) {
				max_life = health->getHealth();
				player = tr;
			}
		}
		return player;
	}	
};