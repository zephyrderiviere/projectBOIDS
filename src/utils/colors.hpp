#ifndef COLORS
#define COLORS


#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

/***********************TERMINAL COLOURS**********************/

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


/***********************WINDOW COLOURS***********************/


static const SDL_Color blueButton = {100, 100, 255, 255};
static const SDL_Color white = {255, 255, 255, 255};
static const SDL_Color black = {0, 0, 0, 255};
static const SDL_Color darkGrey = {100, 100, 100, 255};
static const SDL_Color beige = {227,212,173, 255};
static const SDL_Color blueviolet = {138, 43, 226, 255};
static const SDL_Color magenta = {255, 0, 255, 255};
static const SDL_Color green = {0, 255, 0, 255};


inline void SetColor(SDL_Renderer* renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

#endif //COLORS