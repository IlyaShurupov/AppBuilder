#include "public/Window.h"

Window::Window() {
	for (int i = 0; i < EventState.num; i++) {
		(&EventState.A)[i] = EVENT_NONE;
	}
	Fbuff = nullptr;
}

Window::~Window()
{
}

void Window::Upd() {
}

bool Window::IsEvent() {
	return true;
}
