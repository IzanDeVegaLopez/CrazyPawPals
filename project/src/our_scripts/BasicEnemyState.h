// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

class BasicEnemy;

class BasicEnemyState {
public:
	virtual ~BasicEnemyState() {
	}
	virtual void enter(BasicEnemy &o) = 0;
	virtual void handleInput(BasicEnemy &o) = 0;
	virtual void update(BasicEnemy &o) = 0;
};

