#include "sdlhelper.hpp"


int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    const SDL_Color &color)
{
    return SDL_SetRenderDrawColor(renderer,
        color.r, color.g, color.b, color.a);
}