#pragma once
#include "../components/WaveManager.h"
#include "../../utils/EventsSystem.hpp"
class paw_patrol_event : public wave_event {
public:
	inline paw_patrol_event(WaveManager* wav)
		:wave_event(wav)
	{
	}

	inline void start_wave_callback() override {
		wave_event::start_wave_callback();
		//TODO: send event for everyone out there
	}
	inline void end_wave_callback() override {
		//TODO: send event to revert
		event_system::event_manager::Instance()->fire_event(event_system::delete_event_created_thingies,
			(event_system::event_receiver::Msg{ 0 }));
	}
};