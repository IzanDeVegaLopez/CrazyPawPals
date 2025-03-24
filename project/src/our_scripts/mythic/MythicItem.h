#pragma once
#include <string>
#include "../components/cards/Mana.h"
#include "../components/Health.h"
#include "../components/movement/MovementController.h"
#include "../components/weapons/Weapon.h"
#include "../components/cards/Deck.hpp"

class MythicItem {
protected:
	std::string _name;
	Texture* _texture;
	
public:
	MythicItem(std::string name = "default",std::string texture_name = "mythic")
		:_name(name), _texture(&sdlutils().images().at(texture_name)) {};
	virtual ~MythicItem() = default;

	virtual void apply_effects() = 0;

	virtual void update(uint32_t dt) {(void)dt;};
	virtual std::string get_name() const { return _name; };
	virtual Texture* get_texture() const { return _texture; };
};
