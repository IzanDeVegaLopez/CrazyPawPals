#pragma once
#include "../card_system/CardList.h"
#include "../card_system/Card.hpp"
#include "../card_system/PlayableCards.hpp"
#include "Mana.h"
#include "../../utils/Vector2D.h"
#include "../components/camera_component.hpp"
#include "ecs/Component.h"
#include <list>
#include <cassert>
class Transform;
class Deck: public ecs::Component {
protected:
	int reload_time = 1000;
	CardList _draw_pile;
	CardList _discard_pile;
	Card* _hand;
	ManaComponent* _mana;
	Transform* _tr;
	const camera_component* _camera;
	bool _is_reloading = false;
	int _time_till_reload_finishes;
	void _put_new_card_on_hand();
	void _finish_realoading();
	bool _can_finish_reloading();
	bool _can_play_hand_card();
public:
	__CMPID_DECL__(ecs::cmp::DECK)
	//Creates a starter with a list of cards
	Deck(CardList&& starterDeck) noexcept;
	Deck(std::list<Card*>&) noexcept;
	~Deck();
	//returns true if the card can be used, calls the useCard function of the card
	//and puts the top card of deck on hand, if there's non left it reloads
	bool use_card(const Vector2D* target_pos) noexcept;
	//puts the card on hand on discard pile
	//and puts the top card of deck on hand, if there's non left it reloads
	//returns true, if a card could be discarded
	bool discard_card() noexcept;
	//Puts the top card of draw pile on discard and fires its mill effect
	//If there's no cards left on deck this does nothing
	void mill() noexcept;
	//Puts all cards on discard pile and sets player unable to use any action outside moving
	//Then puts all cards on drawPile and shuffles
	void reload() noexcept;
	void update(uint32_t deltaTime) noexcept override;
	void render() noexcept override;
	friend std::ostream& operator << (std::ostream& os, const Deck& deck);

	void add_card_to_deck(Card*);
	//removeCard, must only be used during menu time
	//Whenever this happens the iterator passed to this function must
	//belong to _draw_pile (during rewards menu all cards are exclusively in
	//the draw pile)
	void remove_card(std::list<Card*>::iterator);

	void initComponent() override;
	/*
	class const_iterator {
	private:
		friend class Deck;
	};
	*/
};