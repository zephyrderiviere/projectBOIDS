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
        SDL_FRect bbox;
        float scale;
    
    public:
        Object(SDL_Renderer* renderer) : renderer(renderer), bbox({0, 0, 0, 0}), scale(1.0f) {}
        Object(SDL_Renderer* renderer, Position<float> c, float scale = 1) : renderer(renderer), bbox({c.i, c.j, 0, 0}), scale(scale) {}
        Object(SDL_Renderer* renderer, float x, float y, float w, float h, float scale = 1) : renderer(renderer), bbox({x, y, w, h}), scale(scale) {}
        Object(SDL_Renderer* renderer, SDL_FRect const& r, float scale = 1) : renderer(renderer), bbox({r.x, r.y, r.w, r.h}), scale(scale) {}


        virtual void render(Position<float> const offset, float scale_w = 1, float scale_h = 1) = 0;
        virtual ~Object() {}
};



class Rectangle : public Object {
    private:
        SDL_Color color;

    public:

        Rectangle(SDL_Renderer* renderer) : Object(renderer, 0, 0, 100, 100), color(white) {}
        Rectangle(SDL_Renderer* renderer, SDL_FRect const r, SDL_Color const& c) : Object(renderer, r), color(c) {}
        ~Rectangle() {};

        void render(Position<float> const offset, float scale_w = 1, float scale_h = 1);
};







class Obstacle : public Object {
    private:
        Texture t;
        Position<float> orientation;

    public:

        Obstacle(SDL_Renderer* renderer) : Object(renderer), t() {}
        Obstacle(SDL_Renderer* renderer, Position<float> coords, char const* file, Position<float> orientation, float scale = 1) : 
                Object(renderer, coords, scale), orientation(orientation.normalize()), t(renderer, file) {

            
            bbox.w = t.dimensions.w;
            bbox.h = t.dimensions.h;  
        }

        ~Obstacle() = default;


        void render(Position<float> const offset, float scale_w = 1, float scale_h = 1);

};


class Boid : public Object {
    protected:
        Texture t;
        Position<float> orientation;
        float speed;

    public:

        Boid(SDL_Renderer* renderer, Position<float> p, char const* file, Position<float> orientation, float speed = 1, float scale = 1) : 
                Object(renderer, p, scale), orientation(orientation.normalize()), t(renderer, file), speed(speed) {

            bbox.w = t.dimensions.w;
            bbox.h = t.dimensions.h;
        }
        
        inline Position<float> getCoords() {
            return Position<float> (bbox.x + bbox.w / 2, bbox.y + bbox.h / 2);
        }
        
        void render(Position<float> const offset, float scale_w = 1, float scale_h = 1);
        void updatePosition();
        virtual void updateOrientation(Position<float> meanPos);
};




#endif //OBJECTS