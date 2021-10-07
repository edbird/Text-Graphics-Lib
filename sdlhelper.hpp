#ifndef SDLRENDERER_HPP
#define SDLRENDERER_HPP


#include <SDL2/SDL.h>


/*int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    const SDL_Color &color);*/


// non-const version
int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    SDL_Color color);


SDL_Color SDL_MakeColor(const int r, const int g, const int b);



#endif // SDLHELPER_HPP