// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "ObjectPool.h"


class EnemyPoolManager {
	struct Enemy {
		float _width;
		float _height;
	};
public:

	EnemyPoolManager();

	virtual ~EnemyPoolManager();

	void update();
	void render();

	std::vector<Enemy*> getActiveObjects() const;
	void addEnemies(uint16_t);

private:
	ObjectPool<Enemy> _pool;
};

