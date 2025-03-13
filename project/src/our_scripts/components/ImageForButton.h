#pragma once
#include "Image.h"

class ImageForButton : public Image {
public:
	__CMPID_DECL__(ecs::cmp::IMAGE)
		ImageForButton();
	ImageForButton(Texture* tex, Texture* stex);
	virtual ~ImageForButton() { };
	void swap_textures();
protected:
	Texture* _selected_tex;
};
