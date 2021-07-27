#pragma once

#include "Object.h"

class Application : public Obj {

	Application& operator = (const Application& in);
	Application(const Application& in) : Obj(in) {}

public:

	Application(Obj* prnt);
	void Compose();
	void Run();
	virtual ~Application() {}
};
