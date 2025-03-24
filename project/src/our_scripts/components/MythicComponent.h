#pragma once
#include <vector>
#include "../../ecs/Component.h"

class MythicItem;
class ManaComponent;
class MovementController;
class Health;
class Weapon;

class MythicComponent: public ecs::Component {
private:
	std::vector<MythicItem*> _obtained_mythics;
    ManaComponent* _mana;
    Health* _player_health;
    MovementController* _player_movement_controller;
    Weapon* _weapon;
public:
    __CMPID_DECL__(ecs::cmp::MYTHICCOMPONENT);
    MythicComponent();
    ~MythicComponent();
    void initComponent() override;
    void add_mythic(MythicItem* mythic);
    void update(uint32_t dt) override;
    inline const std::vector<MythicItem*>& get_mythics() const { return _obtained_mythics; }
};
