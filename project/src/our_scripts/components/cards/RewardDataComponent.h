#pragma once
#include "../../../ecs/Component.h"
#include "../../../game/GameStructs.h"
#include <string>

struct RewardDataComponent : public ecs::Component{
public:
    __CMPID_DECL__(ecs::cmp::REWARDDATACOMPONENT);
    RewardDataComponent();
    RewardDataComponent(const std::string& s);
    virtual ~RewardDataComponent();
    void set_data(const std::string& s, GameStructs::CardType ct);
    const GameStructs::CardType& CT() { return _ct; };
protected:
    GameStructs::CardType _ct;
    std::string _data;
};