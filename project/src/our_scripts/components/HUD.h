#pragma once
#include "../../ecs/Component.h"
#include "Mana.h"
#include "Health.h"
#include "Deck.hpp"
#include "Transform.h"
#include "camera_component.hpp"
class HUD : public ecs::Component {
private:
	ManaComponent* _mana;
	Health* _health;
	Deck* _deck;
	Transform* _tr;
	const camera_component* _camera;

	Texture* _tex_orb;
	Texture* _tex_orb_empty;
	Texture* _tex_prime;
public:
	__CMPID_DECL__(ecs::cmp::HUD);
	HUD();
	virtual ~HUD();
	void initComponent() override;
	void update(uint32_t delta_time) override;
	void render() override;
};