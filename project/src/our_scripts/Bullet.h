
#include "../game/GameObject.h"

class Bullet: public GameObject
{
public:
	Bullet(ecs::Manager* mngr,Vector2D& pos, Vector2D& dir, Vector2D& prevDir, float speed, float lifeTime = 2.0f);
	virtual ~Bullet() {};
private:
	Transform* _tr;
};


