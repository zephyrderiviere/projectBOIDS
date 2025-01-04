#ifndef BOID
#define BOID

#include "Objects.hpp"
#include <vector>
#include "../World.hpp"

class Boid : public Object {
    protected:
        Texture* t;
        Position<float> speed;

        Position<float> cohesion;
        Position<float> separation;
        Position<float> alignment;

    public:

        Boid(SDL_Renderer* renderer, Position<float> const& p, Position<float> const& initSpeed, Texture* texture, float scale = 1) : 
                Object(renderer, p, scale), t(texture), speed(initSpeed) {

            bbox.w = t->dimensions.w * scale;
            bbox.h = t->dimensions.h * scale;
        }
        
        inline Position<float> getSpeed() {
            return speed;
        }
        

        void render(Position<float> const offset, float scale_w = 1, float scale_h = 1) const;
        void renderAccelerations(Position<float> offset, float scale_w, float scale_h) const;

        std::vector<std::pair<Object*, float>> findCloseObstacles(std::vector<Object*> const& stationaryObjects, float const limit);
        std::vector<Boid*> findCloseBoids(std::vector<Boid*> const& boids, float const limit);
        void updatePosition(float const dt);
        void updateSpeed(Position<float> const& meanPos, Position<float> const& meanSpeed, std::vector<Boid*> const& closeBoids, std::vector<std::pair<Object*, float>> const& closeObstacles, WorldSettings const&);
};



#endif //BOID