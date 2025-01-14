#ifndef TEXTURE
#define TEXTURE

#include "utils/ErrorHandling.hpp"
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
        Texture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect dims) : renderer(renderer),texture(texture), dimensions(dims) {}
        Texture(SDL_Renderer* renderer, char const* filepath, int const x = 0, int const y = 0);
        Texture(Texture const& other);

        Texture& operator=(Texture const& other);

        virtual ~Texture();


        void render() const;
        void render(Position<int> const& p) const;
        void render(Position<float> const& p, float objectScale = 1, float scale_w = 1, float scale_h = 1) const;
        void render(Position<float> const& p, Position<float> const& orientation, float objectScale = 1, float scale_w = 1, float scale_h = 1) const;

};


class Atlas : public Texture {
    public:
        unsigned n, m; //Number of sub-textures per row / column
        int sub_w, sub_h;


        Atlas() : Texture(), n(0), m(0) {}
        Atlas(SDL_Renderer* renderer, char const* filepath, int const n, int const m, int const x=0, int const y=0) : 
                    Texture(renderer, filepath, x, y), n(n), m(m) {
            
            if (dimensions.w % n != 0 || dimensions.h % m != 0) {
                SDL_DestroyTexture(texture);
                throw exception(ATLAS_SUBTEXTURE_SIZE_CREATION_ERROR);
            }
            sub_w = dimensions.w / n;
            sub_h = dimensions.h / m;
        }

        ~Atlas() override = default;

        void renderSubTexture(Position<int> const& sub) const;
        void renderSubTexture(Position<int> const& sub, Position<int> const& p) const;
        void renderSubTexture(Position<int> const& sub, Position<float> const& p, float objectScale = 1, float scale_w = 1, float scale_h = 1) const;
        void renderSubTexture(Position<int> const& sub, Position<float> const& p, Position<float> const& orientation, float objectScale = 1, float scale_w = 1, float scale_h = 1) const;
};


class Text : public Texture {
    public:


        Text(SDL_Renderer* renderer, char const* text, TTF_Font* font, SDL_Color const& color, int const x = 0, int const y = 0);
        Text(SDL_Renderer* renderer, char const* text, char const* font_filepath, unsigned size, SDL_Color const& color, int const x = 0, int const y = 0);

        ~Text() override = default;
};


#endif //TEXTURE