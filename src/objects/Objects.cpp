#include "Objects.hpp"
#include <SDL2/SDL_render.h>
#include "../utils/ErrorHandling.hpp"





void Rectangle::render(SDL_Renderer* renderer) {
    SDL_Rect r = {coords.i, coords.j, width, height};
    SetColor(renderer, color);
    if (SDL_RenderDrawRect(renderer, &r) != 0) {
        throw exception(RECTANGLE_RENDER_ERROR);
    }
}