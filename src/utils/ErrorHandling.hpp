#ifndef ERRORHANDLING
#define ERRORHANDLING
#include <SDL2/SDL.h>
#include <exception>
#include <string>

enum ErrorCode {
    SUCCESS,
    INIT_ERROR,
    WINDOW_CREATION_ERROR,
    RENDERER_CREATION_ERROR,

    PIXEL_RENDER_ERROR,
    LINE_RENDER_ERROR,
    RECTANGLE_RENDER_ERROR,
    TEXTURE_RENDER_ERROR,
};



class exception : public std::exception {
    ErrorCode error;

    public:
        exception(ErrorCode e = SUCCESS) : error(e) {}


        char const* what();

        void ExitWithError();    


};



#endif //ERRORHANDLING