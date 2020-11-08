#include "public/Window.h"

Window::Window() {
	for (int i = 0; i < EventState.num; i++) {
		(&EventState.A)[i] = NONE;
	}
	Fbuff = nullptr;
}

Window::~Window()
{
}