#include "HUD.h"
#include "game/Game.h"
#include "ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"

HUD::HUD():_camera(nullptr),_wm(nullptr)
{
	_event_textures = std::vector<Texture*>(int(EVENTS_MAX)+1);
	_event_textures[int(ICE_SKATE)] = (&sdlutils().images().at("event_letters")); 
	_event_textures[STAR_SHOWER] = &sdlutils().images().at("event_letters");
}

HUD::~HUD()
{
}

void HUD::initComponent()
{
	_wm = Game::Instance()->get_mngr()->getComponent<WaveManager>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::WAVE));
	_camera = Game::Instance()->get_mngr()->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA));
}

void HUD::update(uint32_t delta_time)
{
	_displaying_wave_event = sdlutils().currRealTime() < _current_wave_event_time;
	//if (!_displaying_wave_event) std::cout << "END" << std::endl;
}

void HUD::render()
{
#pragma region number
	int wavenum = _wm->get_current_wave();
	rect_f32 num = rect_f32_screen_rect_from_viewport(rect_f32({ 0.85,0.1 }, { 0.07,0.05 }), _camera->cam.screen);
	SDL_Rect numtrue{ 
		int(num.position.x),
		int(num.position.y),
		int(num.size.x),
		int(num.size.y)
	};
	Texture numtex{
		sdlutils().renderer(),
		"0"+std::to_string(wavenum) + "/10",
		sdlutils().fonts().at("ARIAL16"),
		SDL_Color(50,50,50,255) };
	numtex.render(numtrue);
#pragma endregion


#pragma region timer
	int wavetime = 60 - (_wm->get_wave_time() / 1000);
	
	rect_f32 timer = rect_f32_screen_rect_from_viewport(rect_f32({ 0.45,0.05 }, { 0.1,0.14 }), _camera->cam.screen);
	SDL_Rect timertrue{
		int(timer.position.x),
		int(timer.position.y),
		int(timer.size.x),
		int(timer.size.y)
	};
	Texture timertex{
		sdlutils().renderer(),
		wavetime < 10 ? "0" + std::to_string(std::max(wavetime,0)) : std::to_string(wavetime),
		sdlutils().fonts().at("ARIAL16"),
		SDL_Color(wavetime <= 0 ? 200 : 50,50,50,255) };
	timertex.render(timertrue);
#pragma endregion

#pragma region events
	if (_displaying_wave_event) {
		
		rect_f32 event = rect_f32_screen_rect_from_viewport(rect_f32({ 0.275,0.2 }, { 0.45,0.15 }), _camera->cam.screen);
		SDL_Rect eventtrue{
			int(event.position.x),
			int(event.position.y),
			int(event.size.x),
			int(event.size.y)
		};
		_event_textures[int(_current_event)]->render(eventtrue);
	}
#pragma endregion

}

void HUD::start_new_wave()
{
	_current_event = _wm->get_current_event();
	if (int(_current_event != -1)) {
		
		_current_wave_event_time = sdlutils().currRealTime() + _wave_event_timeout;
		_displaying_wave_event = true;
	}
}

void HUD::reset()
{
	_current_event = NONE;
	_current_wave_event_time = 0;
	_displaying_wave_event = false;
}