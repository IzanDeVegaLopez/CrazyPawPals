#include "MythicComponent.h"
#include "../../game/Game.h"
#include "../../ecs/Manager.h"
#include "../mythic/MythicItem.h"
#include "cards/Mana.h"
#include "Health.h"
#include "movement/MovementController.h"
#include "weapons/Weapon.h"

MythicComponent::MythicComponent()
	:_mana(nullptr),_player_health(nullptr),_player_movement_controller(nullptr), _weapon(nullptr){}
MythicComponent::~MythicComponent()
{
	for (auto mythic : _obtained_mythics) {
		delete mythic;
	}
	_obtained_mythics.clear();
}

void MythicComponent::initComponent() {
	_mana = Game::Instance()->get_mngr()->getComponent<ManaComponent>(_ent);
	assert(_mana != nullptr);

	_player_health = Game::Instance()->get_mngr()->getComponent<Health>(_ent);
	assert(_player_health != nullptr);

	_player_movement_controller = Game::Instance()->get_mngr()->getComponent<MovementController>(_ent);
	assert(_player_movement_controller != nullptr);

	_weapon= Game::Instance()->get_mngr()->getComponent<Weapon>(_ent);
	assert(_weapon != nullptr);
}

void MythicComponent::add_mythic(MythicItem* mythic) {
	 if (mythic == nullptr) return;
    _obtained_mythics.push_back(mythic);
    mythic->apply_effects(_player_health, _mana, _player_movement_controller, _weapon);
}

void MythicComponent::update(uint32_t dt) {
	for (auto m : _obtained_mythics) {
		m->update(dt);
	}
}

