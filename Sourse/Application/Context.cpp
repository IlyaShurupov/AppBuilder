#pragma once
#include "public/Context.h"

Window::Window() {
  for (int i = 0; i < EventState.num; i++) {
    (&EventState.A)[i] = EVENT_NONE;
  }

  Fbuff = nullptr;
}

Window::~Window() {}

void Window::UpdEventState() {}

bool Window::IsEvent() { return true; }

void Window::Draw(class Context* C) {}

Context::Context() {}

Context::~Context() {}

Window* Context::getActiveWindow() { return ActiveWin; }
