#include "StopOnBorder.h"

#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Manager.h"
#include "../../../game/Game.h"
#include "Transform.h"


StopOnBorder::StopOnBorder(const camera_screen& camera, float w, float h):_tr(nullptr), _cam(camera), _halfWidth(w/2.0f), _halfHeight(h/2.0f){}
StopOnBorder::~StopOnBorder() {}

void
StopOnBorder::initComponent() {
	_tr = Game::Instance()->get_mngr()->getComponent<Transform>(_ent);
	assert(_tr != nullptr);
}
void StopOnBorder::update(Uint32 delta_time) {
    (void)delta_time;
    auto& pos = _tr->getPos();
    auto& dir = _tr->getDir();

    float limitX = 2 * _cam.camera.half_size.x;
    float limitY = 2 * _cam.camera.half_size.y;

    if (pos.getX() - _halfWidth < -limitX) {
        pos.setX(-limitX + _halfWidth);
        dir.setX(0); 
    }
    else if (pos.getX() + _halfWidth > limitX) {
        pos.setX(limitX - _halfWidth);
        dir.setX(0);
    }

    if (pos.getY() - _halfHeight < -limitY) {
        pos.setY(-limitY + _halfHeight);
        dir.setY(0); 
    }
    else if (pos.getY() + _halfHeight > limitY) {
        pos.setY(limitY - _halfHeight);
        dir.setY(0);
    }
}

