#include "Objects.hpp"
#include <SDL2/SDL_render.h>
#include "../utils/ErrorHandling.hpp"






void Rectangle::render(Position<float> const offset, float scale_w, float scale_h) {
    SetColor(renderer, color);
    SDL_FRect r = {scale_w * (bbox.x - offset.i), scale_h * (bbox.y - offset.j), scale_w * bbox.w, scale_h * bbox.h};
    if (SDL_RenderDrawRectF(renderer, &bbox) != 0) {
        throw exception(RECTANGLE_RENDER_ERROR);
    }
}


void Obstacle::render(Position<float> const offset, float scale_w, float scale_h) {
    t.render(Position<float>(scale_w * (bbox.x - offset.i), scale_h * (bbox.y - offset.j)), orientation, scale, scale_w, scale_h);
}


void Boid::render(Position<float> const offset, float scale_w, float scale_h) {
    t.render(Position<float>(scale_w * (bbox.x - offset.i), scale_h * (bbox.y - offset.j)), orientation, scale, scale_w, scale_h);
}

void Boid::updatePosition() {
    Position<float> updated = Position<float>(bbox.x, bbox.y) + speed * orientation;
    bbox.x = updated.i; bbox.y = updated.j;
}
        
void Boid::updateOrientation(Position<float> meanPos) {
    orientation = (orientation + (meanPos - Position<float>(bbox.x, bbox.y))).normalize();
}
/*
a * offset + b = (0, 0)
a * (offset + (screen.w, screen.h)) + b = (w, h)

b = - a * offset
a = (w / screen.w, h / screen.h)

b = - offset * (scale_w, scale_h)
*/