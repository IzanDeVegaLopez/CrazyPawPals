// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../../ecs/Component.h"
class Transform;

class SimpleMove: public ecs::Component {
	Transform* _tr;
public:
	__CMPID_DECL__(ecs::cmp::SIMPLEMOVE);
	SimpleMove();
	virtual ~SimpleMove();
	void initComponent() override;
	void update(int delta_time) override;
};

