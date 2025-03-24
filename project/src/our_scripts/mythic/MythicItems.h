#pragma once
#include "MythicItem.h"
#include "../../utils/EventsSystem.hpp"

/**
 * @class BloodClaw
 * @brief BloodClaw is a MythicItem that applies effects to the player's health and weapon.
 *
 * This class reduces the player's health by 50% and doubles weapon damage when applied.
 */
class BloodClaw : public MythicItem {
	Health* _health;
	Weapon* _weapon;
public:
	BloodClaw() : _health(nullptr), _weapon(nullptr) {}
	BloodClaw(Health* h, Weapon* w);

	void apply_effects() override;

	void set_health(Health* h) { _health = h; }
	void set_weapon(Weapon* w) { _weapon = w; }
};

/**
 * @class ProfaneHotline
 * @brief ProfaneHotline is a MythicItem that affects the player's mana management.
 *
 * When a card is milled, the player gains mana equal to its cost.
 * However, mana regeneration is reduced by 50%.
 */
class ProfaneHotline :public event_system::event_receiver, public MythicItem {
	ManaComponent* _mana;
public:
	ProfaneHotline() : _mana(nullptr) {}
	ProfaneHotline(ManaComponent* m);

	void apply_effects() override;
	void event_callback0(const event_system::event_receiver::Msg& m);

	void set_mana(ManaComponent* m) { _mana = m; }
};

/**
 * @class CurtainReaper
 * @brief CurtainReaper is a MythicItem that modifies the player's health mechanics.
 *
 * The player’s health is reduced by 50%.
 * Upon defeating an enemy, the player gains 20% of the enemy's health as a shield.
 */
class CurtainReaper : public event_system::event_receiver, public MythicItem {
	Health* _health;
public:
	CurtainReaper() : _health(nullptr) {}
	CurtainReaper(Health* h);

	void apply_effects() override;
	void event_callback0(const event_system::event_receiver::Msg& m);

	void set_health(Health* h) { _health = h; }
};

/**
 * @class Incense
 * @brief Incense is a MythicItem that enhances mana regeneration and reduces player's damage.
 *
 * Mana regeneration is doubled.
 * However, base weapon damage is reduced by 50%.
 */
class Incense : public MythicItem {
	ManaComponent* _mana;
	Weapon* _weapon;
public:
	Incense() : _mana(nullptr), _weapon(nullptr) {}
	Incense(ManaComponent* m,Weapon* w);

	void apply_effects() override;

	void set_mana(ManaComponent* m) { _mana = m; }
	void set_weapon(Weapon* w) { _weapon = w; }
};

/**
 * @class ArcaneSurge
 * @brief ArcaneSurge is a MythicItem that enhances mana regeneration while affecting reload speed.
 *
 * While reloading, mana regeneration increases by 50%.
 * However, reload speed is reduced by 50%.
 */
class ArcaneSurge : public MythicItem {
	ManaComponent* _mana;
	Deck* _deck;

	bool _set = false;
	int _ini_mana;
public:
	ArcaneSurge() : _mana(nullptr), _deck(nullptr),_ini_mana() {}
	ArcaneSurge(ManaComponent* m, Deck* d);

	void apply_effects() override;
	void update(uint32_t dt) override;

	void set_mana(ManaComponent* m) { _mana = m; }
	void set_deck(Deck* d) { _deck = d; }
};
	
/**
 * @class BloodPact
 * @brief BloodPact is a MythicItem that enhances mana regeneration while affecting the player's health.
 *
 * This class reduces the player's health by 50% and doubles mana regeneration.
 */
class BloodPact : public MythicItem {
	ManaComponent* _mana;
	Health* _health;
public:
	BloodPact() : _mana(nullptr), _health(nullptr) {}
	BloodPact(ManaComponent* m, Health* h);

	void apply_effects() override;

	void set_mana(ManaComponent* m) { _mana = m; }
	void set_health(Health* h) { _health = h; }
};
	
/**
 * @class PreternaturalForce
 * @brief PreternaturalForce is a MythicItem that enhances player's damage and reduces mana production.
 *
 * This class reduces the player's  mana regeneration by 50% and doubles damage.
 */
class PreternaturalForce : public MythicItem {
	ManaComponent* _mana;
	Weapon* _weapon;
public:
	PreternaturalForce() : _mana(nullptr), _weapon(nullptr) {}
	PreternaturalForce(ManaComponent* m, Weapon* w);

	void apply_effects() override;

	void set_mana(ManaComponent* m) { _mana = m; }
	void set_weapon(Weapon* w) { _weapon = w; }
};

/**
 * @class ClawFile
 * @brief ClawFile is a MythicItem that enhances player's reload speed and reduces movement speed.
 *
 * While reloading, movement speed reduced by 50%.
 * However, reload speed is reduced by 50%.
 */
class ClawFile : public MythicItem {
	MovementController* _mc;
	Deck* _deck;

	bool _set = false;
	float _ini_mc;
public:
	ClawFile() : _mc(nullptr), _deck(nullptr) {}
	ClawFile(Deck* d, MovementController* mc);

	void apply_effects() override;
	void update(uint32_t dt) override;

	void set_deck(Deck* d) { _deck = d; }
	void set_movement_controller(MovementController* mc) { _mc = mc; }
};