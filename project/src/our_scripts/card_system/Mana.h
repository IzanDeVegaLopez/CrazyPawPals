#pragma once
#include <algorithm>
class Mana {
private:
	int _mana_count = 0; // Current amount of mana.
	int _max_mana; // Maximum amount of mana that can be stored.
	int _mana_regen; // Mana gained per second.
public:
	Mana(int max = 5000, int regen = 750) : _max_mana(max), _mana_regen(regen) {}
	void update(int deltaTime) { // Add override after implementing components 
		_mana_count = std::min(_mana_count + _mana_regen*deltaTime/1000, _max_mana);
	}
	bool spend_mana(int tax) {
		if (tax <= _mana_count) {
			_mana_count -= tax;
			return true;
		}
		return false;
	}
	void add_mana(int gain) { _mana_count += gain; }
	void add_mana_regen(int gain) { _mana_regen += gain; }
	void add_max_mana(int gain) { _max_mana += gain * 1000; }

};
