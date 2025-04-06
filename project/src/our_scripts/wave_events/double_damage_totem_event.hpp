#pragma once
#include "../components/WaveManager.h"
#include "../../utils/EventsSystem.hpp"
class double_damage_totem_event : public wave_event {
public:
	inline double_damage_totem_event(WaveManager* wav)
		:wave_event(wav)
	{
	}
	inline void end_wave_callback() override {
		event_system::event_manager::Instance()->fire_event(event_system::delete_event_created_thingies,
			(event_system::event_receiver::Msg{ 0 }));
	}
};