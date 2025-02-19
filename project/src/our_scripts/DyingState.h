#include "BasicEnemyState.h"

class InactiveState : public BasicEnemyState
{
public:
	InactiveState() {}

	virtual void enter(BasicEnemy& _basicEnemy) {

	}

	virtual void handleInput() {}

	virtual void update(BasicEnemy& _basicEnemy) {

		if (/*Out of the pool*/) {
			_basicEnemy.setState(BasicEnemy::WALKING);
		}
	}
private:
};