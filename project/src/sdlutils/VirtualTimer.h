// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL.h>
#include <cassert>

/*
 * This class implements a virtual timer, i.e., a timer that can be paused and
 * resumed.
 */

struct VirtualTimer {
	inline void resetTime() {
		current_time = 0;
	}

	inline Uint32 currRealTime() const {
		return current_time;
	}

	inline Uint32 currTime() const {
		return current_time;
	}

	uint64_t current_time;
};

