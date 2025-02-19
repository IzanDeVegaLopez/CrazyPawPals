#pragma once
#include <vector>
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../ecs/Component.h"

class Scene {
public:
	Scene();
	virtual ~Scene();
	//Each scene
	virtual void initScene() = 0;
	virtual void enterScene() = 0;
	virtual void exitScene() = 0;

	virtual void update() = 0;
	virtual void render() = 0;
	//Por ahora, hacemos spawnEntity (la que sea) en cada escena
	//ecs::Entity* spawn(std::vector<ecs::Component> components);

};