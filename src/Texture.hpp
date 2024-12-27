#ifndef TEXTURE
#define TEXTURE

#include "utils/Position.hpp"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

class Texture {
    public:
        SDL_Texture* texture;
        SDL_Rect dimensions;


        void render(SDL_Renderer* renderer) const;
        void render(SDL_Renderer* renderer, Position<int> p);

};


class Atlas : public Texture {
    public:
        unsigned n, m; //Number of sub-textures per row / column

};


#endif //TEXTURE