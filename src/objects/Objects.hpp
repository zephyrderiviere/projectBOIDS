#ifndef OBJECTS 
#define OBJECTS

#include "../utils/Position.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include "../utils/colors.hpp"
#include "../Texture.hpp"

class Object {
    protected:
        Position<int> coords;
    
    public:
        Object() : coords() {}
        Object(int i, int j) : coords(i, j) {}

        virtual void render(SDL_Renderer* renderer) = 0;
        virtual ~Object() {}
};



class Rectangle : public Object {
    private:
        int width, height;
        SDL_Color color;

    public:

        Rectangle() : Object(), width(100), height(100), color(white) {}
        Rectangle(SDL_Rect& r, SDL_Color& c) : Object(r.x, r.y), width(r.w), height(r.h), color(c) {}
        Rectangle(SDL_Rect r, SDL_Color c) : Object(r.x, r.y), width(r.w), height(r.h), color(c) {}
        ~Rectangle() {};

        void render(SDL_Renderer* renderer);
};







class Obstacle : public Object {
    private:
        Texture t;
};


class Boid : public Object {

};




#endif //OBJECTS