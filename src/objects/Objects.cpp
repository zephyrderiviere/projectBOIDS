#include "Objects.hpp"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <cassert>
#include "../utils/ErrorHandling.hpp"


void Object::renderBBox(Position<float> const offset, float scale_w, float scale_h) const {
    SDL_FRect r = {scale_w * (bbox.x - offset.i), scale_h * (bbox.y - offset.j), scale_w * bbox.w, scale_h * bbox.h};

    if (SDL_RenderDrawRectF(renderer, &r) != 0) {
        throw exception(BBOX_RENDER_ERROR);
    }
}


void Rectangle::render(Position<float> const offset, float scale_w, float scale_h) const {
    SetColor(renderer, color);
    SDL_FRect r = {scale_w * (bbox.x - offset.i), scale_h * (bbox.y - offset.j), scale_w * bbox.w, scale_h * bbox.h};
    if (SDL_RenderDrawRectF(renderer, &r) != 0) {
        throw exception(RECTANGLE_RENDER_ERROR);
    }
}



void Circle::render(Position<float> const offset, float scale_w, float scale_h) const {

	Position<float> center = getCenter() - offset;
    center.i *= scale_w;
    center.j *= scale_h;
	
	
	SetColor(renderer, color);
    int a = radius;
    int b = 0;
    	int da = 1;
    	int db = 1;
    	int err = da - (radius << 1);

    while (a >= b) {
       	if (
        SDL_RenderDrawPoint(renderer, center.i + a, center.j + b) ||
       	SDL_RenderDrawPoint(renderer, center.i + b, center.j + a) ||
       	SDL_RenderDrawPoint(renderer, center.i - b, center.j + a) ||
       	SDL_RenderDrawPoint(renderer, center.i - a, center.j + b) ||
       	SDL_RenderDrawPoint(renderer, center.i - a, center.j - b) ||
       	SDL_RenderDrawPoint(renderer, center.i - b, center.j - a) ||
       	SDL_RenderDrawPoint(renderer, center.i + b, center.j - a) ||
       	SDL_RenderDrawPoint(renderer, center.i + a, center.j - b) != 0) {
            throw exception(CIRCLE_RENDER_ERROR);
        }
    
        if (err <= 0){
      	    b++;
          	err += db;
           	db += 2;
       	}
       	if (err > 0){
           	a--;
           	da += 2;
           	err += da - (radius << 1);
       	}
	}
}


void Obstacle::render(Position<float> const offset, float scale_w, float scale_h) const {
    t->render(Position<float>(scale_w * (bbox.x - offset.i), scale_h * (bbox.y - offset.j)), orientation, scale, scale_w, scale_h);
}