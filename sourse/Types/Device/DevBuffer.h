# pragma once 


#include "Geometry/Rect.h"
#include "BitMap/Color.h"

class DevBuffer {

	Rect<float>* rec;

public:

	DevBuffer(Rect<float>& rect);

	void DrawRect(const Rect<int>& rect, const Color& col);
	void DrawBounds(Rect<int>& rect, const Color& col, short thickness);
	void DrawLine(const vec2<SCR_INT>& head, const vec2<SCR_INT>& tail, const Color& col, short thickness);

	~DevBuffer();

	/*
	void draw_tex_ro_rend(const vec2<int>& pos);

	template <typename SizeType>
	void resize(SizeType width, SizeType height) {
		Resize_int((int)width, (int)height);
	}

	void Assign(const Color& col) {
		DrawRect(Rect<int>(0, 0, size.x, size.y), col);
	}

	*/
	//void Project(DevBuffer* srs, const vec2<int>& pos);
};