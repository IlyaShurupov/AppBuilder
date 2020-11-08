#pragma once

#include "Window.h"
#include "LinkedList.h"
#include "Object.h"
#include "Operator.h"

struct Context {
	List<Window> windows;
	List<Object> Collection;
	List<Operator> Operators;
};