#include "State.h"

class Transform;
class MovementController;

class WalkingState : public State {
protected:
	Transform* _tr;
	Transform* _player_tr;
	MovementController* _movementController;
	
	bool _to_destination;

	void search_player_tr();
public:
	WalkingState(Transform* tr, MovementController* movementController, bool toDestination = false);
	~WalkingState() {};
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;
};