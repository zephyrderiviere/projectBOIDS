#include "ErrorHandling.hpp"
#include <iostream>


static char* getMessageFromErrorCode(ErrorCode error) {
    switch (error) {
        case SUCCESS : return (char*)"Everything succeded without errors";
        case INIT_ERROR : return (char*)"Error during initialization of SDL2";
        case WINDOW_CREATION_ERROR : return (char*)"Error during creation of the Window";
        case RENDERER_CREATION_ERROR : return (char*)"Error during creation of the Renderer";

        case PIXEL_RENDER_ERROR : return (char*)"Error during rendering of a pixel";
        case LINE_RENDER_ERROR : return (char*)"Error during rendering of a line";
        case RECTANGLE_RENDER_ERROR : return (char*)"Error during rendering of a rectangle";
        case TEXTURE_RENDER_ERROR : return (char*)"Error during rendering of a texture";

        default: return (char*)"Error";
    }

    return (char*)"";
}


char const* exception::what() {
    return getMessageFromErrorCode(error);
}


void exception::ExitWithError() {
    std::string errorMessage = getMessageFromErrorCode(error);

    std::cout << errorMessage << " -> " << SDL_GetError() << std::endl;


    exit(1);    
}