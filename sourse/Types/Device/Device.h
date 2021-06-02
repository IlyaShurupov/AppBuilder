#pragma once 

#include "Types.h"

enum class InputState {
	NONE = 0,
	HOLD,
	PRESSED,
	RELEASED,
};

class Device {

public:
	Device();
	
	void StartDraw();
	void EndDraw();
 
	InputState GetKeyState(int ascii_code, InputState current);
	void GetCrsr(vec2<float>& crs);
	void PumpEvents();
	void ClearEvents();
	
	~Device();
};