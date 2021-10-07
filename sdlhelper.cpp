#include "sdlhelper.hpp"


/*int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    const SDL_Color &color)
{
    return SDL_SetRenderDrawColor(renderer,
        color.r, color.g, color.b, color.a);
}*/

// non const version
int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    SDL_Color color)
{
    return SDL_SetRenderDrawColor(renderer,
        color.r, color.g, color.b, color.a);
}


SDL_Color SDL_MakeColor(const int r, const int g, const int b)
{
    const SDL_Color color {r, g, b, 255};
    return color;
}