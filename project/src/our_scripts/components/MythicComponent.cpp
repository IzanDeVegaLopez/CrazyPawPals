#include "MythicComponent.h"
#include "../../game/Game.h"
#include "../../ecs/Manager.h"
#include "../mythic/MythicItem.h"
#include "../mythic/MythicItems.h"
#include "cards/Mana.h"
#include "Health.h"
#include "movement/MovementController.h"
#include "weapons/Weapon.h"

MythicComponent::MythicComponent() {
	_obtained_mythics.push_back(new BloodClaw);
}


MythicComponent::~MythicComponent(){
	for (auto mythic : _obtained_mythics) {
		delete mythic;
	}
	_obtained_mythics.clear();
}

void 
MythicComponent::initComponent() {
}

void 
MythicComponent::add_mythic(MythicItem* mythic) {
	 if (mythic == nullptr) return;
    _obtained_mythics.push_back(mythic);
    mythic->apply_effects();
}

void 
MythicComponent::update(uint32_t dt) {
	for (auto m : _obtained_mythics) {
		m->update(dt);
	}
}

