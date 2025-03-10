#pragma once
#include "Scene.h"

class Texture;
class ImageForButton;
class SelectionMenuScene : public Scene
{
public:
	SelectionMenuScene();
	virtual ~SelectionMenuScene();
	void initScene() override;
	void enterScene() override;
	void exitScene() override;
	void render() override;

private:
	Texture* _selection;
	void create_weapon_button(GameStructs::WeaponType wt, const GameStructs::ButtonProperties& bp);
	void create_deck_button(GameStructs::DeckType dt,const GameStructs::ButtonProperties& bp);
	void create_weapon_buttons();
	void create_deck_buttons();
	inline void weapon_selected() { _weapon_selected = true; };
	inline void deck_selected() { _deck_selected = true; };
	//flags to control if both has been selected
	bool _weapon_selected;
	bool _deck_selected;

	ImageForButton* _last_weapon_button;
};