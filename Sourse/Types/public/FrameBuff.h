#pragma once

#define CHANEL_CAPACITY unsigned char

typedef struct Color4 {
	CHANEL_CAPACITY R = 0;
	CHANEL_CAPACITY G = 0;
	CHANEL_CAPACITY B = 0;
	CHANEL_CAPACITY A = 0;
} Color4;


class FBuff
{
public:
	Color4* Buff;
	size_t height, width;
	FBuff(size_t width, size_t height);
	~FBuff();

	Color4* get(size_t x,  size_t y);
	void set(size_t x, size_t y, Color4* color);
	void clear();
	void DrawRect(size_t x, size_t y, Color4& color, int width, int height);
private:
};