#include "tty_write_functions.hpp"


#include "sdlhelper.hpp"
#include "sdlfonttexture.hpp"


#include <SDL2/SDL.h>


void debug_draw_chars_texture(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    std::shared_ptr<SDLFontTexture> sdlfonttexture,
    const int x, const int y)
{
    //std::cout << __func__ << std::endl;
    
    int width = 0;
    int height = 0;

    // TODO: check return value
    int return_value =
        SDL_QueryTexture(sdlfonttexture->m_chars_texture.get(),
            nullptr, nullptr, &width, &height);

    SDL_Rect rsrc = {0, 0, width, height};
    SDL_Rect rdst = {x, y, width, height};

    //rdst.x += x;
    //rdst.y += y;

    SDL_RenderCopy(
        sdlrenderer.get(),
        sdlfonttexture->m_chars_texture.get(),
        &rsrc, &rdst);
}


// TODO: these are the autoadvance versions of the functions
// TODO: write some non-autoadvancing functions


// documentation notes on argument `advance`
//
// all printable characters have a width, which is stored in the object
// sdlfonttexture->map_rendered_chars_advance
// 

void write(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    std::shared_ptr<SDLFontTexture> sdlfonttexture,
    //SDL_Renderer &sdlrenderer,
    const char c,
    int &x, const int y,
    const bool advance = false)
{

    //std::cout << __func__ << std::endl;

    // this prevents the function trying to copy an invalid character
    // also in the write with background function
    if(sdlfonttexture->map_rendered_chars_glyph_value_valid.at(c) != true) return;

    int x_copy = x;
    int y_copy = y;

    /*std::shared_ptr<SDLFontTexture> sdlfonttexture
        = sdlfontmanager.m_sdlfonttexturemanager;*/

    SDL_Rect rsrc = sdlfonttexture->map_rendered_chars_srect.at(c);
    SDL_Rect rdst = sdlfonttexture->map_rendered_chars_drect.at(c);
    //SDL_Rect rdst = {x, y, rsrc.w, rsrc.h};

    //std::cout << "x=" << x << std::endl;

    rdst.x += x_copy;
    rdst.y += y_copy;

    SDL_RenderCopy(
        sdlrenderer.get(),
        sdlfonttexture->m_chars_texture.get(),
        &rsrc, &rdst);

    const int x_advance =
        sdlfonttexture->map_rendered_chars_advance.at(c);
    x_copy += x_advance;

    //std::cout << "advance=" << advance << std::endl;


    // TODO: how should this map work?
    // what is the most useful data to store?
    // SDL_RenderCopy uses a src_rect
    // so should just store the src_rect
    // the x,y values are the starting position of the char
    // in the source texture
    // and width,height are the width and height of the area
    // to copy with the rendercopy function

    //std::map<char, SDL_Rect> map_rendered_chars_rect;
    //SDL_RenderCopy(renderer, text_texture, &rsrc, &rdst);

    if(advance == true)
    {
        x = x_copy;
    }

}


// TODO: convert SDL Renderer to a smart pointer?

void write_with_background(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    std::shared_ptr<SDLFontTexture> sdlfonttexture,
    //SDL_Renderer &sdlrenderer,
    const char c,
    int &x, const int y,
    const bool advance,
    const SDL_Color &background_color)
{

    // this prevents the function trying to copy an invalid character
    // also in standard write function
    if(sdlfonttexture->map_rendered_chars_glyph_value_valid.at(c) != true) return;

    int x_copy = x;
    int y_copy = y;

    SDL_Rect rsrc = sdlfonttexture->map_rendered_chars_srect.at(c);
    SDL_Rect rdst = sdlfonttexture->map_rendered_chars_drect.at(c);
    //SDL_Rect rdst = {x, y, rsrc.w, rsrc.h};

    rdst.x += x_copy;
    rdst.y += y_copy;

    SDL_SetRenderDrawColor(sdlrenderer.get(), background_color);
    SDL_RenderFillRect(sdlrenderer.get(), &rdst);
    SDL_RenderCopy(
        sdlrenderer.get(),
        sdlfonttexture->m_chars_texture.get(),
        &rsrc, &rdst);

    const int x_advance =
        sdlfonttexture->map_rendered_chars_advance.at(c);
    x_copy += x_advance;

    if(advance == true)
    {
        x = x_copy;
    }
}



void write_string(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    std::shared_ptr<SDLFontTexture> sdlfonttexture,
    //SDL_Renderer &sdlrenderer,
    const std::string &text,
    int &x, const int y,
    const bool advance = false)
{

    int x_copy = x;
    int y_copy = y;

    //int local_x = x;
    for(auto c: text)
    {
        // if advance is true, the changed values of x propagate
        // back through this function and the called function below `write`
        // if advance is false, the changed values of x do not propagate
        // back through this function, however the character printing
        // position is advanced internally to the `write` function (below)
        // otherwise all the characters of the string are printed on
        // top of each other, which makes no sense for a string printing
        // function
        write(sdlrenderer, sdlfonttexture, c, x_copy, y_copy, true);
        //write(sdlrenderer, sdlfontmanager, c, local_x, y);

    /*
        SDL_Rect rsrc = sdlfonttexture->map_rendered_chars_srect.at(c);
        SDL_Rect rdst = sdlfonttexture->map_rendered_chars_drect.at(c);
        //SDL_Rect rdst = {x, y, rsrc.w, rsrc.h};

        rdst.x += local_x;
        rdst.y += y;

        SDL_SetRenderDrawColor(sdlrenderer.get(), background_color);
        SDL_RenderFillRect(sdlrenderer.get(), &rdst);
        SDL_RenderCopy(
            sdlrenderer.get(),
            sdlfonttexture->m_chars_texture.get(),
            &rsrc, &rdst);

        const int advance =
            sdlfonttexture->map_rendered_chars_advance.at(c);
        local_x += advance;
    */
    }

    if(advance == true)
    {
        x = x_copy;
    }
}


// NOTE: this is a bit weird because in order to know the size of the
// rectangle (custom symbol) which should be drawn, the font texture
// is required to query the size of the characters in the font
//
// TODO: would it be better to draw actual font characters for the
// caret symbol? Do the fonts (in general) support some special
// characters for this? If not, I could potentially draw custom
// font symbols and include them in the sdlfonttexture
// This would seem to be the most sensible approach as the
// font texture will store all data for drawing any of the required
// symbols.
//
// Problems: font (at least liberation mono) does not appear to have
// a caret symbol included, and trying to render beyond the standard
// (' ' to '~') ascii character range caused the rendering to break
//
// The difficulty arrises when mapping ascii values to these font
// symbols. Would need an ascii value for a block character. Or a
// vertical line, which doesn't exist (?) in the ascii table.
// May have to extend the data type from "char" to something else.
// At the moment I have used a TextGridElement class to hold the
// expanded data type.
#if 0
void write_custom_symbol(
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    std::shared_ptr<SDLFontTexture> sdlfonttexture,
    //const char c,
    int &x, const int y,
    const bool advance = false)
{
    // this function was never completed?

    int x_copy = x;
    int y_copy = y;

    //SDL_Rect rsrc = sdlfonttexture->map_rendered_chars_srect.at(c);
    SDL_Rect rdst = sdlfonttexture->map_rendered_chars_drect.at(c);
    //SDL_Rect rdst = {x, y, rsrc.w, rsrc.h};

    //std::cout << "x=" << x << std::endl;

    rdst.x += x_copy;
    rdst.y += y_copy;

    SDL_RenderCopy(
        sdlrenderer.get(),
        sdlfonttexture->m_chars_texture.get(),
        &rsrc, &rdst);

    const int x_advance =
        sdlfonttexture->map_rendered_chars_advance.at(c);
    x_copy += x_advance;

    if(advance == true)
    {
        x = x_copy;
    }

}
#endif