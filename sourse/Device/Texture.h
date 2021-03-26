# pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "Geometry/Rect.h"
#include "BitMap/Color.h"


void init_device_texture(SDL_Renderer* holder_p);

class DevBuffer {

    SDL_Texture* devtx = nullptr;
    void Create(int width, int height);

    public:

        vec2<int> size;

        template <typename SizeType>
        DevBuffer(SizeType width, SizeType height) {
            Create((int)width, (int)height);
        }

        ~DevBuffer();

		void DrawRect(const Rect<int>& rect, const Color& col);
		void Project(DevBuffer* srs, const vec2<int>& pos);

		void draw_tex_ro_rend(const vec2<int>& pos);


        template <typename SizeType>
        void resize(SizeType width, SizeType height);

        void Assign(const Color& col) {
            DrawRect(Rect<int>(0, 0, size.x, size.y), col);
        }

        void DrawBounds(Rect<int>& rect, Color& col, short thickness) {
            
        }
    
};