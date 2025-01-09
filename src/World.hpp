#ifndef WORLD_SETTINGS
#define WORLD_SETTINGS


#include <cstdio>

static int const CHUNK_SIZE = 1000;


struct WorldSettings {
    float alpha;        //Air resistance
    float dt;           //Delta time
    float minSpeed;     //Speed lower bound
    float maxSpeed;     //Speed upper bound
    float rotSpeed;     //Rotation speed of the boids (in degrees)


    float closeBoidsLimit;   //Limit to consider other objects as "close" to a boid
    float closeBoidsSeparationLimit;
    float closeObstaclesLimit;
    float inFrontCloseObstaclesLimit;

    
    unsigned size;              //Size of the map
    unsigned renderDistance;    //Distance in number of chunks that needs to be loaded, that is, the objects are stored in memory
    
    unsigned const chunkGen_NewPoints;
    unsigned const chunkGen_MinDist;

    WorldSettings(float const alpha = 0.1f, float const dt = 0.01f, float maxSpeed = 500.0f, float rotation = 1.0f, 
                  float closeBoidsLimit = 50.0f, float closeBoidsSeparationLimit = 20.0f, float closeObstaclesLimit = 100.0f, float inFrontCloseObstaclesLimit = 500.0f,
                  unsigned s = 10000, unsigned renderDistance = 5) : 

        alpha(alpha), dt(dt), maxSpeed(maxSpeed), rotSpeed(rotation), 
        closeBoidsLimit(closeBoidsLimit), closeBoidsSeparationLimit(closeBoidsSeparationLimit), closeObstaclesLimit(closeObstaclesLimit), inFrontCloseObstaclesLimit(inFrontCloseObstaclesLimit),
        size(s), renderDistance(renderDistance),
        chunkGen_NewPoints(30),
        chunkGen_MinDist(150)
        {}
};

#endif //WORLD_SETTINGS