#ifndef ERRORHANDLING
#define ERRORHANDLING
#include <SDL2/SDL.h>
#include <exception>

enum ErrorCode {
    SUCCESS,
    INIT_ERROR,
    WINDOW_CREATION_ERROR,
    RENDERER_CREATION_ERROR,
    TEXTURE_CREATION_ERROR,
    SURFACE_CREATION_ERROR,
    FONT_CREATION_ERROR,


    LOADING_TEXTURE_FILE_ERROR,
    LOADING_TEXT_ERROR,
    CONVERTING_SURFACE_TO_TEXTURE_ERROR,

    COPY_SURFACE_ERROR,

    PIXEL_RENDER_ERROR,
    LINE_RENDER_ERROR,
    RECTANGLE_RENDER_ERROR,
    CIRCLE_RENDER_ERROR,
    TEXTURE_RENDER_ERROR,
    BBOX_RENDER_ERROR,
};



class exception : public std::exception {
    ErrorCode error;

    public:
        exception(ErrorCode e = SUCCESS) : error(e) {}


        char const* what();

        void ExitWithError();    


};



#endif //ERRORHANDLING