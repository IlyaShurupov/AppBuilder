#include "Device/Device.h"

// #include "UI/UInputs.h"
// #include "UI/UInterface.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include "Device/DevBuffer.h"

Device::Device() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
	holder_root = SDL_CreateWindow("holder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, SDL_WINDOW_SHOWN);
	holder = SDL_CreateRenderer(holder_root, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
  init_device_texture(holder);

  keystate = SDL_GetKeyboardState(NULL);
  CodeMap['a'] = SDL_SCANCODE_A;
  CodeMap['b'] = SDL_SCANCODE_B;
}

Device::~Device() {
    SDL_DestroyRenderer(holder);
	SDL_DestroyWindow(holder_root);
	SDL_Quit();
}

void Device::PumpEvents() {
  SDL_PumpEvents();
}

InputState Device::GetKeyState(int ascii_code, InputState current)  { 

  if (keystate[CodeMap[ascii_code]]) {
    if (current == InputState::NONE) {
      return InputState::PRESSED;
    } else  {
      return InputState::HOLD;
    } 
  } else {
    if (current == InputState::HOLD) {
      return InputState::RELEASED;
    } else  {
      return InputState::NONE;
    } 
  }
}

void Device::ClearEvents() {
  SDL_FlushEvents(SDL_USEREVENT, SDL_LASTEVENT);
}

void Device::StartDraw () {
  SDL_SetRenderDrawColor(holder, 0, 0, 0, 0);
  SDL_RenderClear(holder);
}

void Device::DrawBuff(DevBuffer* buff, vec2<aligned> pos) {
  buff->draw_tex_ro_rend(pos);
}

void Device::GetCrsr(vec2<float>& crs) {

  vec2<int> incrs;
  Uint32 state = SDL_GetMouseState(&incrs.x, &incrs.y);

  crs.x = (SCR_INT)incrs.x;
  crs.y = (SCR_INT)incrs.y;
}