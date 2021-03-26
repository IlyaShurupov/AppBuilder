#include "../Device.h"

// #include "../../Application/UI/UInputs.h"
// #include "../../Application/UI/UInterface.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "Texture.h"

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

void DeviceManager::Inputs(UInputs* uinputs) {

}

void DeviceManager::Output(UIItem* UIroot) {

}