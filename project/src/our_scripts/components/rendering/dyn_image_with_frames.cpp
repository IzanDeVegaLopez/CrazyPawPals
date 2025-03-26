#include "dyn_image_with_frames.hpp"
#include "sdlutils/Texture.h"
#include "sdlutils/SDLUtils.h"


void dyn_image_with_frames::update(uint32_t delta_time)
{
    if (sdlutils().virtualTimer().currTime() > next_frame_time) {
        current_frame++;
        if (current_frame >= end_frame)current_frame = ini_frame;
        subrect.position.x = current_frame * subrect.size.x;
        next_frame_time = sdlutils().virtualTimer().currTime() + frame_duration;
    }
}
