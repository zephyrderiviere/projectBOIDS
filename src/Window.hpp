#ifndef WINDOW
#define WINDOW


#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_error.h>

const int x_window_offset = 100;
const int y_window_offset = 100;






class Window {
    public:
        
        SDL_Window* window;
        SDL_Rect screen;
    
        SDL_Renderer* renderer;

        Window () : window(NULL), renderer(NULL) {}        
        Window(char const* title, SDL_Rect size);
        ~Window();

        Window const& operator=(Window const& other);

        
        inline void setDrawColor(SDL_Color const& c) const {
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        }

};



#endif //WINDOW