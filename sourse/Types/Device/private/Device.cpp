#include "Device/Device.h"

#include <GL/glew.h>

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"


#ifdef _WIN
#include <Windows.h>
bool keyIsDown(int code) {
	return GetAsyncKeyState(code) & 0x8000;
}
#else 
bool keyIsDown(int code) {
	return false;
}
#endif

GLFWwindow* window = nullptr;
NVGcontext* vg = nullptr;

Device::Device() {

	if (!glfwInit()) {
		printf("Failed to init GLFW.");
	}

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	window = glfwCreateWindow(1000, 600, "NanoVG", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!window) {
		glfwTerminate();
	}

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("Could not init glew.\n");
	}

	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

	if (vg == NULL) {
		printf("Could not init nanovg.\n");
	}

	// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
	glGetError();

	glfwSwapInterval(0);

	nvgCreateFont(vg, "sans", "D:\\Dev\\tmp\\nanovg\\x64\\Debug\\Roboto-Regular.ttf");

}

Device::~Device() {
	nvgDeleteGL3(vg);
	glfwTerminate();
}

void Device::PumpEvents() {
	glfwPollEvents();
}

void Device::ClearEvents() {
}

InputState Device::GetKeyState(int key_code, InputState current) {

	if (keyIsDown(key_code)) {
		if (current == InputState::NONE) {
			return InputState::PRESSED;
		}
		else {
			return InputState::HOLD;
		}
	}
	else {
		if (current == InputState::HOLD) {
			return InputState::RELEASED;
		}
		else {
			return InputState::NONE;
		}
	}
}

void Device::StartDraw() {
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
	nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
}

void Device::EndDraw() {

	nvgEndFrame(vg);
	glfwSwapBuffers(window);
}


void Device::GetCrsr(vec2<float>& crs) {

	vec2<double> incrs;
	glfwGetCursorPos(window, &incrs.x, &incrs.y);

	crs.x = (SCR_INT)incrs.x;
	crs.y = (SCR_INT)incrs.y;
}