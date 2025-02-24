#include "State.h"

class DyingState : public State
{
public:
	DyingState() {}

	virtual void enter(BasicEnemy& _basicEnemy) {

	}
	virtual void update(BasicEnemy& _basicEnemy) {

		if (/*Animation time over*/) {
			_basicEnemy.setState(BasicEnemy::INACTIVE);
		}
	}
private:
};