#pragma once
#include "MythicItem.h"
#include "../../utils/EventsSystem.hpp"

class BloodClaw : public MythicItem {
public:
	BloodClaw();
	void apply_effects(Health* health, ManaComponent* mana, MovementController* movement, Weapon* weapon) override;
};

class ManaSwap :public event_system::event_receiver, public MythicItem {
	bool _has_mill;
	ManaComponent* _mana;
public:
	ManaSwap();
	void apply_effects(Health* health, ManaComponent* mana, MovementController* movement, Weapon* weapon) override;
	void event_callback0(const event_system::event_receiver::Msg& m);
};
