#include "Texture.hpp"
#include <SDL2/SDL_render.h>
#include "utils/ErrorHandling.hpp"



void Texture::render(SDL_Renderer* renderer) const {
    if (SDL_RenderCopy(renderer, texture, NULL, &dimensions) != 0) {
        throw exception(TEXTURE_RENDER_ERROR);
    }
}