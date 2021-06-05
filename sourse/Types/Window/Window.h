#pragma once

#include "Geometry/Rect.h"
#include "BitMap/Color.h"

class Window {

	Rect<float> wrld_rec;
	class NVGcontext* nvg = nullptr;
	class GLFWwindow* window = nullptr;

public:

	Window();


	void BeginFrame();
	void EndFrame();

	void SetBounds(const Rect<float>& wrld_rec);

	void DrawRect(const Rect<float>& rect, const Color& col, float radius = 0);
	void DrawBounds(const Rect<float>& rect, const Color& col, short thickness);
	void DrawLine(const vec2<SCR_INT>& head, const vec2<SCR_INT>& tail, const Color& col, short thickness);
	void DrawText(const char* str, const float x, float y, float font_scale, const Color& col);
	void Clear(const Color& col);

	void GetCrsr(vec2<float>& crs);

	~Window();
};
