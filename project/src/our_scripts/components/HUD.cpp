#include "HUD.h"
#include "../../game/Game.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../rendering/card_rendering.hpp"
HUD::HUD(): _tex_orb(&sdlutils().images().at("manaorb")), _tex_orb_empty(&sdlutils().images().at("manaorbempty")), _tex_prime(&sdlutils().images().at("prime"))
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

	//bg
	SDL_SetRenderDrawColor(sdlutils().renderer(), 80, 55, 60, 255);
	SDL_Rect health1{ 200,sdlutils().height()-125,max_health * 2,20};
	SDL_RenderFillRect(sdlutils().renderer(), &health1);

	//remaining
	SDL_SetRenderDrawColor(sdlutils().renderer(), 200, 80, 100, 255);
	SDL_Rect health2{ 200,sdlutils().height()-125,health * 2,20 };
	SDL_RenderFillRect(sdlutils().renderer(), &health2);

#pragma endregion

#pragma region mana
	int mana_count = _mana->raw_mana_count();
	int max_mana = _mana->max_mana();

	//bg
	SDL_SetRenderDrawColor(sdlutils().renderer(), 60, 55, 80, 255);
	SDL_Rect mana1{ 200,sdlutils().height() - 95,200,16 };
	SDL_RenderFillRect(sdlutils().renderer(), &mana1);

	//progress
	SDL_SetRenderDrawColor(sdlutils().renderer(), 81, 100, 222, 255);
	SDL_Rect mana2{ 200,sdlutils().height() - 95,(mana_count % 1000) / 5,16 };
	SDL_RenderFillRect(sdlutils().renderer(), &mana2);

	// full mana orbs
	int display = max_mana / 1000;
	for (int i = 1; i <= display; i++) {
		SDL_Rect output{ 42 * (i - 1) + 200,sdlutils().height() - 69,32,32 };
		if (mana_count >= i * 1000) _tex_orb->render(output);
		else _tex_orb_empty->render(output);
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
		rect_f32 baroutput2{ {_tr->getPos().getX() - 0.35f, _tr->getPos().getY() + 1.2f}, {(reload_time -time_till_reload_finishes) / 1000.0f, 0.2 } };
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
	//Mostrar n� cartas draw_pile and discard_pile
	card_rendering_descriptor crd = card_rendering_descriptor();
	//Position and scale for the cost --> both values from 0 to 1

	//Función que calcula la posición de una carta según el tiempo
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
		//Función que calcula la posición de una carta según el tiempo
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
		SDL_Rect primerect{ 10,71,31,32 };
		_tex_prime->render(primerect);
	}
#pragma endregion
#pragma endregion


}
