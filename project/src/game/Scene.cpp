#include "Scene.h"

Scene::Scene() : _mngrGame(nullptr)
{

}

Scene::~Scene()
{
	delete _mngrGame;
}
/*
Scene::spawn(std::vector<ecs::Component> components)
{
	auto e = _mngrGame->addEntity();
	for (int i = 0; i < components.size(); i++)
	{
		_mngrGame->addComponent<components[i]>(e)
	}
}*/