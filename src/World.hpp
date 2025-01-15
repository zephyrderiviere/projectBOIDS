#ifndef WORLD_SETTINGS
#define WORLD_SETTINGS

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

    
    int size;              //Size of the map
    unsigned seed;         //Seed for chunk generation
    int renderDistance;    //Distance in number of chunks that needs to be loaded, that is, the objects are stored in memory
    
    unsigned chunkGen_NewPoints;
    unsigned chunkGen_MinDist;

    WorldSettings() {
        alpha = 0.1f;
        minSpeed = 50.0f;
        maxSpeed = 200.0f;
        rotSpeed = 1.0f;
        
        closeBoidsLimit = 200.0f;
        closeBoidsSeparationLimit = 20.0f; 
        closeObstaclesLimit = 200.0f; 
        inFrontCloseObstaclesLimit = 500.0f;
                
        size = 10000;
        seed = 0;
        renderDistance = 5;

        chunkGen_NewPoints = 30;
        chunkGen_MinDist = 300;
    }
};

#endif //WORLD_SETTINGS