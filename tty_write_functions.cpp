#include "tty_write_functions.hpp"


// TODO: these functions should go in a new C++ file
// move them


// moved from sdlfonttexture.hpp

#include "sdlhelper.hpp"

#include "sdlfonttexture.hpp"


// TODO: these are the autoadvance versions of the functions
// produce some non-autoadvancing functions


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

    int x_copy = x;
    int y_copy = y;

    // get offset as index
    //int offset_x = 0;
    //int index = index_of_printable_char(c);

    // TODO: get the index, and the offset by iterating through
    // the map

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