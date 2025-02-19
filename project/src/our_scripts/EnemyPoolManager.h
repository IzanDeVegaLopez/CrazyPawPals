// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"
#include "ObjectPool.h"

class Texture;
class Enemy;

class EnemyPoolManager : public ecs::Component {
	

public:
	EnemyPoolManager();
	virtual ~EnemyPoolManager();
	void update(uint32_t delta_time) override;
	void render() override;
private:

	void addEnemies(uint16_t);

	//ObjectPool<Enemy> _pool;
	Texture *_img;
};

