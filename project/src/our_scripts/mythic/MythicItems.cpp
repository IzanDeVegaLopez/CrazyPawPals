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
	:MythicItem("ManaSwap"), _mana(nullptr)
{
	event_system::event_manager::Instance()->suscribe_to_event(event_system::mill, this, &event_system::event_receiver::event_callback0);
}
void ManaSwap::event_callback0(const event_system::event_receiver::Msg& m) {
	int newManaCount = _mana->mana_count() + m.int_value;
	_mana->change_mana(newManaCount);
}
void ManaSwap::apply_effects(Health* health, ManaComponent* mana, MovementController* movement, Weapon* weapon) {
	_mana = mana;
	int m = _mana->mana_regen();
	_mana->change_mana_regen(-(m/2));
}

#pragma endregion

#pragma region ShieldHarvest
ShieldHarvest::ShieldHarvest()
	:MythicItem("ManaSwap"), _health(nullptr)
{
	event_system::event_manager::Instance()->suscribe_to_event(event_system::enemy_dead, this, &event_system::event_receiver::event_callback0);
}
void ShieldHarvest::event_callback0(const event_system::event_receiver::Msg& m) {
	int shield =  m.int_value;
	_health->takeShield(shield);
}
void ShieldHarvest::apply_effects(Health* health, ManaComponent* mana, MovementController* movement, Weapon* weapon) {
	_health = health;
	int currhealth = _health->getHealth() / 2;
	_health->takeDamage(currhealth);
}

#pragma endregion
