// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "State.h"

class Shoot3P : public State {
public:
	virtual ~Shoot3P() {
	}
	void enter() override;
	void update() override;
	void exit() override;
};

