#include "State.h"

class Transform;
class MovementController;

class WalkingState : public State {
protected:
	Transform* _tr;
	Transform* _playerTr;
	MovementController* _movementController;
	
	bool _to_destination;
public:
	WalkingState(Transform* tr, Transform* playerTr, MovementController* movementController, bool toDestination = false);
	~WalkingState() {};
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;
};