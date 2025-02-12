// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

class NinjaWS;

class NinjaState {
public:
	virtual ~NinjaState() {
	}
	virtual void enter(NinjaWS &o) = 0;
	virtual void handleInput(NinjaWS &o) = 0;
	virtual void update(NinjaWS &o) = 0;
};

