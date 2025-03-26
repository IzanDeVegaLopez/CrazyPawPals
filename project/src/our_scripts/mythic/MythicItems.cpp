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
	:MythicItem("ProfaneHotline"), _mana(m)
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

#pragma region CurtainReaper
CurtainReaper::CurtainReaper(Health* h)
	:MythicItem("CurtainReaper"), _health(h)
{
	event_system::event_manager::Instance()->suscribe_to_event(event_system::enemy_dead, this, &event_system::event_receiver::event_callback0);
}

void 
CurtainReaper::event_callback0(const event_system::event_receiver::Msg& m) {
	int shield =  m.int_value;
	_health->takeShield(shield);
}

void 
CurtainReaper::apply_effects() {
	int currhealth = _health->getHealth() / 2;
	_health->takeDamage(currhealth);
}

#pragma endregion

#pragma region Incense
Incense::Incense(ManaComponent* m, Weapon* w)
	:MythicItem("Incense"), _mana(m), _weapon(w){}

void 
Incense::apply_effects() {
	_mana->change_mana_regen(_mana->mana_regen());
	int damage = _weapon->damage() / 2;
	_weapon->set_damage(damage);
}

#pragma endregion

#pragma region ArcaneSurge
ArcaneSurge::ArcaneSurge(ManaComponent* m, Deck* d)
	:MythicItem("ArcaneSurge"),  _mana(m), _deck(d), _set(false), _ini_mana(_mana->mana_regen()){}

void 
ArcaneSurge::apply_effects() {
	_deck->set_reload_time(_deck->reload_time() - 0.5f * _deck->reload_time());
	//std::cout << "mana: " << _mana->mana_regen() << std::endl;
}

void ArcaneSurge::update(uint32_t dt) {
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
	:MythicItem("PreternaturalForce"),  _mana(m), _weapon(w){}

void 
PreternaturalForce::apply_effects() {
	_mana->change_mana_regen(- (_mana->mana_regen() * 0.5));
	int damage = _weapon->damage() * 2;
	_weapon->set_damage(damage);
}
#pragma endregion


#pragma region ClawFile
ClawFile::ClawFile(Deck* d, MovementController* mc)
	:MythicItem("ClawFile"),  _deck(d), _mc(mc), _set(false), _ini_mc(_mc->get_max_speed()){}

void 
ClawFile::apply_effects() {
	//std::cout << "deck: " << _deck->reload_time() << std::endl;
	_deck->set_reload_time(_deck->reload_time() - 0.5f * _deck->reload_time());
	//std::cout << "deck despues: " << _deck->reload_time() << std::endl;

	//std::cout << "vel: " << _mc->get_max_speed() << std::endl;
	//std::cout << "velini: " << _ini_mc << std::endl;
}

void ClawFile::update(uint32_t dt) {
	(void)dt;
	if (_deck->is_reloading()) {
		if (!_set) {
			//std::cout << "ClawFile activated" << std::endl;
			_set = true;
			_mc->set_max_speed(0.5f *_ini_mc);
			//std::cout << "vel: " << _mc->get_max_speed() << std::endl;
		}
	}
	else if(_set){
		_mc->set_max_speed((_ini_mc));
		_set = false;
		//std::cout << "vel: " << _mc->get_max_speed() << std::endl;
	}
}
#pragma endregion

#pragma region MeowOrNever
MeowOrNever::MeowOrNever(Health* h, MovementController* mc)
	:MythicItem("MeowOrNever"),  _health(h), _mc(mc){}

void 
MeowOrNever::apply_effects() {
	//std::cout << "deck: " << _health->getHealth() << std::endl;
	int currhealth = _health->getHealth() / 2;
	_health->takeDamage(currhealth);
	//std::cout << "deck despues: " << _health->getHealth() << std::endl;
	
	//std::cout << "vel: " << _mc->get_max_speed() << std::endl;
	_mc->set_max_speed(2.0f *_mc->get_max_speed());
	//std::cout << "vel: " << _mc->get_max_speed() << std::endl;
}
#pragma endregion

#pragma region ZoomiesInducer
ZoomiesInducer::ZoomiesInducer(MovementController* mc, Transform* tr, uint32_t time, uint32_t duration, float distance)
	:MythicItem("ZoomiesInducer"), _mc(mc), _tr(tr), _timer(time), _last_time(0), _distance(distance), _duration(duration){
}

void
ZoomiesInducer::apply_effects() {
	std::cout << "vel: " << _mc->get_max_speed() << std::endl;
	_mc->set_max_speed(2.0f * _mc->get_max_speed());
	std::cout << "velini: " << _mc->get_max_speed() << std::endl;
}

void ZoomiesInducer::update(uint32_t dt) {
	if (_timer + _last_time <= sdlutils().virtualTimer().currTime()) {
		std::cout << "ZoomiesInducer" << std::endl;
		_last_time = sdlutils().virtualTimer().currTime();
		Vector2D nextPos = _tr->getPos() + _tr->getDir() * _distance;
		_mc->dash(nextPos, _duration);
	}
}
#pragma endregion