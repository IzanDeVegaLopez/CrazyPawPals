#include "BasicEnemyState.h"

class DyingState : public BasicEnemyState
{
public:
	DyingState() {}

	virtual void enter(BasicEnemy& _basicEnemy) {

	}

	virtual void handleInput() {}

	virtual void update(BasicEnemy& _basicEnemy) {

		if (/*Animation time over*/) {
			_basicEnemy.setState(BasicEnemy::INACTIVE);
		}
	}
private:
};