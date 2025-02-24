// This file is part of the course TPV2@UCM - Samir Genaim

#include "Manager.h"
#include "../our_scripts/components/render_ordering.hpp"

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

void Manager::render(sceneId_t sId){
	
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
