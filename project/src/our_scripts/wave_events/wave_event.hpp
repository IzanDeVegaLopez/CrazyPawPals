#pragma once

class WaveManager;

class wave_event {
public:
	inline wave_event(WaveManager* wav) : _wave_manager_ref(wav), _time_since_wave_started(0) {};
	virtual void start_wave_callback();
	virtual void end_wave_callback();
	virtual void update(unsigned int delta_time);
protected:
	WaveManager* _wave_manager_ref;
	virtual void pronounce_event_anim();
	virtual void hide_event_anim();
	unsigned int _time_since_wave_started;
	unsigned int _time_to_stop_showing_wave_descriptor = 5000;
	//needs a reference to wave manager to manage its dynimage component
};