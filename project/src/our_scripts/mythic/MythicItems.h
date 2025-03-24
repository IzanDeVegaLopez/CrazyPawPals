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
 * @class ShieldHarvest
 * @brief ShieldHarvest is a MythicItem that modifies the player's health mechanics.
 *
 * The player’s health is reduced by 50%.
 * Upon defeating an enemy, the player gains 20% of the enemy's health as a shield.
 */
class ShieldHarvest : public event_system::event_receiver, public MythicItem {
	Health* _health;
public:
	ShieldHarvest() : _health(nullptr) {}
	ShieldHarvest(Health* h);

	void apply_effects() override;
	void event_callback0(const event_system::event_receiver::Msg& m);

	void set_health(Health* h) { _health = h; }
};

/**
 * @class ManaCatalyst
 * @brief ManaCatalyst is a MythicItem that enhances mana regeneration and reduces player's damage.
 *
 * Mana regeneration is doubled.
 * However, base weapon damage is reduced by 50%.
 */
class ManaCatalyst : public MythicItem {
	ManaComponent* _mana;
	Weapon* _weapon;
public:
	ManaCatalyst() : _mana(nullptr), _weapon(nullptr) {}
	ManaCatalyst(ManaComponent* m,Weapon* w);

	void apply_effects() override;

	void set_mana(ManaComponent* m) { _mana = m; }
	void set_weapon(Weapon* w) { _weapon = w; }
};

/**
 * @class ManaForge
 * @brief ManaForge is a MythicItem that enhances mana regeneration while affecting reload speed.
 *
 * While reloading, mana regeneration increases by 50%.
 * However, reload speed is reduced by 50%.
 */
class ManaForge : public MythicItem {
	ManaComponent* _mana;
	Deck* _deck;

	bool _set = false;
	int _ini_mana;
public:
	ManaForge() : _mana(nullptr), _deck(nullptr) {}
	ManaForge(ManaComponent* m, Deck* d);

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
 * This class reduces the player's health by 50% and doubles mana regeneration.
 */
class PreternaturalForce : public MythicItem {
	ManaComponent* _mana;
	Weapon* _weapon;
public:
	PreternaturalForce() : _mana(nullptr), _weapon(nullptr) {}
	PreternaturalForce(ManaComponent* m, Weapon* w);

	void apply_effects() override;

	void set_mana(ManaComponent* m) { _mana = m; }
	void set_health(Weapon* w) { _weapon = w; }
};

