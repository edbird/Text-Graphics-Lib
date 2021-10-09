#ifndef TTY_WRITE_FUNCTIONS_HPP
#define TTY_WRITE_FUNCTIONS_HPP


#include <memory>


#include <SDL2/SDL.h>


#include "sdlfontmanager.hpp"


// TODO: might need to do some perfomance metrics
// and optimization on these functions since they
// are likely to be the most called GUI function
// and probably one potential performance critical
// function call

void write(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    //const SDLFontManager &sdlfontmanager,
    std::shared_ptr<SDLFontTexture> sdlfonttexture,
    //SDL_Renderer &sdlrenderer,
    const char c,
    int &x, const int y,
    const bool advance);


void write_with_background(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    //const SDLFontManager &sdlfontmanager,
    std::shared_ptr<SDLFontTexture> sdlfonttexture,
    //SDL_Renderer &sdlrenderer,
    const char c,
    int &x, const int y,
    const bool advance,
    const SDL_Color &background_color);


void write_string(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    //const SDLFontManager &sdlfontmanager,
    std::shared_ptr<SDLFontTexture> sdlfonttexture,
    //SDL_Renderer &sdlrenderer,
    const std::string &text,
    int &x, const int y,
    const bool advance);


void write_custom_symbol(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    int &x, const int y,
    const bool advance);


void debug_draw_chars_texture(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    std::shared_ptr<SDLFontTexture> sdlfonttexture,
    const int x, const int y);


#endif // TTY_WRITE_FUNCTIONS_HPP