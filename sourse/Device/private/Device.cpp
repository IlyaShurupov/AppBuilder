#include "../Device.h"

#include "UI/UInputs.h"
#include "UI/UInterface.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "DevBuffer.h"

DeviceManager::DeviceManager() {
  SDL_Init(SDL_INIT_EVERYTHING);
	holder_root = SDL_CreateWindow("holder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2000, 1200, SDL_WINDOW_SHOWN);
	holder = SDL_CreateRenderer(holder_root, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
  init_device_texture(holder);
}

DeviceManager::~DeviceManager() {
    SDL_DestroyRenderer(holder);
	SDL_DestroyWindow(holder_root);
	SDL_Quit();
}

void UpdInputSate(Input& key, bool down, bool& IsEvent) {
  if ((int)key.state == (int)down) {
    return;
  }

  IsEvent = true;

  if (key.state == InputState::NONE) {
    key.state = InputState::PRESSED;
  } else if (key.state == InputState::HOLD) {
    key.state = InputState::RELEASED;
  } else {
    key.state = InputState(down);
  }
}

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

void DeviceManager::Inputs(UInputs* uinputs) {
    
  SDL_PumpEvents();
  
  UInputs& usin = *uinputs;
  usin.IsEvent = false;
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);

  UpdCursorPos(usin);

  UpdInputSate(usin.A, keystate[SDL_SCANCODE_A], usin.IsEvent);
  UpdInputSate(usin.B, keystate[SDL_SCANCODE_LEFT], usin.IsEvent);
  UpdInputSate(usin.C, keystate[SDL_SCANCODE_LEFT], usin.IsEvent);
  UpdInputSate(usin.D, keystate[SDL_SCANCODE_LEFT], usin.IsEvent);
  UpdInputSate(usin.F, keystate[SDL_SCANCODE_LEFT], usin.IsEvent);
  UpdInputSate(usin.G, keystate[SDL_SCANCODE_LEFT], usin.IsEvent);

  UpdInputSate(usin.K0, keystate[SDL_SCANCODE_LEFT], usin.IsEvent);
  UpdInputSate(usin.K1, keystate[SDL_SCANCODE_LEFT], usin.IsEvent);
  UpdInputSate(usin.K2, keystate[SDL_SCANCODE_LEFT], usin.IsEvent);
  UpdInputSate(usin.K3, keystate[SDL_SCANCODE_LEFT], usin.IsEvent);
  UpdInputSate(usin.K4, keystate[SDL_SCANCODE_LEFT], usin.IsEvent);
  UpdInputSate(usin.K5, keystate[SDL_SCANCODE_LEFT], usin.IsEvent);

  UpdInputSate(usin.SPACE, keystate[SDL_SCANCODE_SPACE], usin.IsEvent);
  UpdInputSate(usin.ENTER, keystate[SDL_SCANCODE_RETURN], usin.IsEvent);
  UpdInputSate(usin.SHIFT_L, keystate[SDL_SCANCODE_LSHIFT], usin.IsEvent);
  UpdInputSate(usin.ALT_L, keystate[SDL_SCANCODE_LALT], usin.IsEvent);
  UpdInputSate(usin.ESCAPE, keystate[SDL_SCANCODE_ESCAPE], usin.IsEvent);
  UpdInputSate(usin.EREASE, keystate[SDL_SCANCODE_BACKSPACE], usin.IsEvent);

  usin.IsEvent = usin.Cdelta.x || usin.Cdelta.y || usin.IsEvent;

  SDL_FlushEvents(SDL_USEREVENT, SDL_LASTEVENT);
}

void DeviceManager::Output(UIItem* UIroot) {
  SDL_SetRenderDrawColor(holder, 0, 0, 0, 0);
  SDL_RenderClear(holder);
  UIroot->hrchy.childs[0].buff->draw_tex_ro_rend(UIroot->hrchy.childs[0].rect.pos);
}