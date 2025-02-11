#include "Container.h"

class Bullet: public Container
{
public:
	Bullet(Vector2D& pos, Vector2D& dir, float speed);
	virtual ~Bullet() {};

	void update() override;

	bool isActive() const;

private:
	bool _active;
};

inline bool
Bullet::isActive() const {
	return _active;
}