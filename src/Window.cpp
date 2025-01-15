#include "Window.hpp"
#include "utils/ErrorHandling.hpp"
#include "utils/Position.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>




Window::Window(char const* title, SDL_Rect size, Uint32 windowFlags) {
    h = size.h;
    w = size.w;
    
    screen.x = -size.w / 2;
    screen.y = -size.h / 2;
    screen.w = w;
    screen.h = h;
    window = NULL;
    renderer = NULL;
    isFullScreen = false;


    window = SDL_CreateWindow(title, size.x, size.y, size.w, size.h, windowFlags);

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


void Window::modifyScale(float const factor) {
    if ((float) screen.w / w < 0.2f && factor < 1.0f) return;
    if ((float) screen.w / w > 5.0f && factor > 1.0f) return; 
    screen.x += screen.w * 0.5f * (1.0f - factor);
    screen.y += screen.h * 0.5f * (1.0f - factor);
    screen.w *= factor;
    screen.h *= factor;
}

Position<int> Window::toPixel(Position<float> const& p) const {
    Position<int> pixel;
    pixel.i = ((float) w / screen.w) * (p.i - screen.x);
    pixel.j = ((float) h / screen.h) * (p.j - screen.y);
    return pixel;
}

Position<float> Window::toScreenCoords(Position<float> const& p) const {
    Position<float> pixel;
    pixel.i = ((float) w / screen.w) * (p.i - screen.x);
    pixel.j = ((float) h / screen.h) * (p.j - screen.y);
    return pixel;
}

Position<float> Window::toPlaneCoords(Position<int> const& pixel) const {
    Position<float> p;
    p.i = screen.x + pixel.i * (float) screen.w / w;
    p.j = screen.y + pixel.j * (float) screen.h / h;
    return p;
}
