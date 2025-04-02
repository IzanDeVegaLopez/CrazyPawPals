#pragma once
#include "../../ecs/Component.h"
#include <string>

struct RewardDataComponent : public ecs::Component{
public:
    __CMPID_DECL__(ecs::cmp::REWARDDATACOMPONENT);
    std::string _data;
    RewardDataComponent() : _data("") {};
    RewardDataComponent(std::string s) : _data(s) {};
    virtual ~RewardDataComponent() {};
};