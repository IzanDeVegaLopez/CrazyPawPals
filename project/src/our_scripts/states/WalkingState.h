#include "State.h"
#include <functional>

class Transform;
class MovementController;

class WalkingState : public State {

public:
	using SearchNextFollow = std::function<void()>;

	WalkingState(Transform* tr, MovementController* movementController, bool toDestination = false);
	~WalkingState() {};
	void enter() override;
	void update(uint32_t delta_time) override;
	void exit() override;

protected:
	Transform* _tr;
	Transform* _player_tr;
	MovementController* _movementController;
	
	bool _to_destination;

	SearchNextFollow _on_search_next_follow;
};