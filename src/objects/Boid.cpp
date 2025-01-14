#include "Boid.hpp"
#include "../utils/ErrorHandling.hpp"
#include "Objects.hpp"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <algorithm>


/*
    Returns the minimum distance between two rectangle
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
    t->renderSubTexture(subTexture, Position<float>(scale_w * (bbox.x - offset.i), scale_h * (bbox.y - offset.j)), speed.normalize(), scale, scale_w, scale_h);
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

    SetColor(renderer, orange);
    if (SDL_RenderDrawLineF(renderer, center.i, center.j, center.i + speed.i, center.j + speed.j) != 0) {
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
    bbox.x += dt * speed.i; 
    bbox.y += dt * speed.j;
    chunk.first = (int) (bbox.x) / CHUNK_SIZE;
    chunk.second = (int) (bbox.y) / CHUNK_SIZE;

}


bool Boid::straightLineIntersect(SDL_FRect const& rect, float angle) const {
    Position<float> vec = speed.rotateTo(angle);
    if (std::abs(vec.i) >= 1e-6) {
        //We check for intersection with vertical sides

        float t = (rect.x - getCenter().i) / vec.i;
        float y = (getCenter() + vec * t).j;
        if (rect.y < y && y < rect.y + rect.h) return true;
        
        t = (rect.x + rect.w - getCenter().i) / vec.i;
        y = (getCenter() + vec * t).j;

        if (rect.y < y && y < rect.y + rect.h) return true;
    }
    if (std::abs(vec.j) >= 1e-6) {
        //We check for intersection with horizontal sides
        
        float t = (rect.y - getCenter().j) / vec.j;
        float x = (getCenter() + vec * t).i;
        if (rect.x < x && x < rect.x + rect.w) return true;

        t = (rect.y + rect.h - getCenter().j) / vec.j;
        x = (getCenter() + vec * t).i;
        if (rect.x < x && x < rect.x + rect.w) return true;
    }

    return false;
}


void Boid::updateRotation(std::vector<std::pair<Object*, float>> const& closeObstacles, WorldSettings const& world) {
    for(std::pair o : closeObstacles) {
        for(float angle = -15.0f; angle < 16.0f; angle += 5.0f) {
            if(straightLineIntersect(o.first->bbox, angle)) {
                speed.rotate((angle > 0 ? -1 : 1) * world.rotSpeed);
                return;
            }
        }
    }
}
        
void Boid::calculateForces(std::vector<Boid*> const& closeBoids, std::vector<std::pair<Object*, float>> const& closeObstacles, WorldSettings const& world) {

    Position<float> boid = getCenter();

    cohesion = Position<float>(0, 0);
    separation = Position<float>(0, 0);
    alignment = Position<float>(0, 0);


    if (!closeBoids.empty()) {
        unsigned verycloseBoids = 0;
        for(Boid* b : closeBoids) {
            if ((boid - b->getCenter()).norm() < world.closeBoidsSeparationLimit) {
                separation += (boid - b->getCenter());
                verycloseBoids++;
            }
            cohesion += (b->getCenter() - boid);
            alignment += b->getSpeed();
        }
        cohesion /= closeBoids.size();
        alignment /= closeBoids.size();
        if (verycloseBoids > 0) separation /= verycloseBoids;
    }

    if (!closeObstacles.empty()) {
        Position<float> separationObstacles;
        for(std::pair o : closeObstacles) {
            if (o.second < world.closeObstaclesLimit / 2) {
                separationObstacles += (boid - o.first->getCenter()) / (std::pow(o.second, 2) / (world.closeObstaclesLimit / 2));
            }
        }
        separation += (separationObstacles / closeObstacles.size());
    }

        

    if (cohesion.norm() < 1e-3f) {
        cohesion = Position<float>(0, 0);
    }
    if (separation.norm() < 1e-3f) {
        separation = Position<float>(0, 0);
    }
    if (alignment.norm() < 1e-3f) {
        alignment = Position<float>(0, 0);
    }
}

void Boid::clampSpeed(WorldSettings const& world) {
    float const norm = speed.norm();

    if (norm < world.minSpeed) {
        speed *= (world.minSpeed / norm);
    }

    if (norm > world.maxSpeed) {
        speed *= (world.maxSpeed / norm);
    }
}

void Boid::updateSpeed(std::vector<Boid*> const& closeBoids, std::vector<std::pair<Object*, float>> const& closeObstacles, WorldSettings const& world) {
    calculateForces(closeBoids, closeObstacles, world);
    separation *= 2.0f;
    Position<float> acceleration = - world.alpha * speed + (cohesion + separation + alignment);

    speed += world.dt * acceleration;

    clampSpeed(world);
}



void LoneBoid::updateSpeed(std::vector<Boid*> const& closeBoids, std::vector<std::pair<Object*, float>> const& closeObstacles, WorldSettings const& world) {
    calculateForces(closeBoids, closeObstacles, world);
    cohesion *= cohesionFactor;
    separation *= separationFactor;
    alignment *= alignmentFactor;

    Position<float> acceleration = - world.alpha * speed + (cohesion + separation + alignment);

    speed += world.dt * acceleration;

    clampSpeed(world);
}


void SociableBoid::updateSpeed(std::vector<Boid*> const& closeBoids, std::vector<std::pair<Object*, float>> const& closeObstacles, WorldSettings const& world) {
    calculateForces(closeBoids, closeObstacles, world);
    cohesion *= cohesionFactor;
    separation *= separationFactor;
    alignment *= alignmentFactor;

    Position<float> acceleration = - world.alpha * speed + (cohesion + separation + alignment);

    speed += world.dt * acceleration;

    clampSpeed(world);
}