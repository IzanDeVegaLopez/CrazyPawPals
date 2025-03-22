#include "MythicItems.h"
#include "../../game/Game.h"
#include "../../game/scenes/GameScene.h"

#pragma region BloodClaw
BloodClaw::BloodClaw()
	:MythicItem("BloodClaw")
{}
void BloodClaw::apply_effects(Health* health, ManaComponent* mana, MovementController* movement, Weapon* weapon) {
	int currhealth = health->getHealth() / 2;
	int damage = weapon->damage() * 2;
	health->takeDamage(currhealth);
	weapon->set_damage(damage);
}
#pragma endregion


#pragma region ManaSwap
ManaSwap::ManaSwap()
	:MythicItem("ManaSwap"), _has_mill(false), _mana(nullptr)
{
	event_system::event_manager::Instance()->suscribe_to_event(event_system::mill, this, &event_system::event_receiver::event_callback0);
}
void ManaSwap::event_callback0(const event_system::event_receiver::Msg& m) {
	_has_mill = true;
	int newManaCount = _mana->mana_count() + m.mana;
	_mana->change_mana(newManaCount);
}
void ManaSwap::apply_effects(Health* health, ManaComponent* mana, MovementController* movement, Weapon* weapon) {
	_mana = mana;
	int m = _mana->mana_regen();
	_mana->change_mana_regen(-(m/2));
}

#pragma endregion
