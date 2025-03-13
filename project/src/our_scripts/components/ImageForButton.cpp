#include "ImageForButton.h"

#include <cassert>

#include "../../ecs/Manager.h"
#include "../../sdlutils/macros.h"
#include "../../sdlutils/Texture.h"
#include "Transform.h"
#include "../../game/Game.h"


ImageForButton::ImageForButton() : Image(), _selected_tex(nullptr) {
}

ImageForButton::ImageForButton(Texture* tex, Texture* stex) : Image(tex), _selected_tex(stex) {
	assert(stex != nullptr);
}
void
ImageForButton::swap_textures() {
	Texture* aux = _tex;
	_tex = _selected_tex;
	_selected_tex = aux;
}