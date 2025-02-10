#include "InputComponent.h"

class ShootComponent : public InputComponent 
{
private:
	float _shootCoolDown;
	float _lastShoot;

public:
	ShootComponent(float shootCoolDown);
	virtual ~ShootComponent() {};

	void handleInput(Container* o) override;

};