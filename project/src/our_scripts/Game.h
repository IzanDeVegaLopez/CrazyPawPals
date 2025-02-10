// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>
#include "GameObject.h"


// when you are not using the methods of a class, just
// say that it exists, that saves time when parsing files
class Container;
class GameManager;

class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();
private:
	void checkCollisions();

	GameManager *_gm;
	//Container *_leftPaddle;
	//Container *_rightPaddle;
	//Container *_ball;
	Container* _enemy;
	std::vector<GameObject*> _objs;
};

