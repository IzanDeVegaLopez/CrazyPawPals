#include "State.h"

class Transform;
class MovementController;

class DashingState : public State {
protected:
	Transform* _tr;
	Transform* _playerTr;
	MovementController* _movementController;
	
	bool _to_destination;

	float _extra_space;
	uint32_t _time;
public:
	DashingState(Transform* tr, Transform* playerTr, MovementController* movementController, bool toDestination = false, uint32_t time = 1000, float extra_space = 1.8f);
	~DashingState() {};
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;
};