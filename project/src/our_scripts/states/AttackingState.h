#include "BasicEnemyState.h"

class AttackingState : public BasicEnemyState
{
public:
	AttackingState() {}

	virtual void enter(BasicEnemy& _basicEnemy) {
		_basicEnemy.tr->setDir(Vector2D(0, 0));
	}

	virtual void handleInput(){}

	virtual void update(BasicEnemy& _basicEnemy) {
		if (/*Far enough to the target*/) {
			_basicEnemy.setState(BasicEnemy::WALKING);
		}

		if (/*Life <= 0*/) {
			_basicEnemy.setState(BasicEnemy::DYING);
		}
	}
private:
};