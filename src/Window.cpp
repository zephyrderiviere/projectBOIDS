#include "Window.hpp"
#include "utils/ErrorHandling.hpp"
#include <SDL2/SDL_render.h>




Window::Window(char const* title, SDL_Rect size) {
    screen.h = size.h;
    screen.w = size.w;
    window = NULL;
    renderer = NULL;


    window = SDL_CreateWindow(title, size.x, size.y, size.w, size.h, SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        throw exception(WINDOW_CREATION_ERROR);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if (renderer == NULL) {
        throw exception(RENDERER_CREATION_ERROR);
    }
}


Window::~Window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}


Window const& Window::operator=(Window const& other) {
    if (this == &other) {
        return *this;
    }

    renderer = other.renderer;
    window = other.window;
    screen = other.screen;

    return *this;
}

