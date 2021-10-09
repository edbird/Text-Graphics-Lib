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


    friend void debug_draw_chars_texture(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        std::shared_ptr<SDLFontTexture> sdlfonttexture,
        const int x, const int y);


    public:


    SDLFontTexture(
            const std::shared_ptr<SDL_Renderer> sdlrenderer,
            //const SDLFontManager &sdlfontmanager,
            const std::shared_ptr<TTF_Font> sdlfont,
            const int font_size,
            ////const int font_line_skip,
            ////const int font_ascent,
            /*const std::string &font_chars_string*/
            std::string &valid_ascii_font_chars_string) // the valid (renderable) characters are returned via this string object - TODO change to wide string?
        : m_font_texture_init_success(false)
        , m_glyphmetrics_success(false)
        , m_font_render_success(false)
        , m_font_size(font_size)
        ////, m_font_line_skip(font_line_skip)
        ////, m_font_ascent(font_ascent) // note removed, as code has been moved inside this constructor to query these values
        , m_font_line_skip(0)
        , m_font_ascent(0)
        // NOTE: store the font size unless it is
        // needed for reference later
        // this value is used when opening the font
        // file and is not actually used by the rendering
        // function
    {

        /*
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
        */

        /*
        std::string valid_font_chars_string =
            check_ascii_chars(
                sdlfont,
                font_chars_string);
        */

        // moved from sdlfontmanager::LoadFontTexture()
        // NOTE: presumably these cannot fail
        /*const int*/ m_font_line_skip = TTF_FontLineSkip(/*m_font*/ sdlfont.get());
        /*const int*/ m_font_ascent = TTF_FontAscent(/*m_font*/ sdlfont.get());
        // TODO: these should be moved INSIDE the SDLFontTexture
        // constructor - it is pointless to retain these variables
        // here as they are not used by this function or this class

        valid_ascii_font_chars_string =
            init_valid_font_chars_string(sdlfont);

        // DEBUG
        std::cout << "valid_ascii_font_chars_string="
                  << valid_ascii_font_chars_string
                  << std::endl;

        set_glyph_metrics(
            //sdlfontmanager,
            sdlfont,
            m_font_ascent,
            valid_ascii_font_chars_string);

        render_ascii_chars(
            //sdlfontmanager,
            sdlfont,
            sdlrenderer,
            valid_ascii_font_chars_string);

        /*
        std::string font_chars_string_request = font_chars_string;

        // TODO: this function
        set_glyph_metrics_safe(
            sdlfont,
            font_ascent,
            font_chars_string_request);

        render_ascii_chars_safe(
            sdlfont,
            sdlrenderer,
            font_chars_string_request);
        */
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

    std::string check_ascii_chars(
        const std::shared_ptr<TTF_Font> &sdlfont,
        const std::string &requested_font_chars_string);

    std::string init_valid_font_chars_string(std::shared_ptr<TTF_Font> sdlfont);


    int GetFontAscent() const
    {
        return m_font_ascent;
    }

    int GetFontLineSkip() const
    {
        return m_font_line_skip;
    }


    int GetWidestCharacterAdvance() const
    {
        int width_max = -1;
        for(const auto& it: map_rendered_chars_advance)
        {
            int width = it.second;

            // DEBUG
            if(width > width_max)
            {
                std::cout << "new widest character found (advance): " << it.first
                          << " width=" << it.second << std::endl;
            }

            width_max = std::max(width_max, width);
        }
        return width_max;
    }

    // return the font advance, calculated from the width
    // of the "W" character, which should be the largest
    // character in the font set (probably)
    // TODO: probably change the name to match the below function
    int GetCharacterWidthW() const
    {
        // can use either map_rendered_chars_srect or
        // map_rendered_chars_drect here
        //int width = map_rendered_chars_drect
        int width = map_rendered_chars_srect.at('W').w;
        return width;
    }

    int GetCharacterAdvanceW() const
    {
        // can use either map_rendered_chars_srect or
        // map_rendered_chars_drect here
        //int width = map_rendered_chars_drect
        int width = map_rendered_chars_advance.at('W');
        return width;
    }

    int GetCharacterWidthUnderscore() const
    {
        // can use either map_rendered_chars_srect or
        // map_rendered_chars_drect here
        //int width = map_rendered_chars_drect
        int width = map_rendered_chars_srect.at('_').w;
        return width;
    }

    int GetCharacterAdvanceUnderscore() const
    {
        // can use either map_rendered_chars_srect or
        // map_rendered_chars_drect here
        //int width = map_rendered_chars_drect
        int width = map_rendered_chars_advance.at('_');
        return width;
    }


    // return the widest character
    int GetWidestCharacterWidth() const
    {
        int width_max = -1;
        for(const auto& it: map_rendered_chars_srect)
        {
            int width = it.second.w;

            // DEBUG
            if(width > width_max)
            {
                std::cout << "new widest character found: " << it.first
                          << " width=" << it.second.w << std::endl;
            }

            width_max = std::max(width_max, width);
        }
        return width_max;
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

    // a function which fills the valid characters map
    // map_rendered_chars_glyph_value_valid
    void render_ascii_chars_safe(
        const std::shared_ptr<TTF_Font> &sdlfont,
        const std::shared_ptr<SDL_Renderer> sdlrenderer,
        const std::string &font_chars_string_request);



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

    // problem with trying to render characters outside of the range
    // ' ' to '~'
    // different fonts may have different ranges of renderable characters
    // and these are not known in advance, but can be determined using
    // the function call TTF_GlyphIsProvided()
    //
    // this is a bit of hack - but provides a way to determine if a
    // glyph is valid
    //
    // the process is as follows: use the function TTF_GlyphIsProvided()
    // to determine if a glyph is valid. If it is, register this in the
    // valid glyph values map, and add the character value code to the
    // string which is to be rendered. Then render that string.

    //TTF_GlyphIsProvided
    std::map<char, bool> map_rendered_chars_glyph_value_valid;


};




#endif // SDLFONTTEXTUREMANAGER_HPP