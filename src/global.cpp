#include "global.hpp"
#include "Texture.hpp"
#include "World.hpp"
#include "objects/Objects.hpp"
#include "utils/Position.hpp"
#include "utils/colors.hpp"
#include "utils/random.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <queue>


void global::loadTextures() {
    /*Cloud Texture*/
    textures.push_back(std::pair("cloud1", new Texture(window.renderer, file_cloud1)));

    /*Boid texture Atlas TODO*/
    textures.push_back(std::pair("boids", new Atlas(window.renderer, file_WhiteBoid, 1, 1)));
}


void global::makeWorldBorder(float const size) {
    for(int x=-size; x<=size; x += 10) {
        if (std::abs(x) == size) {
            for(int y=-size; y<size; y+=10) {
                stationaryObjects.push_back(new Rectangle(window.renderer, {(float) x, (float) y, 1, 10}, blueviolet));
            }
        }
        if (x != size) {
            stationaryObjects.push_back(new Rectangle(window.renderer, {(float) x, -size, 10, 1}, blueviolet));
            stationaryObjects.push_back(new Rectangle(window.renderer, {(float) x, +size, 10, 1}, blueviolet));
        }
    }
}

Texture* global::findTexture(std::string const& name) {
    for(std::pair t : textures) {
        if (t.first == name) return t.second;
    }

    return NULL;
}

global::~global() {
    for(Object* o : stationaryObjects) {
        delete o;
    }
    for(Boid* b : boids) {
        delete b;
    }
    for(std::pair t : textures) {
        delete t.second;
    }

    TTF_CloseFont(debugFont);
    SDL_Quit();
}

bool global::notInNeighbourHood(Position<float> const& point, std::list<Position<float>> const& samplePoints, unsigned const cellSize) const {
    for (Position<float> p : samplePoints) {
        if ((p - point).norm() < world.chunkGen_MinDist || ((p.i / cellSize) == (point.i / cellSize)) && ((p.j / cellSize) == (point.j / cellSize))) {
            return false;
        }
    }
    return true;
}

//Generates a chunk (by default a 1000x1000 zone) using the Poisson Disk Sampling method
void global::generateChunk(std::pair<int, int> const& chunk) {   
    randomGenerator tea (chunk);

    unsigned const cellSize = CHUNK_SIZE / 10;
    std::queue<Position<float>> toProcess;
    std::list<Position<float>> samplePoints;
    Position<float> p (tea.randomFloat(CHUNK_SIZE * chunk.first, CHUNK_SIZE * (chunk.first + 1)), 
                       tea.randomFloat(CHUNK_SIZE * chunk.second, CHUNK_SIZE * (chunk.second + 1)));
    toProcess.push(p); samplePoints.push_back(p);

    while(!toProcess.empty()) {
        Position<float> point = toProcess.front();
        toProcess.pop();
        for(int i=0; i<world.chunkGen_NewPoints; i++) {
            Position<float> rand = point + tea.randomDiskSampling(world.chunkGen_MinDist, 2.0f * world.chunkGen_MinDist);

            if (rand.inRectangle(CHUNK_SIZE * chunk.first, CHUNK_SIZE * (chunk.first + 1), CHUNK_SIZE * chunk.second, CHUNK_SIZE * (chunk.second + 1))
                && notInNeighbourHood(rand, samplePoints, cellSize)) {

                toProcess.push(rand);
                samplePoints.push_back(rand);
            }
        }
    }

    for(Position<float>& p : samplePoints) {
        stationaryObjects.push_back(new Obstacle(window.renderer, p, findTexture("cloud1"), tea.randomFloat(0, 360.0f), 0.1f));
    }
}

void global::unloadChunk(std::pair<int, int> const& chunk) {
    for(auto iter = stationaryObjects.begin(); iter != stationaryObjects.end(); iter++) {
        Object* o = *(iter.base());
        if (o->chunk == chunk) {
            delete o;
            stationaryObjects.erase(iter);
            iter--;
        }
    }
}



void global::handleKeyPresses() {
    isKeyPressed[e.key.keysym.scancode] = true;
    switch(e.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
            isRunning = false;
            break;

        case SDL_SCANCODE_Q: {
            stationaryObjects.push_back(new Rectangle(window.renderer, {100, 100, 100, 100}, white));
            break;
        }

        case SDL_SCANCODE_C: {
            Position<float> coord(window.screen.x + rand() % window.screen.w, window.screen.y + rand() % window.screen.h);
            double rotation = rand() / 360.0;
            stationaryObjects.push_back(new Obstacle(window.renderer, coord, findTexture("cloud1"), rotation, 0.1f));
            break;
        }

        case SDL_SCANCODE_B: {
            if (isKeyPressed[SDL_SCANCODE_F3]) {
                renderHitboxes = !renderHitboxes;
            } else {
                Position<float> coord(window.screen.x + rand() % window.screen.w, window.screen.y + rand() % window.screen.h);
                Atlas* boidsAtlas = dynamic_cast<Atlas*>(findTexture("boids"));
                boids.push_back(new Boid(window.renderer, coord, Position<float>(10, 0), boidsAtlas, 0.2f));
            }
            break;
        }

        case SDL_SCANCODE_E: {
            generateChunk(std::pair(0, 0));
            break;
        }

        case SDL_SCANCODE_R: {
            unloadChunk(std::pair(0, 0));
            break;
        }

        case SDL_SCANCODE_F: {
            freeCam = true;
            following = NULL;
            break;
        }

        case SDL_SCANCODE_F3: {
            isDebug = !isDebug;
            break;
        }

        case SDL_SCANCODE_K: {
            window.modifyScale(0.9f);
            break;
        }

        case SDL_SCANCODE_L: {
            window.modifyScale(1.0f / 0.9f);
            break;
        }

        case SDL_SCANCODE_SPACE: {
            pause = !pause;
            break;
        }
        


        default: break;
    }
}

void global::handleKeyReleases() {
    isKeyPressed[e.key.keysym.scancode] = false;

}

void global::handleMouseButtonPresses() {
    isMouseButtonPressed = true;

    Position<int> mouse;
    SDL_GetMouseState(&mouse.i, &mouse.j);
    Position<float> mousePlaneCoords = window.toPlaneCoords(mouse);

    if (e.button.button == SDL_BUTTON_LEFT) {
        for (Boid* o : boids) {
            if (mousePlaneCoords.inRectangle(
                    o->bbox.x, o->bbox.x + o->bbox.w,
                    o->bbox.y, o->bbox.y + o->bbox.h)) {

                freeCam = false;
                following = o;
                break;
            }
        }
    }
}

void global::handleMouseButtonReleases() {
    isMouseButtonPressed = false;
}

void global::handleMouseMotion() {
    if (isMouseButtonPressed) {
        int vx = e.motion.xrel, vy = e.motion.yrel;

        window.screen.x -= vx * ((float)window.screen.w) / window.w;
        window.screen.y -= vy * ((float)window.screen.h) / window.h;;
    }
}


void global::updateScene() {
    Position<float> const offset (window.screen.x, window.screen.y);
    float scale_w = (float) window.w / window.screen.w, scale_h = (float) window.h / window.screen.h;

    if (boids.empty()) return;


    world.dt = (float) (clock() - t) / CLOCKS_PER_SEC;

    for(Boid* b : boids) {
        std::vector<Boid*> closeBoids = b->findCloseBoids(boids, world.closeBoidsLimit);
        auto closeObstacles = b->findCloseObstacles(stationaryObjects, world.closeObstaclesLimit);
        b->updateSpeed(closeBoids, closeObstacles, world);
        b->updateRotation(closeObstacles, world);
        b->updatePosition(world.dt);
    }

    if (!freeCam) {
        window.screen.x = following->bbox.x + (following->bbox.w - window.screen.w) / 2;
        window.screen.y = following->bbox.y + (following->bbox.h - window.screen.h) / 2;
    }
}


void global::renderDebug() {
    clock_t t2 = clock();

    double renderTime = (double) (t2 - t) / CLOCKS_PER_SEC;
    double fps = 1 / renderTime;
    t = t2;
    char s[100];
    sprintf(s, "FPS : %lf", fps);

    Text fpsText(window.renderer, s, debugFont, white);
    fpsText.render();

    sprintf(s, "Top left = (%d, %d)", window.screen.x, window.screen.y);

    Text coordsText(window.renderer, s, debugFont, white);
    coordsText.render(Position<int>(0, debugFontSize));

    Position<int> center(window.screen.x + window.screen.w / 2, window.screen.y + window.screen.h / 2);


    sprintf(s, "Center = (%d, %d)", center.i, center.j);

    Text centerText(window.renderer, s, debugFont, white);
    centerText.render(Position<int>(0, 2 * debugFontSize));

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    sprintf(s, "Mouse = (%d, %d)", mouseX, mouseY);

    Text mouseText(window.renderer, s, debugFont, white);
    mouseText.render(Position<int>(0, 4 * debugFontSize));
}

void global::renderBBoxes() {
    SetColor(window.renderer, red);
    Position<float> const offset (window.screen.x, window.screen.y);
    float scale_w = (float) window.w / window.screen.w, scale_h = (float) window.h / window.screen.h;
    SDL_FRect screen = {(float) window.screen.x, (float) window.screen.y, (float) window.screen.w, (float) window.screen.h};


    for(Object* o : stationaryObjects) {
        if (SDL_HasIntersectionF(&o->bbox, &screen)) {
            o->renderBBox(offset, scale_w, scale_h);
        }
    }
    for(Boid* b : boids) {
        if (SDL_HasIntersectionF(&b->bbox, &screen)) {
            b->renderBBox(offset, scale_w, scale_h);
        }
    }
}





void global::render() {

    window.setDrawColor(skyBlue);
    SDL_RenderClear(window.renderer);
    SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND);

    Position<float> const offset (window.screen.x, window.screen.y);
    float scale_w = (float) window.w / window.screen.w, scale_h = (float) window.h / window.screen.h;

    SDL_FRect screen = {(float) window.screen.x, (float) window.screen.y, (float) window.screen.w, (float) window.screen.h};

    for(Object* o : stationaryObjects) {
        if (SDL_HasIntersectionF(&o->bbox, &screen)) {
            o->render(offset, scale_w, scale_h);   
        }
    }

    for(Boid* b : boids) {
        if (SDL_HasIntersectionF(&b->bbox, &screen)) {
            b->render(offset, scale_w, scale_h);
        }
    }

    if (renderHitboxes) {
        renderBBoxes();
        for(Boid* b : boids) {
            if (SDL_HasIntersectionF(&b->bbox, &screen)) {
                b->renderAccelerations(offset, scale_w, scale_h);
            }
        }
    }
    
    if (isDebug) {
        renderDebug();
    }

    SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_NONE);
    SDL_RenderPresent(window.renderer);
}


void global::mainLoop() {
    t = clock();
    while(isRunning) {
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT: 
                    isRunning = false;
                    break;

                case SDL_KEYDOWN: 
                    handleKeyPresses(); 
                    break;
                
                case SDL_KEYUP:
                    handleKeyReleases();
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    handleMouseButtonPresses();
                    break;

                case SDL_MOUSEBUTTONUP:
                    handleMouseButtonReleases();
                    break;

                case SDL_MOUSEMOTION:
                    handleMouseMotion();
                    break;

            }
        }
        if (!pause) updateScene();
        render();
    }
}