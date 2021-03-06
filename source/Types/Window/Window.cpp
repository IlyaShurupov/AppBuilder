
#include "Window/Window.h"

#include <GL/glew.h>

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
//#include "GLFW/glfw3native.h"

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#include "Strings/Strings.h"

#define NVGCOL(col) nvgRGBA((uint1)(col.r * 255), (uint1)(col.g * 255), (uint1)(col.b * 255), (uint1)(col.a * 255))

#include <Windows.h>

void get_font_dir(const char* file, Str* str) {
	*str = file;
	str->trim(Range(0, str->rfind('\\', Range(0, str->len()))));
	*str += "RobotoRegular.ttf";
}

Window::Window() {
	
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

	window = glfwCreateWindow(1000, 700, "NanoVG", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);

	if (!window) {
		glfwTerminate();
	}

	if (glewInit() != GLEW_OK) {
		printf("Could not init glew.\n");
	}

	nvg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

	Str font;
	get_font_dir(__FILE__, &font);
	nvgCreateFont(nvg, "sans", font.str);
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

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	nvgBeginFrame(nvg, (float)winWidth, (float)winHeight, pxRatio);
}

void Window::EndFrame() {
	nvgEndFrame(nvg);
	glfwSwapBuffers(window);
}

void Window::SetBounds(const Rect<float>& _bounds) {
	bounds = _bounds;
}

void Window::SetCanvasRect(const Rect<float>& rect) {
	wrld_rec = rect;
}

void Window::RRect(Rect<float> _rect, const Color& col, float radius) {

	_rect.pos += wrld_rec.pos;
	_rect.clamp(bounds);

	if (_rect.size.x < 2 || _rect.size.y < 2) {
		return;
	}

	nvgBeginPath(nvg);

	if (!radius) {
		nvgRect(nvg, _rect.pos.x, _rect.pos.y, _rect.size.x, _rect.size.y);
	}
	else {
		nvgRoundedRect(nvg, _rect.pos.x, _rect.pos.y, _rect.size.x, _rect.size.y, radius);
	}

	nvgFillColor(nvg, NVGCOL(col));
	nvgFill(nvg);
}

void Window::Text(const char* const_c_str, float x, float y, float font_scale, const Color& col) {

	char* c_str = (char*)const_c_str;

	nvgFontSize(nvg, font_scale);
	nvgFontFace(nvg, "sans");
	nvgFillColor(nvg, NVGCOL(col));
	nvgTextAlign(nvg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);


	float char_width = font_scale * 0.55f;
	float char_height = font_scale * 1.2f;

	int text_length = cstr_len(c_str);
	int line_start_idx = 0;
	int line_num = 0;
	char tmp_char = ' ';


	bool last_line = false;

	do {

		int line_end_idx = find_char(c_str, line_start_idx + 1, '\n');
		if (line_end_idx == -1) {
			if (last_line) {
				break;
			}
			else {
				line_end_idx = text_length;
				last_line = true;
			}
		}
		
		int line_len = line_end_idx - line_start_idx;
		Rect<float> text_rect(x, y + (char_height * line_num), char_width * (line_len), font_scale);

		text_rect.pos += wrld_rec.pos;

		// clamp y
		if (text_rect.pos.y < bounds.pos.y || text_rect.pos.y + text_rect.size.y > bounds.pos.y + bounds.size.y) {
			return;
		}

		// clamp x
		float left_remainder = bounds.pos.x - text_rect.pos.x;
		float right_remainder = text_rect.pos.x + text_rect.size.x - (bounds.pos.x + bounds.size.x);
		int trim_left = 0;
		int trim_right = 0;

		if (left_remainder > 0) {
			trim_left = (int)((float)left_remainder / char_width) + 1;
		}
		if (right_remainder > 0) {
			trim_right = (int)((float)right_remainder / char_width) + 2;
		}

		if (trim_left + trim_right > line_len) {
			return;
		}

		int line_last_draw_char = line_start_idx + line_len - trim_right + 1;
		tmp_char = c_str[line_last_draw_char];
		c_str[line_last_draw_char] = '\0';

		// draw
		text_rect.clamp(bounds);
		nvgText(nvg, text_rect.pos.x, text_rect.pos.y, &c_str[line_start_idx + trim_left], NULL);

		c_str[line_last_draw_char] = tmp_char;

		line_num++;
		line_start_idx = line_end_idx;

	} while (1);
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
	RRect(wrld_rec, col);
}

void Window::GetCrsr(vec2<float>& crs) {

	vec2<double> incrs;
	glfwGetCursorPos(window, &incrs.x, &incrs.y);

	crs.x = (float)incrs.x;
	crs.y = (float)incrs.y;
}


Window::~Window() {
	nvgDeleteGL3(nvg);
}