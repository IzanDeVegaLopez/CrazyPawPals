#include "HUD.h"
#include "game/Game.h"
#include "ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"

HUD::HUD()
{
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
}

void HUD::render()
{
	int wavetime = 60-(_wm->get_wave_time()/1000);
	//std::cout << wavetime << std::endl;
	rect_f32 timer = rect_f32_screen_rect_from_viewport(rect_f32({ 0.45,0.05 }, { 0.1,0.12 }), _camera->cam.screen);
	SDL_Rect timertrue{ timer.position.x,timer.position.y,timer.size.x,timer.size.y };
	Texture timertex{ 
		sdlutils().renderer(),
		wavetime<10 ? "0"+std::to_string(std::max(wavetime,0)) : std::to_string(wavetime),
		sdlutils().fonts().at("ARIAL16"),
		SDL_Color(wavetime < 0 ? 200 : 50,50,50,255)};
	timertex.render(timertrue);
}
