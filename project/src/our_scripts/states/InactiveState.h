#include "State.h"

class InactiveState : public State
{
public:
	InactiveState() {}

	virtual void enter(BasicEnemy& _basicEnemy) {

	}

	virtual void update(BasicEnemy& _basicEnemy) {

		if (/*Out of the pool*/) {
			_basicEnemy.setState(BasicEnemy::WALKING);
		}
	}
private:
};