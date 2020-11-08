#pragma once
#include "Vec2.h"

enum KeyState {
	EVENT_NONE = 0,
	PRESSED,
	RELEASED,
	HOLD,
	DOUBLE,
};

struct EventState {
	KeyState A;
	KeyState B;
	KeyState C;
	KeyState D;
	KeyState ENTER;
	KeyState SPACE;
	KeyState KEY_1;
	KeyState KEY_2;
	KeyState KEY_3;
	// ...
	short num = 9;
	vec2<short> Cursor;
	vec2<short> dCursor;
};