#pragma once
#include <algorithm>
#include "../../../ecs/Component.h" 
#include "../../../sdlutils/Texture.h"
#include "SDL.h"
#include "SDL_image.h"
#include "../../../sdlutils/SDLUtils.h"
class ManaComponent:public ecs::Component {
private:
	int _max_mana; // Maximum amount of mana that can be stored.
	int _mana_regen; // Mana gained per second.
	int _mana_count; // Current amount of mana.
public:
	__CMPID_DECL__(ecs::cmp::MANA);
	ManaComponent(int max = 5000, int regen = 750) : _max_mana(max), _mana_regen(regen), _mana_count(0) {}
	void initComponent() override { _mana_count = 0; }
	void update(Uint32 delta_time) override {
		_mana_count = std::min(_mana_count + int(_mana_regen*delta_time)/1000, _max_mana);
		
	}
	/*void render() override {
		SDL_SetRenderDrawBlendMode(sdlutils().renderer(), SDL_BLENDMODE_NONE);

		//bg
		SDL_SetRenderDrawColor(sdlutils().renderer(), 222, 81, 100, 255);
		SDL_Rect progress1{ 10,45,200,16 };
		SDL_RenderFillRect(sdlutils().renderer(), &progress1);

		//progress
		SDL_SetRenderDrawColor(sdlutils().renderer(), 81, 100, 222, 255);
		SDL_Rect progress2{ 10,45,(_mana_count % 1000) / 5,16 };
		SDL_RenderFillRect(sdlutils().renderer(), &progress2);

		// full mana orbs
		int display = _max_mana / 1000;
		for (int i = 1; i <= display; i++) {
			SDL_Rect output { 42 * (i-1) + 10,10,32,32 };
			if (_mana_count >= i * 1000) _tex_orb->render(output);
			else _tex_orb_empty->render(output);
		}

		
	}*/
	
	// Changes current mana by a given amount. Can be positive or negative, and cannot be lower than 0 or greater than _max_mana.
	void change_mana(int delta) { _mana_count = std::max(0, std::min(_mana_count + (delta*1000), _max_mana)); }
	// Changes mana regeneration by a given amount. Can be positive or negative.
	void change_mana_regen(int delta) { _mana_regen += delta; }
	// Changes maximum mana by a given amount. Can be positive or negative.
	void change_max_mana(int delta) { _max_mana += delta * 1000; }
	// Returns your current mana.
	int mana_count() const { return _mana_count/1000; }
	int mana_regen() const { return  _mana_regen; }

	inline int raw_mana_count() const { return _mana_count; }
	inline int max_mana() const { return _max_mana; }

};
