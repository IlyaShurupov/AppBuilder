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
    Uint32 state = SDL_GetGlobalMouseState(&incrs.x, &incrs.y);
    
    // incrs.y = Height - incrs.y;

    usin.Cursor.x = (SCR_INT)incrs.x;
    usin.Cursor.y = (SCR_INT)incrs.y;
}

void DeviceManager::Inputs(UInputs* uinputs) {
    
    SDL_PumpEvents();

    UInputs& usin = *uinputs;

    usin.IsEvent = false;
    
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    UpdInputSate(usin.LMB, keystate[SDL_SCANCODE_LEFT], usin.IsEvent);
    UpdInputSate(usin.ALT_L, keystate[SDL_SCANCODE_RIGHT], usin.IsEvent);

    UpdCursorPos(usin);


    usin.Cdelta.x = usin.Cursor.x - usin.PrevCursor.x;
    usin.Cdelta.y = usin.Cursor.y - usin.PrevCursor.y;

    usin.IsEvent = usin.Cdelta.x || usin.Cdelta.y || usin.IsEvent;

    SDL_FlushEvents(SDL_USEREVENT, SDL_LASTEVENT);
}

void DeviceManager::Output(UIItem* UIroot) {
  SDL_SetRenderDrawColor(holder, 0, 0, 0, 0);
  SDL_RenderClear(holder);
  UIroot->hrchy.childs[0].buff->draw_tex_ro_rend(UIroot->hrchy.childs[0].rect.pos);
}