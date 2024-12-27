#include <SDL2/SDL.h>
#include <vector>
#include "Window.hpp"
#include "objects/Objects.hpp"
#include "utils/ErrorHandling.hpp"



class global {
    private:
        Window window;
        bool isRunning;
        SDL_Event e;
        std::vector<Object*> objects;

        void handleKeyPresses();
        void handleKeyReleases();
        void handleMouseEvents();
        void handleWindowEvents();

    public:
        global(char const* title, SDL_Rect size = {100, 100, 1200, 800}) : window(title, size), isRunning(true), objects() {}
        ~global();

        void render();
        void mainLoop();


};