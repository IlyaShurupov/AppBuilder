#include "../Device.h"

// #include "../../Application/UI/UInputs.h"
// #include "../../Application/UI/UInterface.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

DeviceManager::DeviceManager() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != -1) {
        printf("Error Initializing SDL: %s\n", SDL_GetError());
    }
}

DeviceManager::~DeviceManager() {
    SDL_Quit();
}

void DeviceManager::Inputs(UInputs* uinputs) {

}

void DeviceManager::Output(UIItem* UIroot) {

}

char* getExecutablePath() {
    return nullptr;
}