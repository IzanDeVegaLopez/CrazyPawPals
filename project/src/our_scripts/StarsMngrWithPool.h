// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"
#include "ObjectPool.h"

class Texture;

class StarsMngrWithPool {
	struct Star {
//		Vector2D _pos;
		float _width;
		float _height;
	};

public:
	StarsMngrWithPool();
	virtual ~StarsMngrWithPool();
	/*void update() override;
	void render() override;
	void handleInput() override;*/
private:

	void addStarts(uint16_t);

	ObjectPool<Star> _pool;
	Texture *_img;
};

