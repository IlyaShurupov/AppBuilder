#pragma once 

#include "Types.h"

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
      const uint1 *keystate;

    public:
        Device();
        void PumpEvents();
        void ClearEvents();
        InputState GetKeyState(int ascii_code, InputState current);
        void StartDraw();
        void GetCrsr(vec2<float>& crs);
        void DrawBuff(class DevBuffer* buff, vec2<aligned> pos);
        ~Device();

};