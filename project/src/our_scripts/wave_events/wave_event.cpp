#include "wave_event.hpp"
#include "../components/WaveManager.h"

void wave_event::start_wave_callback()
{
	pronounce_event_anim();
}

void wave_event::end_wave_callback()
{

}

void wave_event::update(unsigned int delta_time)
{
	_time_since_wave_started += delta_time;
	if (_time_since_wave_started > _time_to_stop_showing_wave_descriptor)
		hide_event_anim();
}

void wave_event::pronounce_event_anim()
{
	//TODO
}

void wave_event::hide_event_anim()
{
	//TODO
}
