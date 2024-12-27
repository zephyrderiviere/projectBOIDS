#include <SDL2/SDL.h>
#include <exception>
#include <stdexcept>
#include "global.hpp"
#include "utils/ErrorHandling.hpp"




int main() {
    try {

        if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            throw exception(INIT_ERROR);
        }

        global g ("Project BOIDS");
        g.mainLoop();
    } catch (exception e) {
        e.ExitWithError();
    } catch (std::runtime_error e) {
        throw e.what();
    }
    
    return 0;
}