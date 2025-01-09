#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include "global.hpp"
#include "utils/ErrorHandling.hpp"




int main() {

    srand(time(NULL));
    global* g = NULL;

    try {

        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0 || TTF_Init() != 0) {
            throw exception(INIT_ERROR);
        }

        g = new global("Project BOIDS");
        g->mainLoop();
    } catch (exception e) {
        delete g;
        e.ExitWithError();
    } catch (std::runtime_error e) {
        delete g;
        throw e.what();
    }
    
    return 0;
}