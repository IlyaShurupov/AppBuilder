#pragma once 

enum class InputState {
  NONE = 0,
  HOLD,
  PRESSED,
  RELEASED,
};

class Device {

    friend class DevBuffer;

    private:
        class SDL_Window*	holder_root = nullptr;
	    class SDL_Renderer* holder = nullptr;
        int CodeMap[255];

    public:
        Device();
        void PumpEvents();
        void ClearEvents();
        InputState GetKeyState(int ascii_code, InputState current);
        ~Device();

};