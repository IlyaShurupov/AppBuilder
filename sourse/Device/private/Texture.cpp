
#include "../Texture.h"

SDL_Renderer* holder;

void init_device_texture(SDL_Renderer* holder_p) {
    holder = holder_p;
}

void DevBuffer::Create(int width, int height) {
    size.assign(width, height);
    devtx = SDL_CreateTexture(holder, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
}

DevBuffer:: ~DevBuffer() {
    if (devtx) {
        SDL_DestroyTexture(devtx);
    }
}

void DevBuffer::DrawRect(const Rect<int>& rect, const Color& col){
        SDL_Rect dest;

        dest.x = rect.pos.x;
        dest.y = rect.pos.y;
        dest.w = rect.size.x;
        dest.h = rect.size.y;

        SDL_SetRenderTarget(holder, devtx);
        SDL_SetRenderDrawColor(holder, col.r * 255, col.g * 255, col.b * 255, col.a * 255);
        SDL_RenderFillRect(holder, &dest);
        SDL_SetRenderTarget(holder, nullptr);
}

void DevBuffer::Project(DevBuffer* srs, const vec2<int>& pos){
        SDL_Rect dest;

        dest.x = pos.x;
        dest.y = pos.y;
        SDL_QueryTexture(srs->devtx, NULL, NULL, &dest.w, &dest.h);

        SDL_SetRenderTarget(holder, devtx);
        SDL_RenderCopy(holder, srs->devtx, NULL, &dest);
        SDL_SetRenderTarget(holder, nullptr);
}

void DevBuffer::draw_tex_ro_rend(const vec2<int>& pos) {
    SDL_Rect dest;
    dest.x = pos.x;
    dest.y = pos.y;
    SDL_QueryTexture(devtx, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(holder, devtx, NULL, &dest);
    SDL_RenderPresent(holder);
}


template <typename SizeType>
void DevBuffer::resize(SizeType width, SizeType height) {
    if (size.x != width || size.y != height) {
        size.assign(width, height);
        SDL_DestroyTexture(devtx);
        devtx = SDL_CreateTexture(holder, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    }
}
