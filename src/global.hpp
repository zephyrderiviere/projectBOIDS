#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Window.hpp"
#include "objects/Objects.hpp"
#include "utils/ErrorHandling.hpp"
#include "objects/Boid.hpp"
#include "World.hpp"


static char const* file_cloud1 = "data/cloud1.bmp";
static char const* file_WhiteBoid = "data/WhiteBoid.bmp";

static char const* file_fpsFont = "data/Movistar.ttf";

static unsigned const SCANCODES_NUMBER = 513;


class global {
    private:
        Window window;

        bool isRunning;
        SDL_Event e;
        bool isMouseButtonPressed;
        bool isKeyPressed[SCANCODES_NUMBER];

        std::vector<std::pair<std::string, Texture*>> textures;
        

        std::vector<Object*> stationaryObjects;
        std::vector<Boid*> boids;

        WorldSettings world;

        bool isDebug;
        TTF_Font* debugFont;
        int debugFontSize;
        bool renderHitboxes;
        clock_t t;

        bool pause;
        bool freeCam;
        Boid* following;

        void loadTextures();



        Texture* findTexture(std::string const& name) ;

        void handleKeyPresses();
        void handleKeyReleases();
        void handleMouseButtonPresses();
        void handleMouseButtonReleases();
        void handleMouseMotion();
        void handleWindowEvents();

        void renderDebug();
        void renderBBoxes();

    public:
        global(char const* title, SDL_Rect size = {100, 100, 1200, 800}, const char* fpsFontfile = file_fpsFont, unsigned fpsFontSize = 25) 
            : window(title, size), stationaryObjects(), boids() {

            loadTextures();

            
            for(unsigned i=0; i<SCANCODES_NUMBER; i++) {
                isKeyPressed[i] = false;
            }

            isDebug = true;
            renderHitboxes = true;
            isRunning = true;
            isMouseButtonPressed = false;
            debugFont = TTF_OpenFont(fpsFontfile, fpsFontSize);
            debugFontSize = fpsFontSize;

            pause = false;
            freeCam = true;
            following = NULL;

            if (debugFont == NULL) {
                throw exception(LOADING_TEXT_ERROR);
            }
        }
        ~global();

        void updateScene();
        void render();
        void mainLoop();


};