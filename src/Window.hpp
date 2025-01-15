#ifndef WINDOW
#define WINDOW


#include "utils/Position.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_error.h>

const int x_window_offset = 100;
const int y_window_offset = 100;





/*
    Window class that contains the SDL_Window and SDL_Renderer assigned to it as well as a few other useful variables and methods.

    The screen rect is used to represents the snapshot of the infinite plane the simulation is held upon captured by the window and showed on the screen ; 
    whereas the w and h variables show the real values (in pixels) of the window size and height.
*/
class Window {
    public:
        
        SDL_Window* window;
        SDL_Rect screen;
        int w, h;
        bool isFullScreen;
    
        SDL_Renderer* renderer;

        void modifyScale(float const factor);

        Window () : window(NULL), renderer(NULL) {}        
        Window(char const* title, SDL_Rect size, Uint32 windowFlags);
        ~Window();

        Window const& operator=(Window const& other);

        
        inline void setDrawColor(SDL_Color const& c) const {
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        }

        Position<int> toPixel(Position<float> const& p) const;
        Position<float> toScreenCoords(Position<float> const& p) const;
        Position<float> toPlaneCoords(Position<int> const& pixel) const;

};



#endif //WINDOW