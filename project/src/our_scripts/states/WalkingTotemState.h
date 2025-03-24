#include "State.h"

class Transform;
class MovementController;

class WalkingTotemState : public State {
protected:
	Transform* _tr;
	MovementController* _movementController;
	Vector2D _targetPos;
	
	bool _to_destination;
public:
	WalkingTotemState(Transform* tr, MovementController* movementController, bool toDestination = false);
	~WalkingTotemState() {};
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;
};