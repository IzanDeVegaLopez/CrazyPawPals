#include "MythicItems.h"
#include "../../game/Game.h"
#include "../../game/scenes/GameScene.h"
#include <iostream>

#pragma region BloodClaw
BloodClaw::BloodClaw(Health* h, Weapon* w)
	:MythicItem("BloodClaw"), _health(h), _weapon(w)
{}
void BloodClaw::apply_effects() {
	int currhealth = _health->getHealth() / 2;
	int damage = _weapon->damage() * 2;
	_health->takeDamage(currhealth);
	_weapon->set_damage(damage);
}
#pragma endregion


#pragma region ProfaneHotline
ProfaneHotline::ProfaneHotline(ManaComponent* m)
	:MythicItem("ManaSwap"), _mana(m)
{
	event_system::event_manager::Instance()->suscribe_to_event(event_system::mill, this, &event_system::event_receiver::event_callback0);
}

void 
ProfaneHotline::event_callback0(const event_system::event_receiver::Msg& m) {
	int newManaCount = _mana->mana_count() + m.int_value;
	_mana->change_mana(newManaCount);
}

void 
ProfaneHotline::apply_effects() {
	int m = _mana->mana_regen();
	_mana->change_mana_regen(-(m/2));
}

#pragma endregion

#pragma region ShieldHarvest
ShieldHarvest::ShieldHarvest(Health* h)
	:MythicItem("ShieldHarvest"), _health(h)
{
	event_system::event_manager::Instance()->suscribe_to_event(event_system::enemy_dead, this, &event_system::event_receiver::event_callback0);
}

void 
ShieldHarvest::event_callback0(const event_system::event_receiver::Msg& m) {
	int shield =  m.int_value;
	_health->takeShield(shield);
}

void 
ShieldHarvest::apply_effects() {
	int currhealth = _health->getHealth() / 2;
	_health->takeDamage(currhealth);
}

#pragma endregion

#pragma region ManaCatalyst
ManaCatalyst::ManaCatalyst(ManaComponent* m, Weapon* w)
	:MythicItem("ManaCatalyst"), _mana(m), _weapon(w){}

void 
ManaCatalyst::apply_effects() {
	_mana->change_mana_regen(_mana->mana_regen());
	int damage = _weapon->damage() / 2;
	_weapon->set_damage(damage);
}

#pragma endregion

#pragma region ManaForge
ManaForge::ManaForge(ManaComponent* m, Deck* d)
	:MythicItem("ManaForge"),  _mana(m), _deck(d), _set(false), _ini_mana(_mana->mana_regen()){}

void 
ManaForge::apply_effects() {
	_deck->set_reload_time(_deck->reload_time() + 0.5f * _deck->reload_time());
	//std::cout << "mana: " << _mana->mana_regen() << std::endl;
}

void ManaForge::update(uint32_t dt) {
	(void)dt;
	if (_deck->is_reloading()) {
		if (!_set) {
			//std::cout << "ManaForge activated" << std::endl;
			_set = true;
			_mana->change_mana_regen(0.5f *_ini_mana);
			//std::cout << "mana: " << _mana->mana_regen() << std::endl;

		}
	}
	else if(_set){
		_mana->change_mana_regen(-(_ini_mana * 0.5f));
		_set = false;
		//std::cout << "Quitando mana mana: " << _mana->mana_regen() << std::endl;
	}
}

#pragma endregion

#pragma region BloodPact
BloodPact::BloodPact(ManaComponent* m, Health* h)
	:MythicItem("BloodPact"),  _mana(m), _health(h){}

void 
BloodPact::apply_effects() {
	int currhealth = _health->getHealth() / 2;
	_health->takeDamage(currhealth);
	_mana->change_mana_regen(_mana->mana_regen());
	//std::cout << "mana: " << _mana->mana_regen() << std::endl;
}
#pragma endregion

#pragma region PreternaturalForce
PreternaturalForce::PreternaturalForce(ManaComponent* m, Weapon* w)
	:MythicItem("ArcaneInferno"),  _mana(m), _weapon(w){}

void 
PreternaturalForce::apply_effects() {
	_mana->change_mana_regen(- (_mana->mana_regen() * 0.5));
	int damage = _weapon->damage() * 2;
	_weapon->set_damage(damage);
}
#pragma endregion