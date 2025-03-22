#pragma once

#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"
#include "../../../utils/EventsSystem.hpp"

class Transform;
class MovementController : public event_system::event_receiver, public ecs::Component
{
private:
	float _max_speed = 0.1f;
	float _acceleration = 3.5f;
	float _decceleration = 1.0f;
	bool _dashing;
	Vector2D _input;
	Vector2D _dash_pos;
	uint32_t _time_remaining;
	Transform* _tr;

public:
	__CMPID_DECL__(ecs::cmp::MOVEMENTCONTROLLER);
	MovementController(float max_speed = 0.1f, float acceleration = 5.0f, float decceleration = 20.0f);
	virtual ~MovementController();
	void initComponent() override;
	void update(uint32_t delta_time) override;
	void accelerate();
	void dash(Vector2D next_pos, uint32_t time);
	void set_input(Vector2D);
	void event_callback0(const event_system::event_receiver::Msg&) override;

	float& get_max_speed() { return _max_speed; }
	float& get_acceleration() { return _acceleration; }


};