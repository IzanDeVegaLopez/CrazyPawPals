#include "HUD.h"
#include "../../../game/Game.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../rendering/card_rendering.hpp"
#include "../WaveManager.h"
HUD::HUD() : _tex_orb(&sdlutils().images().at("manaorb")), _tex_orb_empty(&sdlutils().images().at("manaorbempty")), _tex_prime(&sdlutils().images().at("prime"))
{

}

HUD::~HUD()
{
}

void HUD::initComponent()
{
	_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	_mana = Game::Instance()->get_mngr()->getComponent<ManaComponent>(_ent);
	_health = Game::Instance()->get_mngr()->getComponent<Health>(_ent);
	_deck = Game::Instance()->get_mngr()->getComponent<Deck>(_ent);
	_camera = Game::Instance()->get_mngr()->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA));
}

void HUD::update(uint32_t delta_time)
{
}

void HUD::render()
{
	SDL_SetRenderDrawBlendMode(sdlutils().renderer(), SDL_BLENDMODE_NONE);

#pragma region health
	int health = _health->getHealth();
	int max_health = _health->getMaxHealth();
	static constexpr float health_scale = 0.0015;

	//bg
	SDL_SetRenderDrawColor(sdlutils().renderer(), 80, 55, 60, 255);
	rect_f32 health1 = rect_f32_screen_rect_from_viewport(rect_f32({ 0.13,0.82 }, { max_health * health_scale,0.03 }),_camera->cam.screen);
	SDL_Rect health1true { health1.position.x,health1.position.y,health1.size.x,health1.size.y};
	SDL_RenderFillRect(sdlutils().renderer(), &health1true);

	//remaining
	SDL_SetRenderDrawColor(sdlutils().renderer(), 200, 80, 100, 255);
	rect_f32 health2 = rect_f32_screen_rect_from_viewport(rect_f32({ 0.13,0.82 }, { health * health_scale,0.03 }), _camera->cam.screen);
	SDL_Rect health2true { health2.position.x,health2.position.y,health2.size.x,health2.size.y };
	SDL_RenderFillRect(sdlutils().renderer(), &health2true);

#pragma endregion

#pragma region mana
	int mana_count = _mana->raw_mana_count();
	int max_mana = _mana->max_mana();
	static constexpr float mana_scale = 0.000025;

	//bg
	SDL_SetRenderDrawColor(sdlutils().renderer(), 60, 55, 80, 255);
	rect_f32 mana1 = rect_f32_screen_rect_from_viewport(rect_f32({ 0.13,0.86 }, { max_mana*mana_scale,0.024 }), _camera->cam.screen);
	SDL_Rect mana1true{ mana1.position.x,mana1.position.y,mana1.size.x,mana1.size.y };
	SDL_RenderFillRect(sdlutils().renderer(), &mana1true);

	//progress
	SDL_SetRenderDrawColor(sdlutils().renderer(), 81, 100, 222, 255);
	rect_f32 mana2 = rect_f32_screen_rect_from_viewport(rect_f32({ 0.13,0.86 }, { (mana_count%1000)*5 * mana_scale,0.024 }), _camera->cam.screen);
	SDL_Rect mana2true{ mana2.position.x,mana2.position.y,mana2.size.x,mana2.size.y };
	SDL_RenderFillRect(sdlutils().renderer(), &mana2true);

	// full mana orbs
	int display = max_mana / 1000;
	for (int i = 1; i <= display; i++) {
		rect_f32 output = rect_f32_screen_rect_from_viewport(rect_f32({ 0.13+(0.024*(i-1)),0.894}, {0.022,0.022}), _camera->cam.screen);
		SDL_Rect outputtrue{ output.position.x,output.position.y,output.size.x,output.size.x };
		if (mana_count >= i * 1000) _tex_orb->render(outputtrue);
		else _tex_orb_empty->render(outputtrue);
	}

	//std::cout << _mana_count << std::endl;
#pragma endregion

#pragma region deck
#pragma region reload
	if (_deck->is_reloading()) {
		int reload_time = _deck->reload_time();
		int time_till_reload_finishes = _deck->time_till_reload_finishes();
		SDL_SetRenderDrawBlendMode(sdlutils().renderer(), SDL_BLENDMODE_NONE);

		//bg
		SDL_SetRenderDrawColor(sdlutils().renderer(), 100, 100, 100, 255);
		rect_f32 baroutput1{ {_tr->getPos().getX() - 0.35f, _tr->getPos().getY() + 1.2f}, {(reload_time) / 1000.0f, 0.2 } };
		SDL_Rect trueoutput1 = SDL_Rect_screen_rect_from_global(baroutput1, _camera->cam);
		SDL_RenderFillRect(sdlutils().renderer(), &trueoutput1);


		//progress
		SDL_SetRenderDrawColor(sdlutils().renderer(), 220, 220, 220, 255);
		rect_f32 baroutput2{ {_tr->getPos().getX() - 0.35f, _tr->getPos().getY() + 1.2f}, {(reload_time - time_till_reload_finishes) / 1000.0f, 0.2 } };
		SDL_Rect trueoutput2 = SDL_Rect_screen_rect_from_global(baroutput2, _camera->cam);
		SDL_RenderFillRect(sdlutils().renderer(), &trueoutput2);
	}
#pragma endregion

#pragma region cards
	AnimationVars av = _deck->animation_vars();
#pragma region camera_definition
	camera_screen cam_screen = camera_screen();
	std::pair<int, int> position = { sdlutils().width(), sdlutils().height() };
	//camera position, similar to aspect ratio but in world units (suppose player is 1 world unit, how many players will fit on camera kinda)
	cam_screen.camera = { {0,0},{8,6} };
	//camera screen on pixels size
	cam_screen.screen = { position.first, position.second };
#pragma endregion

#pragma region hand_card
	//Mostrar carta en la mano
	//Mostrar n? cartas draw_pile and discard_pile
	card_rendering_descriptor crd = card_rendering_descriptor();
	//Position and scale for the cost --> both values from 0 to 1

	//Funci�n que calcula la posici�n de una carta seg�n el tiempo
	float percentual_time_to_card_in_position = (sdlutils().virtualTimer().currTime() - av._last_card_draw_time) / (float)av._card_draw_anim_duration;

	crd.mana_cost_font_key = "ARIAL16";
	crd.mana_cost_color = { 81, 100, 222, 255 };
	crd.health_cost_color = { 200, 80, 100, 255 };
	if (_deck->hand() == nullptr) {
		crd.card_image_key = "card_reloading";
		crd.mana_cost_subrect = { {0,0.2},{0,0} };
	}
	else {
		crd.mana_cost_subrect = { {0.1,0.2},{0.3,0.3} };
		crd.card_image_key = percentual_time_to_card_in_position < 0.5f ? "card_back" : _deck->hand()->get_name().data();
		crd.mana_cost = _deck->hand()->get_costs().get_mana();
		crd.health_cost = _deck->hand()->get_costs().get_health();
		crd.health_cost_subrect = { {0.55,0.2},{0.3,0.3} };
	}

	//Function for rendering a card
	card_rendering_descriptor_render(
		crd,
		cam_screen,
		//take renderer
		*sdlutils().renderer(),
		//destination rect --> where will the card be placed (position, size in world units)
		{ {(float)std::lerp(-6,-8, std::min(percentual_time_to_card_in_position,1.0f)),-3.5f},{(percentual_time_to_card_in_position < 0.5f) ? (float)std::lerp(2,0,std::min(percentual_time_to_card_in_position * 2,1.0f)) : (float)std::lerp(0,2,std::min((percentual_time_to_card_in_position - 0.5f) * 2,1.0f)),2.5f} },
		//src subrect --> if our image is only 1 take this parameters
		//if we have a map of 5x6 cards and we wanted to render card (3,2) being first card(0,0), and last (4,5)
		//values would be --> { {3/5, 2/6}, {1/5,1/6} }
		{ {0,0},{1,1} },
		//rotation
		0,
		//adittional options
		//card_rendering_descriptor_options_none,
		//card_rendering_descriptor_options_flip_horizontal,
		//card_rendering_descriptor_options_flip_vertical,
		//card_rendering_descriptor_options_full_subrect
		card_rendering_descriptor_options_none
	);
#pragma endregion

#pragma region milled_card
	if (_deck->last_milled_card() != nullptr) {
		//Funci�n que calcula la posici�n de una carta seg�n el tiempo
		percentual_time_to_card_in_position = (sdlutils().virtualTimer().currTime() - av._last_milled_card_time) / (float)av._mill_card_anim_duration;

		float scale = std::lerp(1.0f, 0.0f, std::max(0.0f, std::min((percentual_time_to_card_in_position - 0.75f) / 0.25f, 1.0f)));

		crd.mana_cost_font_key = "ARIAL16";
		crd.mana_cost_subrect = { {0.0f,0.4f - 0.2f * scale},{0.4f,0.4f * scale} };
		crd.health_cost_subrect = { {0.0f,0.7f - 0.2f * scale},{0.4f,0.4f * scale} };
		crd.mana_cost_color = { 81, 100, 222, 255 };
		crd.health_cost_color = { 200, 80, 100, 255 };
		crd.card_image_key = _deck->last_milled_card()->get_name().data();
		crd.mana_cost = _deck->last_milled_card()->get_costs().get_mana();

		//Function for rendering a card
		card_rendering_descriptor_render(
			crd,
			cam_screen,
			//take renderer
			*sdlutils().renderer(),
			//destination rect --> where will the card be placed (position, size in world units)
			{ {-6,std::lerp(-3.0f,-2.5f,percentual_time_to_card_in_position)},{2,2.5f * scale} },
			//src subrect --> if our image is only 1 take this parameters
			//if we have a map of 5x6 cards and we wanted to render card (3,2) being first card(0,0), and last (4,5)
			//values would be --> { {3/5, 2/6}, {1/5,1/6} }
			{ {0,0},{1,scale} },
			//rotation
			0,
			//adittional options
			//card_rendering_descriptor_options_none,
			//card_rendering_descriptor_options_flip_horizontal,
			//card_rendering_descriptor_options_flip_vertical,
			//card_rendering_descriptor_options_full_subrect
			card_rendering_descriptor_options_none
		);
	}
#pragma endregion
#pragma endregion

#pragma region prime
	if (_deck->get_primed()) {
		rect_f32 prime = rect_f32_screen_rect_from_viewport(rect_f32({0.013,0.805 }, { 0.1,0.1 }), _camera->cam.screen);
		SDL_Rect primetrue{ prime.position.x,prime.position.y,prime.size.x,prime.size.x };
		_tex_prime->render(primetrue);
	}
#pragma endregion
#pragma endregion
#pragma region timer
	//auto ent = Game::Instance()->get_mngr()->getHandler(ecs::hdlr::WAVE);
	//WaveManager* wavemanager = Game::Instance()->get_mngr()->getComponent<WaveManager>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::WAVE));
	/*if (Game::Instance()->get_mngr()->hasComponent<WaveManager>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::FOGGROUP))) {
		int wavetime = Game::Instance()->get_mngr()->getComponent<WaveManager>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::FOGGROUP))->get_wave_time();
		rect_f32 timer = rect_f32_screen_rect_from_viewport(rect_f32({ 0.4,0.05 }, { 0.2,0.1 }), _camera->cam.screen);
		SDL_Rect timertrue{ timer.position.x,timer.position.y,timer.size.x,timer.size.y };
		Texture timertex{ sdlutils().renderer(),std::to_string(wavetime),sdlutils().fonts().at("ARIAL16"),{50,50,50,255} };
		timertex.render(timertrue);
	}*/
	rect_f32 timer = rect_f32_screen_rect_from_viewport(rect_f32({ 0.4,0.01 }, { 0.2,0.15 }), _camera->cam.screen);
	SDL_Rect timertrue{ timer.position.x,timer.position.y,timer.size.x,timer.size.y };
	Texture timertex{ sdlutils().renderer(),std::to_string(60),sdlutils().fonts().at("ARIAL16"),{50,50,50,255} };
	timertex.render(timertrue);
#pragma endregion

}
