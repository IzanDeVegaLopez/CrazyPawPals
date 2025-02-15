
#include "../game/GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy(ecs::Manager* mngr);
	virtual ~Enemy() {};
private:
	Transform* _tr;
};


