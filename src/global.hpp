#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Window.hpp"
#include "objects/Objects.hpp"
#include "utils/ErrorHandling.hpp"


static char const* file_cloud1 = "data/cloud1.bmp";
static char const* file_WhiteBoid = "data/WhiteBoid.bmp";

static char const* file_fpsFont = "data/Movistar.ttf";


class global {
    private:
        Window window;

        bool isRunning;
        SDL_Event e;
        bool isMouseButtonPressed;
        

        std::vector<Object*> stationaryObjects;
        std::vector<Boid*> boids;

        bool isDebug;
        TTF_Font* debugFont;
        int debugFontSize;
        clock_t t;


        void handleKeyPresses();
        void handleKeyReleases();
        void handleMouseButtonPresses();
        void handleMouseButtonReleases();
        void handleMouseMotion();
        void handleWindowEvents();

        void renderDebug();

    public:
        global(char const* title, SDL_Rect size = {100, 100, 1200, 800}, const char* fpsFontfile = file_fpsFont, unsigned fpsFontSize = 25) 
            : window(title, size), stationaryObjects(), boids() {

            isDebug = true;
            isRunning = true;
            isMouseButtonPressed = false;
            debugFont = TTF_OpenFont(fpsFontfile, fpsFontSize);
            debugFontSize = fpsFontSize;

            if (debugFont == NULL) {
                throw exception(LOADING_TEXT_ERROR);
            }
        }
        ~global();

        void updateScene();
        void render();
        void mainLoop();


};