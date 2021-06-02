# pragma once 


#include "Geometry/Rect.h"
#include "BitMap/Color.h"

class DevBuffer {

	Rect<float>* rec;

public:

	DevBuffer(Rect<float>& rect);

	void DrawRect(const Rect<float>& rect, const Color& col, float radius = 0);
	void DrawBounds(const Rect<float>& rect, const Color& col, short thickness);
	void DrawLine(const vec2<SCR_INT>& head, const vec2<SCR_INT>& tail, const Color& col, short thickness);
	void DrawText(const char* str, const float x, float y, float font_scale, const Color& col);
	void Clear(const Color& col);

	~DevBuffer();

};