#pragma once

#include "Window.h"
#include "LinkedList.h"
#include "Object.h"
#include "Operator.h"

struct Context {
	Window* ActiveWin;
	List<Window> Windows;
	List<Object> Collection;
	List<Operator> Operators;
};