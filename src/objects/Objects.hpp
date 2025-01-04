#ifndef OBJECTS 
#define OBJECTS

#include "../utils/Position.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include "../utils/colors.hpp"
#include "../Texture.hpp"

class Object {
    protected:
        SDL_Renderer* renderer;
        float scale;
    
    public:
        SDL_FRect bbox;

        Object(SDL_Renderer* renderer) : renderer(renderer), bbox({0, 0, 0, 0}), scale(1.0f) {}
        Object(SDL_Renderer* renderer, Position<float> c, float scale = 1) : renderer(renderer), bbox({c.i, c.j, 0, 0}), scale(scale) {}
        Object(SDL_Renderer* renderer, float x, float y, float w, float h, float scale = 1) : renderer(renderer), bbox({x, y, scale * w, scale * h}), scale(scale) {}
        Object(SDL_Renderer* renderer, SDL_FRect const& r, float scale = 1) : renderer(renderer), bbox({r.x, r.y, scale * r.w, scale * r.h}), scale(scale) {}

        inline Position<float> getCenter() const {
            return Position<float>(bbox.x + bbox.w / 2.0f, bbox.y + bbox.h / 2.0f);
        }


        virtual void render(Position<float> const offset, float scale_w = 1, float scale_h = 1) const = 0;
        void renderBBox(Position<float> const offset, float scale_w = 1, float scale_h = 1) const;
        virtual ~Object() {}
};



class Rectangle : public Object {
    private:
        SDL_Color color;

    public:

        Rectangle(SDL_Renderer* renderer) : Object(renderer, 0, 0, 100, 100), color(white) {}
        Rectangle(SDL_Renderer* renderer, SDL_FRect const r, SDL_Color const& c, float const scale = 1) : Object(renderer, r, scale), color(c) {}
        ~Rectangle() {};

        void render(Position<float> const offset, float scale_w = 1, float scale_h = 1) const;
};


class Circle : public Object {
    private:
        SDL_Color color;
        unsigned radius;
        

        

    public:

        Circle(SDL_Renderer* renderer) : Object(renderer, 0, 0, 0, 0), color(black), radius(0) {}
        Circle(SDL_Renderer* renderer, SDL_FRect const r, SDL_Color const& c, int const radius) : Object(renderer, r), color(c), radius(radius) {}
        

        void render(Position<float> const offset, float scale_w = 1, float scale_h = 1) const;
};



class Obstacle : public Object {
    private:
        Texture* t;
        Position<float> orientation;

    public:

        Obstacle(SDL_Renderer* renderer) : Object(renderer), t(NULL) {}
        Obstacle(SDL_Renderer* renderer, Position<float> coords, Texture* texture, Position<float> orientation, float scale = 1) : 
                Object(renderer, coords, scale), orientation(orientation.normalize()), t(texture) {

            
            bbox.w = t->dimensions.w * scale;
            bbox.h = t->dimensions.h * scale;  
        }

        Obstacle(SDL_Renderer* renderer, Position<float> coords, Texture* texture, double orientation, float scale = 1) : 
                Object(renderer, coords, scale), orientation(std::cos(orientation * M_PI / 180.0), std::sin(orientation * M_PI / 180.0)), t(texture) {
        
            bbox.w = t->dimensions.w * scale;
            bbox.h = t->dimensions.h * scale;  
        }
        ~Obstacle() = default;


        void render(Position<float> const offset, float scale_w = 1, float scale_h = 1) const;

};




#endif //OBJECTS