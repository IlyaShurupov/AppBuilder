
#include "Device/DevBuffer.h"

#include "nanovg.h"

extern NVGcontext* vg;

DevBuffer::DevBuffer(Rect<float>& rect) {
	rec = &rect;
}

void DevBuffer::DrawRect(const Rect<int>& rect, const Color& col) {

	nvgBeginPath(vg);
	nvgRect(vg, 100, 100, 120, 30);
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

void DevBuffer::DrawBounds(Rect<int>& rect, const Color& col, short thickness) {
}

void DevBuffer::DrawLine(const vec2<SCR_INT>& head, const vec2<SCR_INT>& tail, const Color& col, short thickness) {
}

DevBuffer::~DevBuffer() {
}