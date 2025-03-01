#include "Deck.hpp"
#include "ecs/Manager.h"
#include <iostream>
#include "game/Game.h"
#include "../components/Transform.h"
#include "../../rendering/card_rendering.hpp"
#include <algorithm>

void Deck::_put_new_card_on_hand()
{
	if (_hand != nullptr) {
		_discard_pile.add_card(std::move(_hand));
	}
	if (!_draw_pile.empty()) {
		_hand = _draw_pile.pop_first();
		_last_card_draw_time = sdlutils().virtualTimer().currTime();
	}
	else {
		_hand = nullptr;
		reload();
	}
}

Deck::Deck(std::list<Card*>& starterDeck) noexcept
{
	_discard_pile = CardList();
	_hand = nullptr;
	//_mana = new Mana(); // REMOVE AFTER IMPLEMENTING PLAYER
	_draw_pile = CardList(starterDeck);
	_draw_pile.shuffle();
	_put_new_card_on_hand();
}

Deck::Deck(CardList&& starterDeck) noexcept
{
	_discard_pile = CardList();
	_hand = nullptr;
	//_mana = new Mana(); // REMOVE AFTER IMPLEMENTING PLAYER
	_draw_pile = starterDeck;
	_draw_pile.shuffle();
	_put_new_card_on_hand();
}

Deck::~Deck()
{
	//Hand es un puntero a una carta
	if(_hand!=nullptr)
		delete _hand;
	//_draw_pile y _discard_pile llamar�n a su destructor cuando esto se destruya al salir de �mbito
}

bool Deck::use_card(const Vector2D* target_pos) noexcept
{
	if (_can_play_hand_card()) {
		//Se pudo usar la carta
		_mana->change_mana(-_hand->get_costs().get_mana());
		_hand->on_play(*this, &_tr->getPos(), target_pos);
		_put_new_card_on_hand();
		return true;
	}
	else {
		//No se pudo usar la carta, dar indicaci�n visual y auditiva al player
		return false;
	}
}

bool Deck::discard_card() noexcept
{
	if (_hand != nullptr) {
		_put_new_card_on_hand();
		return true;
	}
	else {
		return false;
	}
}

void Deck::mill() noexcept
{
	if (!_draw_pile.empty()) {
		_last_milled_card = _discard_pile.add_card(_draw_pile.pop_first()->on_mill());
		_last_milled_card_time = sdlutils().virtualTimer().currTime();
	}
}

void Deck::reload() noexcept
{
	if (!_is_reloading) {
		//TODO -> block player action
		_is_reloading = true;
		_time_till_reload_finishes = reload_time;

		//Puts all cards on discard
		if (_hand != nullptr) {
			_discard_pile.add_card(std::move(_hand));
			_hand = nullptr;
		}
		_draw_pile.move_from_this_to(_discard_pile);
	}
}
void Deck::_finish_realoading()
{
	//this is called after waiting
	//this moves everything to draw_pile, shuffles and return control to player
	_is_reloading = false;
	_discard_pile.move_from_this_to(_draw_pile);
	_draw_pile.shuffle();
	_put_new_card_on_hand();
	//std::cout << *this;
}
bool Deck::_can_finish_reloading()
{
	return _is_reloading && _time_till_reload_finishes <= 0;
}

bool Deck::_can_play_hand_card()
{
	return (!_is_reloading && _mana->mana_count() >= _hand->get_costs().get_mana());
	//TODO: card checks mana and life costs
	
}

void Deck::update(Uint32 deltaTime) noexcept
{
	//TODO
	//Counts time down for reload time and do the rest of things needed for finishing reload

	_draw_pile.update(deltaTime);
	if(_hand != nullptr)
		_hand->update(deltaTime);
	_discard_pile.update(deltaTime);

	_time_till_reload_finishes -= deltaTime;
	//if(_is_reloading)
		//std::cout << _time_till_reload_finishes << std::endl;
	//std::cout << _time_till_reload_finishes << std::endl;
	if (_can_finish_reloading()) {
		_finish_realoading();
	}
}

void Deck::render() noexcept
{
#pragma region reload_bar
		//reload bar
		if (_is_reloading) {
			SDL_SetRenderDrawBlendMode(sdlutils().renderer(), SDL_BLENDMODE_NONE);

			//bg
			SDL_SetRenderDrawColor(sdlutils().renderer(), 100, 100, 100, 255);
			rect_f32 baroutput1{ {_tr->getPos().getX(), _tr->getPos().getY() + 0.3f}, {(reload_time) / 1000.0f, 0.2 } };
			SDL_Rect trueoutput1 = SDL_Rect_screen_rect_from_global(baroutput1, _camera->cam);
			SDL_RenderFillRect(sdlutils().renderer(), &trueoutput1);


			//progress
			SDL_SetRenderDrawColor(sdlutils().renderer(), 220, 220, 220, 255);
			rect_f32 baroutput2{ {_tr->getPos().getX(), _tr->getPos().getY() + 0.3f}, {(reload_time - _time_till_reload_finishes) / 1000.0f, 0.2 } };
			SDL_Rect trueoutput2 = SDL_Rect_screen_rect_from_global(baroutput2, _camera->cam);
			SDL_RenderFillRect(sdlutils().renderer(), &trueoutput2);
		}
#pragma endregion

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
		float percentual_time_to_card_in_position = (sdlutils().virtualTimer().currTime() - _last_card_draw_time) / (float)_card_draw_anim_duration;
		
		crd.mana_cost_font_key = "ARIAL16";
		crd.mana_cost_color = { 255,0,0,255 };
		if (_hand == nullptr) {
			crd.card_image_key = "card_reloading";
			crd.mana_cost_subrect = { {0,0.2},{0,0} };
		}
		else {
			crd.mana_cost_subrect = { {0,0.2},{0.4,0.4} };
			crd.card_image_key = percentual_time_to_card_in_position < 0.5f ? "card_back" : _hand->get_name().data();
			crd.mana_cost = _hand->get_costs().get_mana();
		}

		//Function for rendering a card
		card_rendering_descriptor_render(
			crd,
			cam_screen,
			//take renderer
			*sdlutils().renderer(),
			//destination rect --> where will the card be placed (position, size in world units)
			{ {(float)std::lerp(-6,-8, std::min(percentual_time_to_card_in_position,1.0f)),-4},{(percentual_time_to_card_in_position < 0.5f) ? (float)std::lerp(2,0,std::min(percentual_time_to_card_in_position*2,1.0f)) : (float)std::lerp(0,2,std::min((percentual_time_to_card_in_position-0.5f)*2,1.0f)),2}},
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
		if (_last_milled_card != nullptr) {
			//Función que calcula la posición de una carta según el tiempo
			percentual_time_to_card_in_position = (sdlutils().virtualTimer().currTime() - _last_milled_card_time) / (float)_mill_card_anim_duration;

			float scale = std::lerp(1.0f, 0.0f, std::max(0.0f, std::min((percentual_time_to_card_in_position - 0.75f) / 0.25f, 1.0f)));

			crd.mana_cost_font_key = "ARIAL16";
			crd.mana_cost_color = { 255,0,0,255 };
			crd.mana_cost_subrect = { {0.0f,0.4f-0.2f * scale},{0.4f,0.4f * scale} };
			crd.card_image_key = _last_milled_card->get_name().data();
			crd.mana_cost = _last_milled_card->get_costs().get_mana();

			//Function for rendering a card
			card_rendering_descriptor_render(
				crd,
				cam_screen,
				//take renderer
				*sdlutils().renderer(),
				//destination rect --> where will the card be placed (position, size in world units)
				{ {-6,std::lerp(-3.0f,-2.5f,percentual_time_to_card_in_position)},{2,2*scale} },
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
		
}

void Deck::add_card_to_deck(Card* c)
{
	assert(c != nullptr);
	_draw_pile.add_card(std::move(c));
}

void Deck::remove_card(std::list<Card*>::iterator)
{
}

void Deck::initComponent()
{
	_mana = Game::Instance()->get_mngr()->getComponent<ManaComponent>(_ent);
	assert(_mana!=nullptr);
	_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	assert(_tr!=nullptr);
	_camera = Game::Instance()->get_mngr()->getComponent<camera_component>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::CAMERA));
	assert(_camera != nullptr);
}

std::ostream& operator<<(std::ostream& os, const Deck& deck)
{
	os << "Draw Pile: " << std::endl;
	os << deck._draw_pile;
	os << std::endl;

	if(deck._hand!=nullptr)
		os << "Hand: "  << std::endl << deck._hand->get_written_info() << std::endl;

	os << "DiscardPile: " << std::endl;
	os << deck._discard_pile;
	os<<std::endl;
	
	return os;
}
