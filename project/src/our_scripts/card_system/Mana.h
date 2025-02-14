#pragma once
#include <algorithm>
#include "../../ecs/Component.h" 
class Mana:public ecs::Component {
private:
	int _max_mana; // Maximum amount of mana that can be stored.
	int _mana_regen; // Mana gained per second.
	int _mana_count; // Current amount of mana.
public:
	__CMPID_DECL__(ecs::cmp::MANA);
	Mana(int max = 5000, int regen = 750) : _max_mana(max), _mana_regen(regen), _mana_count(0) {}
	void update(int delta_time) override { // Add override after implementing components 
		_mana_count = std::min(_mana_count + _mana_regen*delta_time/1000, _max_mana);
	}
	
	// Changes current mana by a given amount. Can be positive or negative, and cannot be lower than 0 or greater than _max_mana.
	void change_mana(int delta) { _mana_count = std::max(0, std::min(_mana_count + delta, _max_mana)); }
	// Changes mana regeneration by a given amount. Can be positive or negative.
	void change_mana_regen(int delta) { _mana_regen += delta; }
	// Changes maximum mana by a given amount. Can be positive or negative.
	void change_max_mana(int delta) { _max_mana += delta * 1000; }
	// Returns your current mana.
	int mana_count() const { return _mana_count; }

};
