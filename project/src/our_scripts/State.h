// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

class State {
public:
	virtual ~State() {
	}
	virtual void enter() = 0;
	virtual void update() = 0;
	virtual void exit() = 0;
};

