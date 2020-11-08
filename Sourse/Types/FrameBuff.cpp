#include "public/FrameBuff.h"
#include <cassert>

#define IDX3D(width, depth, x, y, z) (width * depth * y + depth * x + z)
#define IDX2D(width, x, y) (width * y + x)

FBuff::FBuff(size_t height, size_t width) {
	this->height = height;
	this->width = width;
	Buff = new Color4[height * width];
}

FBuff::~FBuff() {
	delete Buff;
}

Color4* FBuff::get(size_t x, size_t y) {
	return &Buff[IDX2D(width, x, y)];
}

void FBuff::set(size_t x, size_t y, Color4* color) {
	Color4* Color = &Buff[IDX2D(width, x, y)];
	Color->R = color->R;
	Color->G = color->G;
	Color->B = color->B;
	Color->A = color->A;
}

void FBuff::clear() {
	size_t len = width * height;
	for (size_t i = 0; i < len; i++) {
		Buff[i].R = 0;
		Buff[i].G = 0;
		Buff[i].B = 0;
		Buff[i].A = 0;
	}
}

void FBuff::DrawRect(size_t x, size_t y, Color4& color, int width, int height) {

	assert((x + width < this->width) && (y + height < this->height));

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			this->set(i + x, j + y, &color);
		}
	}
}
