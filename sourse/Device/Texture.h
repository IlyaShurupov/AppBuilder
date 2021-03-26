# pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "Geometry/Vec2.h"

template <typename COL>
class DevTexture {
    private:
        SDL_Texture* tex = nullptr;
    public:
        vec2<int> size;

        DevTexture() {}
        ~DevTexture() {
            if (tex) {
                // SDL_DestroyTexture(tex);
            }
        }
  
        template <typename SizeType>
        DevTexture(SizeType width, SizeType height) {
            size.assign(width, height);
            // tex = SDL_CreateTexture();
        }

        template <typename SizeType>
        void resize(SizeType width, SizeType height) {
            if (size.x != width || size.y != height) {
                size.assign(width, height);
                // SDL_DestroyTexture(tex);
                // tex = SDL_CreateTexture();
            }
        }

};