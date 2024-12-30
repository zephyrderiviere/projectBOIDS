#include "global.hpp"
#include "objects/Objects.hpp"
#include "utils/Position.hpp"
#include "utils/colors.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <ctime>


global::~global() {
    for(Object* o : stationaryObjects) {
        delete o;
    }
    for(Boid* b : boids) {
        delete b;
    }
    TTF_CloseFont(debugFont);
    SDL_Quit();
}



void global::handleKeyPresses() {
    switch(e.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
            isRunning = false;
            break;

        case SDL_SCANCODE_A: {
            stationaryObjects.push_back(new Rectangle(window.renderer, {100, 100, 100, 100}, white));
            break;
        }

        case SDL_SCANCODE_C: {
            Position<float> coord(rand() % window.screen.w, rand() % window.screen.h);
            Position<float> orientation(2, 4);
            stationaryObjects.push_back(new Obstacle(window.renderer, coord, file_cloud1, orientation, 0.1f));
            break;
        }

        case SDL_SCANCODE_B: {
            Position<float> coord(rand() % window.screen.w, rand() % window.screen.h);
            boids.push_back(new Boid(window.renderer, coord, file_WhiteBoid, Position<float>(1, 0), 1.0f, 0.1f));
            break;
        }

        case SDL_SCANCODE_F3: {
            isDebug = !isDebug;
            break;
        }

        case SDL_SCANCODE_O: {
            window.modifyScale(0.9f);
            break;
        }

        case SDL_SCANCODE_P: {
            window.modifyScale(1.0f / 0.9f);
            break;
        }
        


        default: break;
    }
}

void global::handleMouseButtonPresses() {
    isMouseButtonPressed = true;
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
    if (boids.empty()) return;

    Position<float> meanPos;

    for(Boid* b : boids) {
        meanPos += b->getCoords();
    }
    meanPos = meanPos / boids.size();

    for(Boid* b : boids) {
        b->updateOrientation(meanPos);
        b->updatePosition();
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





void global::render() {

    window.setDrawColor(black);
    SDL_RenderClear(window.renderer);
    SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND);

    Position<float> const offset (window.screen.x, window.screen.y);
    float scale_w = (float) window.w / window.screen.w, scale_h = (float) window.h / window.screen.h;

    for(Object* o : stationaryObjects) {
        o->render(offset, scale_w, scale_h);   
    }

    for(Boid* b : boids) {
        b->render(offset, scale_w, scale_h);
    }
    
    SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_NONE);
    if (isDebug) {
        renderDebug();
    }
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
        updateScene();
        render();
    }
}