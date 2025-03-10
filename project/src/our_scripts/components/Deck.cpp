#include "Deck.hpp"
#include "ecs/Manager.h"
#include <iostream>
#include "game/Game.h"
#include "../components/MovementController.h"
#include "../components/Transform.h"
#include "../../rendering/card_rendering.hpp"
#include <algorithm>

void Deck::_put_new_card_on_hand()
{
	if (!_draw_pile.empty()) {
		_hand = _draw_pile.pop_first();
		_av._last_card_draw_time = sdlutils().virtualTimer().currTime();
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
		_health->payHealth(_hand->get_costs().get_health());
		_hand->on_play(*this, &_tr->getPos(), target_pos);
		switch (_hand->get_play_destination()) {
		case DISCARD_PILE:
			_discard_pile.add_card(std::move(_hand));
			break;
		case DRAW_PILE:
			_draw_pile.add_card(std::move(_hand));
			break;
		}
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
		switch (_hand->get_discard_destination()) {
		case DISCARD_PILE:
			_discard_pile.add_card(std::move(_hand));
			break;
		case DRAW_PILE:
			_draw_pile.add_card(std::move(_hand));
			break;
		}
		_put_new_card_on_hand();
		return true;
	}
	else {
		return false;
	}
}

std::pair<bool, Card*> Deck::mill() noexcept
{		
	bool milled = false;
	if (!_draw_pile.empty()) {
		milled = true;
		_last_milled_card = _draw_pile.pop_first()->on_mill(*this, &_tr->getPos());
		switch (_last_milled_card->get_mill_destination()) {
		case DISCARD_PILE:
			_discard_pile.add_card(_last_milled_card);
			break;
		case DRAW_PILE:
			_draw_pile.add_card(_last_milled_card);
			break;
		}
		_av._last_milled_card_time = sdlutils().virtualTimer().currTime();
		Game::Instance()->get_event_mngr()->fire_event(event_system::mill, event_system::event_receiver::Msg());
	}
	return std::make_pair(milled, _last_milled_card);
}

void Deck::reload() noexcept
{
	if (!_is_reloading) {
		//TODO -> block player action
		_is_reloading = true;
		_time_till_reload_finishes = _reload_time;
		event_system::event_manager::Instance()->fire_event(event_system::shuffle, event_system::event_receiver::Msg());
		//Puts all cards on discard
		if (_hand != nullptr) {
			_discard_pile.add_card(std::move(_hand));
			_hand = nullptr;
		}
		_draw_pile.move_from_this_to(_discard_pile);
		_primed = false;
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

void Deck::add_card_to_deck(Card* c)
{
	assert(c != nullptr);
	_draw_pile.add_card(std::move(c));
}

void Deck::add_card_to_discard_pile(Card* c)
{
	assert(c != nullptr);
	_discard_pile.add_card(std::move(c));
}

void Deck::remove_card(std::list<Card*>::iterator)
{
}

MovementController* Deck::get_movement_controller()
{
	return Game::Instance()->get_mngr()->getComponent<MovementController>(_ent); 
}

void Deck::set_primed(bool prime)
{
	_primed = prime;
}

void Deck::move_discard_to_draw() {
	_discard_pile.move_from_this_to(_draw_pile);
	_draw_pile.shuffle();
}

void Deck::initComponent()
{
	_mana = Game::Instance()->get_mngr()->getComponent<ManaComponent>(_ent);
	assert(_mana!=nullptr);
	_health = Game::Instance()->get_mngr()->getComponent<Health>(_ent);
	assert(_health!=nullptr);
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
