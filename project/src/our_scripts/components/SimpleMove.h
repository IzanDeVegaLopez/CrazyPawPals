// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../../ecs/Component.h"

class SimpleMove: public ecs::Component {
public:
	SimpleMove();
	virtual ~SimpleMove();
	void update() override;
};

