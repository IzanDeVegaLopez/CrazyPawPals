#include "BasicEnemyState.h"

class WalkingState : public BasicEnemyState
{
public:
	WalkingState(float s) : _speed(s) {}

	virtual void enter(BasicEnemy& _basicEnemy) {
		
	}

	virtual void handleInput() {}

	virtual void update(BasicEnemy& _basicEnemy) override {
		if (/*Close enough to the target*/) {
			_basicEnemy.setState(BasicEnemy::ATTACKING);
		}

		if (/*Life <= 0*/) {
			_basicEnemy.setState(BasicEnemy::DYING);
		}
	}
private:
	float _speed;
};