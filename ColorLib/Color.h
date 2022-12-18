#ifndef COLOR_H
#define COLOR_H


#include <SDL2/SDL.h>


// TODO: change to use the Color Service Locator which I have written in another project


// default colors
/*
const SDL_Color COLOR_WHITE;
const SDL_Color COLOR_BLACK;
const SDL_Color COLOR_GREEN;
const SDL_Color COLOR_GREY_LIGHT;
const SDL_Color COLOR_RED;

const SDL_Color COLOR_GREY;


// GUI element colors
const SDL_Color COLOR_TEXT_DEFAULT = COLOR_BLACK;
const SDL_Color COLOR_BACKGROUND_DEFAULT = COLOR_GREY_LIGHT;
//const SDL_Color COLOR_BACKGROUND_DEFAULT = COLOR_RED;
*/

// default colors
const SDL_Color COLOR_WHITE { 255, 255, 255, 255 };
const SDL_Color COLOR_BLACK { 0, 0, 0, 255 };
const SDL_Color COLOR_GREEN { 0, 255, 0, 255 };
const SDL_Color COLOR_GREY_LIGHT { 220, 220, 220, 255 };
const SDL_Color COLOR_RED { 255, 0, 0, 255 };

const SDL_Color COLOR_GREY { 128, 128, 128, 255 };


// GUI element colors
const SDL_Color COLOR_TEXT_DEFAULT = COLOR_BLACK;
const SDL_Color COLOR_BACKGROUND_DEFAULT = COLOR_GREY_LIGHT;

#endif // COLOR_H