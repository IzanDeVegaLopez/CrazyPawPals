#pragma once
#pragma once
#include "../../../ecs/Component.h"
#include "../cards/Mana.h"
#include "../Health.h"
#include "../cards/Deck.hpp"
#include "../movement/Transform.h"
#include "../rendering/camera_component.hpp"
#include "../WaveManager.h"
class HUD : public ecs::Component {
private:
	WaveManager* _wm;
	const camera_component* _camera;
public:
	__CMPID_DECL__(ecs::cmp::PLAYER_HUD);
	HUD();
	virtual ~HUD();
	void initComponent() override;
	void update(uint32_t delta_time) override;
	void render() override;
};