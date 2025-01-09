#ifndef BOID
#define BOID

#include "Objects.hpp"
#include <vector>
#include "../World.hpp"

class Boid : public Object {
    protected:
        Atlas* t;
        Position<int> subTexture;
        Position<float> speed;
        int rotationDirection;

        Position<float> cohesion;
        Position<float> separation;
        Position<float> alignment;


        bool straightLineIntersect(SDL_FRect const& rect) const;

        void calculateForces(std::vector<Boid*> const& closeBoids, std::vector<std::pair<Object*, float>> const& closeObstacles, WorldSettings const&);
        void limitSpeed(WorldSettings const&);

    public:

        Boid(SDL_Renderer* renderer, Position<float> const& p, Position<float> const& initSpeed, Atlas* texture, float scale = 1) : 
                Object(renderer, p, scale), t(texture), speed(initSpeed) {
        
            bbox.w = t->dimensions.w * scale;
            bbox.h = t->dimensions.h * scale;
            subTexture = Position<int>(0, 0);
            rotationDirection = ((rand() % 2) == 0) ? -1 : 1;
        }
        
        inline Position<float> getSpeed() const {
            return speed;
        }
        

        void render(Position<float> const offset, float scale_w = 1, float scale_h = 1) const;
        void renderAccelerations(Position<float> offset, float scale_w, float scale_h) const;

        std::vector<std::pair<Object*, float>> findCloseObstacles(std::vector<Object*> const& stationaryObjects, float const limit);
        std::vector<Boid*> findCloseBoids(std::vector<Boid*> const& boids, float const limit);
        void updatePosition(float const dt);
        void updateRotation(std::vector<std::pair<Object*, float>> const& stationaryObjects, WorldSettings const& world);
        virtual void updateSpeed(std::vector<Boid*> const& closeBoids, std::vector<std::pair<Object*, float>> const& closeObstacles, WorldSettings const&);
};


class LoneBoid : public Boid {
    private:
        static float constexpr cohesionFactor = 0.8f;
        static float constexpr separationFactor = 10.0f;
        static float constexpr alignmentFactor = 1.0f;

    public:

        void updateSpeed(std::vector<Boid*> const& closeBoids, std::vector<std::pair<Object*, float>> const& closeObstacles, WorldSettings const&);
};



#endif //BOID