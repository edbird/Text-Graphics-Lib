#ifndef TTY_WRITE_FUNCTIONS_HPP
#define TTY_WRITE_FUNCTIONS_HPP


#include <memory>


#include <SDL2/SDL.h>


#include "sdlfontmanager.hpp"



void write(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    const SDLFontManager &sdlfontmanager,
    //SDL_Renderer &sdlrenderer,
    const char c,
    int &x, const int y,
    const bool advance);


void write_with_background(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    const SDLFontManager &sdlfontmanager,
    //SDL_Renderer &sdlrenderer,
    const char c,
    int &x, const int y,
    const bool advance,
    const SDL_Color &background_color);


void write_string(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    const SDLFontManager &sdlfontmanager,
    //SDL_Renderer &sdlrenderer,
    const std::string &text,
    int &x, const int y,
    const bool advance);




#endif // TTY_WRITE_FUNCTIONS_HPP