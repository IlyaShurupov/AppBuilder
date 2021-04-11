#include "Device/Device.h"

// #include "UI/UInputs.h"
// #include "UI/UInterface.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "Device/DevBuffer.h"

Device::Device() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
	holder_root = SDL_CreateWindow("holder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DM.w, DM.h, SDL_WINDOW_SHOWN);
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

/*

void UpdCursorPos(UInputs& usin) {

  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  auto Width = DM.w;
  auto Height = DM.h;
  usin.PrevCursor = usin.Cursor;
  vec2<int> incrs;
  Uint32 state = SDL_GetMouseState(&incrs.x, &incrs.y);
  
  // incrs.y = Height - incrs.y 
  usin.Cursor.x = (SCR_INT)incrs.x;
  usin.Cursor.y = (SCR_INT)incrs.y;
  usin.Cdelta.x = usin.Cursor.x - usin.PrevCursor.x;
  usin.Cdelta.y = usin.Cursor.y - usin.PrevCursor.y;

  UpdInputSate(usin.LMB, state & SDL_BUTTON(SDL_BUTTON_LEFT), usin.IsEvent);
  UpdInputSate(usin.RMB, state & SDL_BUTTON(SDL_BUTTON_RIGHT), usin.IsEvent);
  UpdInputSate(usin.MMB, state & SDL_BUTTON(SDL_BUTTON_MIDDLE), usin.IsEvent);
}

*/