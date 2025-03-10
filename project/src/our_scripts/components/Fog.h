#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/SDLUtils.h"

class Transform;
class Texture;

class Fog : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::FOG);

	Fog() {};
	virtual ~Fog() {};
	void initComponent() override;
	void update(uint32_t delta_time) override;

	bool getFogActive() { return fogActive; }
	void setFog(bool fogActive_) { fogActive = fogActive_; }

private:
	Transform* _playerTr;
	Texture* _fogTex;
	bool fogActive = false;
	float fogPercentage = 0;

	SDL_Rect fogRight;
	SDL_Rect fogLeft;
	SDL_Rect fogUp;
	SDL_Rect fogDown;

	float fogRightEnd;
	float fogLeftEnd;
	float fogUpEnd;
	float fogDownEnd;

	float fogRightOrigin;
	float fogLeftOrigin;
	float fogUpOrigin;
	float fogDownOrigin;
};