#ifndef SDLFONTTEXTUREMANAGER_HPP
#define SDLFONTTEXTUREMANAGER_HPP


//
//
//  Order of operations:
//
// 1: Load font from file
// 2: Set line skip and line ascent values (global to font)
// 2.5: initialize a vector of characters to be rendered
// 3: call render ASCII characters function
//    3a: creates SDL surface from TTF library
//    3b: creates texture using window renderer
//    3c: sets glyph metrics (unique to each character of the font)
//
// 3c should probably be moved to follow 2
//



// TODO: may not need to maintain state for all error conditions
// since throwing an error should force program to quit unless error is
// handled


#include "exceptiontypes.hpp"



#include <memory>
#include <iostream>
#include <map>



#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>



// manage the actual texture for the font
// a shared pointer to this object is passed around
// and this object should contain a function to render
// a font character to a SDL_Window object, OR
// it should contain helper functions which provide
// the required data for an external function to do
// the rendering


class SDLFontManager;


// TODO: this class does not check to see if the sdl ttf library
// initialized correctly
// if I wanted to have this check, I would either have to pass
// a reference to the sdlmanager to each function which depends
// on the ttf library, or find another solution
//
// possible alteratives might include making this class a member
// of the SDLFontManager class? or something entirely different?
//
// alternatively, this may not be relevant. for example, the
// constructor takes a std::shared_ptr<TTF_Font> object, and
// presumably if the font is valid then the ttf library must
// have initialized successfully
class SDLFontTexture
{


    // required for GetFontAscent()
    // and GetFontLineSkip() functions
    // in SDLFontManager class
    friend class SDLFontManager;


    friend void write(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        //const SDLFontManager &sdlfontmanager,
        std::shared_ptr<SDLFontTexture> sdlfonttexture,
        //SDL_Renderer &sdlrenderer,
        const char c,
        int &x, const int y,
        bool advance);

    
    friend void write_string(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        //const SDLFontManager &sdlfontmanager,
        std::shared_ptr<SDLFontTexture> sdlfonttexture,
        //SDL_Renderer &sdlrenderer,
        const std::string &text,
        int &x, const int y,
        bool advance);


    friend void write_with_background(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        //const SDLFontManager &sdlfontmanager,
        std::shared_ptr<SDLFontTexture> sdlfonttexture,
        //SDL_Renderer &sdlrenderer,
        const char c,
        int &x, const int y,
        bool advance,
        const SDL_Color &background_color);



    public:


    SDLFontTexture(
            const std::shared_ptr<SDL_Renderer> sdlrenderer,
            //const SDLFontManager &sdlfontmanager,
            const std::shared_ptr<TTF_Font> sdlfont,
            const int font_size,
            const int font_line_skip,
            const int font_ascent,
            const std::string &font_chars_string)
        : m_font_texture_init_success(false)
        , m_glyphmetrics_success(false)
        , m_font_render_success(false)
        , m_font_size(font_size)
        , m_font_line_skip(font_line_skip)
        , m_font_ascent(font_ascent)
        // NOTE: store the font size unless it is
        // needed for reference later
        // this value is used when opening the font
        // file and is not actually used by the rendering
        // function
    {
        set_glyph_metrics(
            //sdlfontmanager,
            sdlfont,
            font_ascent,
            font_chars_string);

        render_ascii_chars(
            //sdlfontmanager,
            sdlfont,
            sdlrenderer,
            font_chars_string);
    }

    ~SDLFontTexture()
    {
        map_rendered_chars_advance.clear();
        map_rendered_chars_srect.clear();
        map_rendered_chars_drect.clear();

        // TODO: remove
        if(m_font_texture_init_success)
        {
            m_chars_texture.reset();
        }
    }


    int GetFontAscent() const
    {
        return m_font_ascent;
    }

    int GetFontLineSkip() const
    {
        return m_font_line_skip;
    }



    // TODO: do not return the TTF_Font directly
    /*
    std::weak_ptr<TTF_Font> get() const
    {
        return std::weak_ptr<TTF_Font>(m_font);
    }
    */




    private:


    void set_glyph_metrics(
        //const SDLFontManager &sdlfontmanager,
        const std::shared_ptr<TTF_Font> &sdlfont,
        const int font_ascent,
        const std::string &font_chars_string);
    
    void render_ascii_chars(
        //const SDLFontManager &sdlfontmanager,
        const std::shared_ptr<TTF_Font> &sdlfont,
        const std::shared_ptr<SDL_Renderer> sdlrenderer,
        const std::string &font_chars_string);




    private:


    bool m_font_texture_init_success;

    bool m_glyphmetrics_success;
        // NOTE: needed to set m_font_texture_init_success
    bool m_font_render_success;
        // NOTE: needed to set m_font_texture_init_success


    // these variables remain members of this class, as they may need
    // to be accessed by external functions for actual rendering
    // of texture to SDL_Window object and drawing of texture
    int m_font_size;

    int m_font_line_skip;
    int m_font_ascent;

    //std::string m_chars;
    std::map<char, int> map_rendered_chars_advance;
    std::map<char, SDL_Rect> map_rendered_chars_srect;
    std::map<char, SDL_Rect> map_rendered_chars_drect;
        // rect properties:
        // x, y position of coordinates of char on texture surface
        // w, h size of char on texture surface

    // only the final texture is saved here
    std::shared_ptr<SDL_Texture> m_chars_texture;


};




#endif // SDLFONTTEXTUREMANAGER_HPP