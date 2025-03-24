#pragma once
#include "../components/WaveManager.h"
#include "../../utils/EventsSystem.hpp"
class double_damage_totem_event : public wave_event {
private:
	float dmg_multiplier = 2.0f;
public:
	inline double_damage_totem_event(WaveManager* wav)
		:wave_event(wav)
	{
	}

	inline void start_wave_callback() override {
		wave_event::start_wave_callback();
		event_system::event_manager::Instance()->fire_event(event_system::double_damage_totem,
			(event_system::event_receiver::Msg{ dmg_multiplier }));
		std::cout << "a" << std::endl;
		//TODO: send event for everyone out there
	}
	inline void end_wave_callback() override {
		//TODO: send event to revert
		event_system::event_manager::Instance()->fire_event(event_system::double_damage_totem,
			(event_system::event_receiver::Msg{ 1/ dmg_multiplier }));
	}
};