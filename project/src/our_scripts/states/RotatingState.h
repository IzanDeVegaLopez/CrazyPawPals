#include "State.h"
#include "../../utils/Vector2D.h"

class MovementController;
class Transform;
class RotatingState : public State
{
public:
	RotatingState(Transform* tr, Transform* playerTr, MovementController* mc);
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;

protected:
	Transform* _tr;
	MovementController* _movementController;
	Transform* _playerTr;
	float _angle;
	Vector2D _center;
	float _radius;
	const float _ANGULAR_SPEED = 2.0f;

};