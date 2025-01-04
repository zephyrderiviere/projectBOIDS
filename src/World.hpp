#ifndef WORLD_SETTINGS
#define WORLD_SETTINGS


#include <cstdio>


struct WorldSettings {
    float alpha;        //Air resistance
    float dt;           //Delta time
    float maxSpeed;     //Speed limit
    float closeLimit;   //Limit to consider other objects as "close" to a boid

    WorldSettings(float const alpha = 0.1, float const dt = 0.01f, float maxSpeed = 100.0f, float closeLimit = 20.0f) : 
        alpha(alpha), dt(dt), maxSpeed(maxSpeed), closeLimit(closeLimit) {}
};

#endif //WORLD_SETTINGS