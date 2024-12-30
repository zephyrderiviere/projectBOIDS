#ifndef TEXTURE
#define TEXTURE

#include "utils/Position.hpp"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

class Texture {
    public:
        SDL_Renderer* renderer;

        SDL_Texture* texture;
        SDL_Rect dimensions;


        Texture() : renderer(NULL), texture(NULL), dimensions({0, 0, 0, 0}) {}
        Texture(SDL_Texture* texture, SDL_Rect dims) : texture(texture), dimensions(dims) {}
        Texture(SDL_Renderer* renderer, char const* filepath, int const x = 0, int const y = 0);
        Texture(Texture const& other);

        Texture& operator=(Texture const& other);

        ~Texture();


        void render() const;
        void render(Position<int> p) const;
        void render(Position<float> p, float objectScale = 1, float scale_w = 1, float scale_h = 1) const;
        void render(Position<float> p, Position<float> orientation, float objectScale = 1, float scale_w = 1, float scale_h = 1) const;

};


class Atlas : public Texture {
    public:
        unsigned n, m; //Number of sub-textures per row / column

};


class Text : public Texture {
    public:


        Text(SDL_Renderer* renderer, char const* text, TTF_Font* font, SDL_Color const& color, int const x = 0, int const y = 0);
        Text(SDL_Renderer* renderer, char const* text, char const* font_filepath, unsigned size, SDL_Color const& color, int const x = 0, int const y = 0);
};


#endif //TEXTURE