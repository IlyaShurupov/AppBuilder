
#include "Window/Window.h"

#include <GL/glew.h>

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#define NVGCOL(col) nvgRGBA(col.r * 255, col.g * 255, col.b * 255, col.a * 255)

Window::Window() {
	window = glfwCreateWindow(1000, 600, "NanoVG", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!window) {
		glfwTerminate();
	}

	if (glewInit() != GLEW_OK) {
		printf("Could not init glew.\n");
	}

	nvg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

	nvgCreateFont(nvg, "sans", "D:\\Dev\\tmp\\nanovg\\x64\\Debug\\Roboto-Regular.ttf");
}

void Window::BeginFrame() {
	int winWidth, winHeight;
	int fbWidth, fbHeight;
	float pxRatio;

	glfwGetWindowSize(window, &winWidth, &winHeight);
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

	// Calculate pixel ration for hi-dpi devices.
	pxRatio = (float)fbWidth / (float)winWidth;

	// Update and render
	glViewport(0, 0, fbWidth, fbHeight);

	glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	nvgBeginFrame(nvg, winWidth, winHeight, pxRatio);
}

void Window::EndFrame() {
	nvgEndFrame(nvg);
	glfwSwapBuffers(window);
}

void Window::SetBounds(const Rect<float>& _wrld_rec) {
	wrld_rec = _wrld_rec;
}

void Window::DrawRect(const Rect<float>& _rect, const Color& col, float radius) {

	nvgBeginPath(nvg);

	if (!radius) {
		nvgRect(nvg, wrld_rec.pos.x, wrld_rec.pos.y, wrld_rec.size.x, wrld_rec.size.y);
	}
	else {
		nvgRoundedRect(nvg, wrld_rec.pos.x, wrld_rec.pos.y, wrld_rec.size.x, wrld_rec.size.y, radius);
	}

	nvgFillColor(nvg, NVGCOL(col));
	nvgFill(nvg);
}

void Window::DrawText(const char* str, const float x, float y, float font_scale, const Color& col) {

	nvgFontSize(nvg, font_scale);
	nvgFontFace(nvg, "sans");
	nvgFillColor(nvg, NVGCOL(col));
	nvgTextAlign(nvg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	nvgText(nvg, x + wrld_rec.pos.x, y + wrld_rec.pos.y, str, NULL);
}

void Window::DrawBounds(const Rect<float>& _rect, const Color& col, short thickness) {

	nvgBeginPath(nvg);
	nvgRect(nvg, 122, 122, 122, 122);
	nvgCircle(nvg, 120, 120, 30);
	nvgPathWinding(nvg, NVG_HOLE);	// Mark circle as a hole.
	nvgFillColor(nvg, nvgRGBA(255, 192, 0, 100));
	nvgFill(nvg);

	nvgFontSize(nvg, 20.0f);
	nvgFontFace(nvg, "sans");
	nvgFillColor(nvg, nvgRGBA(255, 255, 255, 255));;
	nvgTextAlign(nvg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
	nvgText(nvg, 120, 120, "sdasdasdasdasdasdasd", NULL);
}

void Window::DrawLine(const vec2<SCR_INT>& head, const vec2<SCR_INT>& tail, const Color& col, short thickness) {
}

void Window::Clear(const Color& col) {
	DrawRect(wrld_rec, col);
}

void Window::GetCrsr(vec2<float>& crs) {

	vec2<double> incrs;
	glfwGetCursorPos(window, &incrs.x, &incrs.y);

	crs.x = (SCR_INT)incrs.x;
	crs.y = (SCR_INT)incrs.y;
}


Window::~Window() {
	nvgDeleteGL3(nvg);
}