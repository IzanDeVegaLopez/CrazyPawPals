#include "Container.h"

class Bullet: public Container
{
public:
	Bullet(Vector2D& pos, Vector2D& dir, float speed);
	~Bullet() {};

	void update() override;

	bool isActive() const;

private:
	bool _active;
	float _speed;
};

inline bool
Bullet::isActive() const {
	return _active;
}