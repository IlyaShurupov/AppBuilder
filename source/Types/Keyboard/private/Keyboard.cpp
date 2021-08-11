#include "Keyboard/Keyboard.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool keyIsDown(int code);

#ifdef _WIN
#include <Windows.h>
bool keyIsDown(int code) {
	return GetAsyncKeyState(code) & 0x8000;
}
#endif


Keyboard::Keyboard() {

	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	glewExperimental = GL_TRUE;

	// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
	glGetError();

	glfwSwapInterval(0);

}

Keyboard::~Keyboard() {
	glfwTerminate();
}

void Keyboard::PumpEvents() {
	glfwPollEvents();
}

void Keyboard::ClearEvents() {

}

InputState Keyboard::GetKeyState(int key_code, InputState current) {

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
