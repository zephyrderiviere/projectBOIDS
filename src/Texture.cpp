#include "Texture.hpp"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <algorithm>
#include <cmath>
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
    SDL_FreeSurface(surface);

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


float getRenderAngle(Position<float> const& orientation) {
    float angle;
    if (orientation.norm() < 1e-4f) {
        angle = 0.0f;
    } else if (orientation.j < 0) {
        // 180 < angle < 360
        angle += 180.0f * (1.0f + 1.0f / M_PIf * std::acos(std::clamp(orientation * (-ux), -1.0f, 1.0f)));
    } else {
        angle = (180.0f / M_PIf) * std::acos(std::clamp(orientation * ux, -1.0f, 1.0f));
    }
    return angle;
}

void Texture::render() const {
    if (SDL_RenderCopy(renderer, texture, NULL, &dimensions) != 0) {
        throw exception(TEXTURE_RENDER_ERROR);
    }
}

void Texture::render(Position<int> const& p) const {
    SDL_Rect r = {p.i, p.j, dimensions.w, dimensions.h};
    if (SDL_RenderCopy(renderer, texture, NULL, &r) != 0) {
        throw exception(TEXTURE_RENDER_ERROR);
    }
}


void Texture::render(Position<float> const& p, float objectScale, float scale_w, float scale_h) const {
    SDL_FRect r = {p.i, p.j, objectScale * scale_w * dimensions.w, objectScale * scale_h * dimensions.h};
    if (SDL_RenderCopyF(renderer, texture, NULL, &r) != 0) {
        throw exception(TEXTURE_RENDER_ERROR);
    }
}

void Texture::render(Position<float> const& p, Position<float> const& orientation, float objectScale, float scale_w, float scale_h) const {
    SDL_FRect r = {p.i, p.j, objectScale * scale_w * dimensions.w, objectScale * scale_h * dimensions.h};
    float angle = getRenderAngle(orientation);
    if (SDL_RenderCopyExF(renderer, texture, NULL, &r, angle, NULL, SDL_FLIP_NONE) != 0) {
        throw exception(TEXTURE_RENDER_ERROR);
    }
}


void Atlas::renderSubTexture(Position<int> const& sub) const {
    if (!sub.inRectangle(0, n-1, 0, m-1)) {
        throw exception(ATLAS_OUT_OF_RANGE_RENDER_ERROR);
    }

    SDL_Rect src = {sub.i * sub_w, sub.j * sub_h, sub_w, sub_h};
    if (SDL_RenderCopy(renderer, texture, &src, &dimensions) != 0) {
        throw exception(ATLAS_RENDER_ERROR);
    }
}


void Atlas::renderSubTexture(Position<int> const& sub, Position<int> const& p) const {
    if (!sub.inRectangle(0, n-1, 0, m-1)) {
        throw exception(ATLAS_OUT_OF_RANGE_RENDER_ERROR);
    }

    SDL_Rect src = {sub.i * sub_w, sub.j * sub_h, sub_w, sub_h};
    SDL_Rect dest = {p.i, p.j, dimensions.w, dimensions.h};
    if (SDL_RenderCopy(renderer, texture, &src, &dest) != 0) {
        throw exception(ATLAS_RENDER_ERROR);
    }
}
void Atlas::renderSubTexture(Position<int> const& sub, Position<float> const& p, float objectScale, float scale_w, float scale_h) const {
    if (!sub.inRectangle(0, n-1, 0, m-1)) {
        throw exception(ATLAS_OUT_OF_RANGE_RENDER_ERROR);
    }
    SDL_Rect src = {sub.i * sub_w, sub.j * sub_h, sub_w, sub_h};
    SDL_FRect dest = {p.i, p.j, objectScale * scale_w * dimensions.w, objectScale * scale_h * dimensions.h};
    if (SDL_RenderCopyF(renderer, texture, &src, &dest) != 0) {
        throw exception(ATLAS_RENDER_ERROR);
    }

}
void Atlas::renderSubTexture(Position<int> const& sub, Position<float> const& p, Position<float> const& orientation, float objectScale, float scale_w, float scale_h) const {
    if (!sub.inRectangle(0, n-1, 0, m-1)) {
        throw exception(ATLAS_OUT_OF_RANGE_RENDER_ERROR);
    }

    SDL_Rect src = {sub.i * sub_w, sub.j * sub_h, sub_w, sub_h};
    SDL_FRect dest = {p.i, p.j, objectScale * scale_w * dimensions.w / n, objectScale * scale_h * dimensions.h / m};
    float angle = getRenderAngle(orientation);
    if (SDL_RenderCopyExF(renderer, texture, &src, &dest, angle, NULL, SDL_FLIP_NONE) != 0) {
        throw exception(ATLAS_RENDER_ERROR);
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
    SDL_FreeSurface(surface);

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
    SDL_FreeSurface(surface);


    if (texture == NULL) {
        throw exception(CONVERTING_SURFACE_TO_TEXTURE_ERROR);
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    dimensions.x = x;
    dimensions.y = y;

    TTF_CloseFont(font);
}