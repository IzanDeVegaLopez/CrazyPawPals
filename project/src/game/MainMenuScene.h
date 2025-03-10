#pragma once

#include "Scene.h"

class Texture;
class MainMenuScene : public Scene
{
public:
	MainMenuScene();
	~MainMenuScene();
	void initScene() override;
	void enterScene() override;
	void exitScene() override;
	void render() override;
private:
	Texture* _background;
	//Methods of entities to spawn
    void create_start_button(const GameStructs::ButtonProperties& bp);
	void create_controls_button(const GameStructs::ButtonProperties& bp);
	void create_exit_button(const GameStructs::ButtonProperties& bp);
};