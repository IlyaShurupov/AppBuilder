
#include "../DevBuffer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

SDL_Renderer* holder;

void init_device_texture(void* holder_p) {
    holder = (SDL_Renderer *)holder_p;
}

void DevBuffer::Create(int width, int height) {
    size.assign(width, height);
    devtx = (void *)SDL_CreateTexture(holder, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
}

DevBuffer:: ~DevBuffer() {
    if (devtx) {
        SDL_DestroyTexture((SDL_Texture *)devtx);
    }
}

void DevBuffer::DrawRect(const Rect<int>& rect, const Color& col){
    SDL_Rect dest;

    dest.x = rect.pos.x;
    dest.y = rect.pos.y;
    dest.w = rect.size.x;
    dest.h = rect.size.y;

    SDL_SetRenderTarget(holder, (SDL_Texture *)devtx);
    SDL_SetRenderDrawColor(holder, col.r * 255, col.g * 255, col.b * 255, col.a * 255);
    SDL_RenderFillRect(holder, &dest);
    SDL_SetRenderTarget(holder, nullptr);
}

void DevBuffer::Project(DevBuffer* srs, const vec2<int>& pos){
    SDL_Rect dest;

    dest.x = pos.x;
    dest.y = pos.y;
    SDL_QueryTexture((SDL_Texture *)srs->devtx, NULL, NULL, &dest.w, &dest.h);

    SDL_SetRenderTarget(holder, (SDL_Texture *)devtx);
    SDL_RenderCopy(holder, (SDL_Texture *)srs->devtx, NULL, &dest);
    SDL_SetRenderTarget(holder, nullptr);
}

void DevBuffer::draw_tex_ro_rend(const vec2<int>& pos) {
    SDL_Rect dest;
    dest.x = pos.x;
    dest.y = pos.y;
    SDL_QueryTexture((SDL_Texture *)devtx, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(holder, (SDL_Texture *)devtx, NULL, &dest);
    SDL_RenderPresent(holder);
}

void DevBuffer::Resize_int(int width, int height) {
    if (size.x != width || size.y != height) {
        size.assign(width, height);
        SDL_DestroyTexture((SDL_Texture *)devtx);
        devtx = (void *)SDL_CreateTexture(holder, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    }
}

void DevBuffer::DrawLine(const vec2<SCR_INT>& head, const vec2<SCR_INT>& tail, const Color& col, short thickness) {
    SDL_SetRenderTarget(holder, (SDL_Texture *)devtx);
    SDL_SetRenderDrawColor(holder, col.r * 255, col.g * 255, col.b * 255, col.a * 255);
    SDL_RenderDrawLine(holder, head.x, head.y, tail.x, tail.y);
    SDL_SetRenderTarget(holder, nullptr);
}

void DevBuffer::DrawBounds(Rect<int>& rect, const Color& col, short thick) {
    rect.pos += 1;
    rect.size -= 2;

    DrawLine(vec2<SCR_INT>(rect.pos), vec2<SCR_INT>(rect.pos.x + rect.size.x, rect.pos.y), col, thick);
    DrawLine(vec2<SCR_INT>(rect.pos), vec2<SCR_INT>(rect.pos.x, rect.pos.y + rect.size.y), col, thick);
    DrawLine(vec2<SCR_INT>(rect.pos.x + rect.size.x, rect.pos.y), vec2<SCR_INT>(rect.size_vec_w()), col, thick);
    DrawLine(vec2<SCR_INT>(rect.pos.x, rect.pos.y + rect.size.y), vec2<SCR_INT>(rect.size_vec_w()), col, thick);
}