#include "Boid.hpp"
#include "../utils/ErrorHandling.hpp"
#include "Objects.hpp"
#include <vector>


/*
    Returns the minimum distance between two rectangle points
*/
float minDistance(SDL_FRect const* r1, SDL_FRect const* r2) {
    bool left = (r2->x + r2->w) < r1->x;
    bool right = (r1->x + r1->w) < r2->x;
    bool bottom = (r1->y + r1->h) < r2->y;
    bool top = (r2->y + r2->h) < r1->y;

    if (left && top) {
        return (Position<float>(r1->x, r1->y) - Position<float>(r2->x + r2->w, r2->y + r2->h)).norm();
    } else if (left && bottom) {
        return (Position<float>(r1->x, r1->y + r1->h) - Position<float>(r2->x + r2->w, r2->y)).norm();
    } else if (right && bottom) {
        return (Position<float>(r1->x + r1->w, r1->y + r1->h) - Position<float>(r2->x, r2->y)).norm();
    } else if (right && top) {
        return (Position<float>(r1->x + r1->w, r1->y) - Position<float>(r2->x, r2->y + r2->h)).norm();
    } else if (right) {
        return r2->x - (r1->x + r1->w);
    } else if (left) {
        return r1->x - (r2->x + r2->w);
    } else if (top) {
        return r1->y - (r2->y + r2->h);
    } else if (bottom) {
        return r2->y - (r1->y + r1->h);
    } else {
        return 0.0f;
    }
}


void Boid::render(Position<float> const offset, float scale_w, float scale_h) const {
    t->render(Position<float>(scale_w * (bbox.x - offset.i), scale_h * (bbox.y - offset.j)), speed.normalize(), scale, scale_w, scale_h);
}

void Boid::renderAccelerations(Position<float> const offset, float scale_w, float scale_h) const {
    Position<float> center = getCenter() - offset;
    center.i *= scale_w; center.j *= scale_h;

    SetColor(renderer, green);
    if (SDL_RenderDrawLineF(renderer, center.i, center.j, center.i + cohesion.i, center.j + cohesion.j) != 0) {
        throw exception(LINE_RENDER_ERROR);
    }
    SetColor(renderer, magenta);
    if (SDL_RenderDrawLineF(renderer, center.i, center.j, center.i + separation.i, center.j + separation.j) != 0) {
        throw exception(LINE_RENDER_ERROR);
    }
    SetColor(renderer, black);
   
    if (SDL_RenderDrawLineF(renderer, center.i, center.j, center.i + alignment.i, center.j + alignment.j) != 0) {
        throw exception(LINE_RENDER_ERROR);
    }  
}

std::vector<std::pair<Object*, float>> Boid::findCloseObstacles(std::vector<Object*> const& stationaryObjects, float const limit) {
    std::vector<std::pair<Object*, float>> closeObjects;

    for(Object* o : stationaryObjects) {
        float minDist = minDistance(&bbox, &o->bbox);
        if (minDist < limit) {
            closeObjects.push_back(std::pair(o, std::max(minDist, 1e-3f)));
        }
    }
    return closeObjects;
}
std::vector<Boid*> Boid::findCloseBoids(std::vector<Boid*> const& boids, float const limit) {
    std::vector<Boid*> closeBoids;
    for(Boid* b : boids) {
        if (this == b) continue;
        float minDist = minDistance(&bbox, &b->bbox);
        if (minDist < limit) {
            closeBoids.push_back(b);
        }
    }
    return closeBoids;
}

void Boid::updatePosition(float const dt) {
    Position<float> updated = Position<float>(bbox.x, bbox.y) + dt * speed;
    bbox.x = updated.i; bbox.y = updated.j;
}
        
void Boid::updateSpeed(Position<float> const& meanPos, Position<float> const& meanSpeed, std::vector<Boid*> const& closeBoids, std::vector<std::pair<Object*, float>> const& closeObstacles, WorldSettings const& world) {
    Position<float> acceleration = - world.alpha * speed;

    Position<float> boid = getCenter();

    cohesion = Position<float>(0, 0);
    separation = Position<float>(0, 0);
    alignment = Position<float>(0, 0);


    if (!closeBoids.empty()) {
        Position<float> separationBoids;
        for(Boid* b : closeBoids) {
            separationBoids += (boid - b->getCenter());
        }
        separation = (separationBoids / closeBoids.size());
    }

    if (!closeObstacles.empty()) {
        Position<float> separationObstacles;
        for(std::pair o : closeObstacles) {
            separationObstacles += (o.first->getCenter()) / (std::pow(o.second, 2) / world.closeLimit);
        }
        separation += (boid - separationObstacles / closeObstacles.size());
    }

    cohesion = (meanPos - boid);
    alignment = meanSpeed;
        

    if (cohesion.norm() < 1e-3f) {
        cohesion = Position<float>(0, 0);
    }
    if (separation.norm() < 1e-3f) {
        separation = Position<float>(0, 0);
    }
    if (alignment.norm() < 1e-3f) {
        alignment = Position<float>(0, 0);
    }


    acceleration += (cohesion + separation + alignment);

    speed += world.dt * acceleration;


    // SPEED LIMIT
    float const norm = speed.norm();
    if (norm > world.maxSpeed) {
        speed *= (world.maxSpeed / norm);
    }
}