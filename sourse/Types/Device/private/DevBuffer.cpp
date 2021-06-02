
#include "Device/DevBuffer.h"

#include "nanovg.h"

extern NVGcontext* vg;

#define NVGCOL(col) nvgRGBA(col.r * 255, col.g * 255, col.b * 255, col.a * 255)

DevBuffer::DevBuffer(Rect<float>& rect) {
	rec = &rect;
}

void DevBuffer::DrawRect(const Rect<float>& _rect, const Color& col, float radius) {

	Rect<float> wrld_rect(_rect);
	wrld_rect.pos += rec->pos;
		
	nvgBeginPath(vg);
	
	if (!radius) {
		nvgRect(vg, wrld_rect.pos.x, wrld_rect.pos.y, wrld_rect.size.x, wrld_rect.size.y);
	}
	else {
		nvgRoundedRect(vg, wrld_rect.pos.x, wrld_rect.pos.y, wrld_rect.size.x, wrld_rect.size.y, radius);
	}
	
	nvgFillColor(vg, NVGCOL(col));
	nvgFill(vg);
}

void DevBuffer::DrawText(const char* str, const float x, float y, float font_scale, const Color& col) {

	nvgFontSize(vg, font_scale);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, NVGCOL(col));
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	nvgText(vg, x + rec->pos.x, y + rec->pos.y, str, NULL);
}

void DevBuffer::DrawBounds(const Rect<float>& _rect, const Color& col, short thickness) {

	nvgBeginPath(vg);
	nvgRect(vg, 122, 122, 122, 122);
	nvgCircle(vg, 120, 120, 30);
	nvgPathWinding(vg, NVG_HOLE);	// Mark circle as a hole.
	nvgFillColor(vg, nvgRGBA(255, 192, 0, 100));
	nvgFill(vg);

	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));;
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	nvgText(vg, 120, 120, "sdasdasdasdasdasdasd", NULL);
}

void DevBuffer::DrawLine(const vec2<SCR_INT>& head, const vec2<SCR_INT>& tail, const Color& col, short thickness) {
}

void DevBuffer::Clear(const Color& col) {
	DrawRect(Rect<int>(*rec), col);
}

DevBuffer::~DevBuffer() {
}