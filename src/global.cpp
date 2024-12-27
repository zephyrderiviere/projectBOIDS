#include "global.hpp"
#include "objects/Objects.hpp"
#include "utils/colors.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>


global::~global() {
    for(Object* o : objects) {
        delete o;
    }
    SDL_Quit();
}

void global::handleKeyPresses() {
    switch(e.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
            isRunning = false;
            break;

        case SDL_SCANCODE_A: {
            objects.push_back(new Rectangle({100, 100, 100, 100}, white));
        }

        


        default: break;
    }
}


void global::render() {

    window.setDrawColor(black);
    SDL_RenderClear(window.renderer);

    for(Object* o : objects) {
        o->render(window.renderer);   
    }
    
    SDL_RenderPresent(window.renderer);
}


void global::mainLoop() {

    while(isRunning) {
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT: 
                    isRunning = false; 
                    break;

                case SDL_KEYDOWN: 
                    handleKeyPresses(); 
                    break;
            }
        }
        render();
    }
}