#include "ErrorHandling.hpp"
#include <iostream>


static char* getMessageFromErrorCode(ErrorCode error) {
    switch (error) {
        case SUCCESS : return (char*)"Everything succeded without errors";
        case INIT_ERROR : return (char*)"Error during initialization of SDL2";
        case WINDOW_CREATION_ERROR : return (char*)"Error during creation of the Window";
        case RENDERER_CREATION_ERROR : return (char*)"Error during creation of the Renderer";
        case TEXTURE_CREATION_ERROR : return (char*)"Error during creation of a Texture";
        case SURFACE_CREATION_ERROR : return (char*)"Error during creation of a Surface";
        case FONT_CREATION_ERROR : return (char*)"Error during creation of a Font";
        
        

        case LOADING_TEXTURE_FILE_ERROR: return (char*)"Error with the loading of a texture file";
        case LOADING_TEXT_ERROR: return (char*)"Error with the loading of a ttf file";
        case CONVERTING_SURFACE_TO_TEXTURE_ERROR: return (char*)"Error while converting a SDL_Surface to a SDL_Texture";

        case COPY_SURFACE_ERROR: return (char*)"Error during the copy of a Surface";

        case PIXEL_RENDER_ERROR : return (char*)"Error during rendering of a pixel";
        case LINE_RENDER_ERROR : return (char*)"Error during rendering of a line";
        case RECTANGLE_RENDER_ERROR : return (char*)"Error during rendering of a Rectangle";
        case CIRCLE_RENDER_ERROR : return (char*)"Error during rendering of a Circle";
        case TEXTURE_RENDER_ERROR : return (char*)"Error during rendering of a Texture";
        case BBOX_RENDER_ERROR : return (char*)"Error during rendering of a BBox";

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