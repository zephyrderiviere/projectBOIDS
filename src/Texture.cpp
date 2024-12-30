#include "Texture.hpp"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <algorithm>
#include "utils/ErrorHandling.hpp"
#include "utils/Position.hpp"




Texture::Texture(SDL_Renderer* renderer, char const* filepath, int const x, int const y) {

    this->renderer = renderer;
    SDL_Surface* surface = SDL_LoadBMP(filepath);
    if (surface == NULL) {
        throw exception(LOADING_TEXTURE_FILE_ERROR);
    }

    dimensions.h = surface->h;
    dimensions.w = surface->w;

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL) {
        throw exception(CONVERTING_SURFACE_TO_TEXTURE_ERROR);
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    dimensions.x = x;
    dimensions.y = y;
    
}

Texture::~Texture() {
    SDL_DestroyTexture(texture);
}




void Texture::render() const {
    if (SDL_RenderCopy(renderer, texture, NULL, &dimensions) != 0) {
        throw exception(TEXTURE_RENDER_ERROR);
    }
}

void Texture::render(Position<int> p) const {
    SDL_Rect r = {p.i, p.j, dimensions.w, dimensions.h};
    if (SDL_RenderCopy(renderer, texture, NULL, &r) != 0) {
        throw exception(TEXTURE_RENDER_ERROR);
    }
}


void Texture::render(Position<float> p, float objectScale, float scale_w, float scale_h) const {
    SDL_FRect r = {p.i, p.j, objectScale * scale_w * dimensions.w, objectScale * scale_h * dimensions.h};
    if (SDL_RenderCopyF(renderer, texture, NULL, &r) != 0) {
        throw exception(TEXTURE_RENDER_ERROR);
    }
}

void Texture::render(Position<float> p, Position<float> orientation, float objectScale, float scale_w, float scale_h) const {
    SDL_FRect r = {p.i, p.j, objectScale * scale_w * dimensions.w, objectScale * scale_h * dimensions.h};
    double angle = 180.0 / M_PI * acos(std::clamp(orientation * ux, -1.0f, 1.0f));
    if (SDL_RenderCopyExF(renderer, texture, NULL, &r, angle, NULL, SDL_FLIP_NONE) != 0) {
        throw exception(TEXTURE_RENDER_ERROR);
    }
}

Text::Text(SDL_Renderer* renderer, char const* text, TTF_Font* font, SDL_Color const& color, int const x, int const y) {
    
    this->renderer = renderer;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (surface == NULL) {
        throw exception(LOADING_TEXT_ERROR);
    }

    dimensions.h = surface->h;
    dimensions.w = surface->w;

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL) {
        throw exception(CONVERTING_SURFACE_TO_TEXTURE_ERROR);
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    dimensions.x = x;
    dimensions.y = y;
}

Text::Text(SDL_Renderer* renderer, char const* text, char const* font_filepath, unsigned size, SDL_Color const& color, int const x, int const y) {
    TTF_Font* font = TTF_OpenFont(font_filepath, size);

    this->renderer = renderer;

    if (font == NULL) {
        throw exception(FONT_CREATION_ERROR);
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (surface == NULL) {
        throw exception(LOADING_TEXT_ERROR);
    }

    dimensions.h = surface->h;
    dimensions.w = surface->w;

    texture = SDL_CreateTextureFromSurface(renderer, surface);


    if (texture == NULL) {
        throw exception(CONVERTING_SURFACE_TO_TEXTURE_ERROR);
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    dimensions.x = x;
    dimensions.y = y;

    TTF_CloseFont(font);
}