#pragma once
#include "Event.h"
#include "FrameBuff.h"

class Window
{
public:
	vec2<short> WorldPos;
	vec2<short> Size;

	EventState EventState;
	FBuff* Fbuff;

	Window();
	~Window();

	void Upd();
};