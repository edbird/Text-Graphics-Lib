#ifndef SDLFONTMANAGER_HPP
#define SDLFONTMANAGER_HPP


#include "exceptiontypes.hpp"


#include "sdlmanager.hpp"
#include "sdlfonttexturemanager.hpp"


#include <memory>
#include <iostream>


//
// TODO: there doesn't seem to be much purpose in separating the
// Font Manager and the Font Texture Manager
// however this feature was in place for when multiple textures
// for different rendered font sizes (or possibly colors/styles)
// might be loaded from the same font manager class
//



class SDLFontManager
{


    friend void write(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        const SDLFontManager &sdlfontmanager,
        //SDL_Renderer &sdlrenderer,
        const char c,
        int &x, const int y);

    
    friend void write_string(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        const SDLFontManager &sdlfontmanager,
        //SDL_Renderer &sdlrenderer,
        const std::string &text,
        int &x, const int y);


    friend void write_with_background(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        const SDLFontManager &sdlfontmanager,
        //SDL_Renderer &sdlrenderer,
        const char c,
        int &x, const int y,
        const SDL_Color &background_color);



    public:

    SDLFontManager()
        : m_font_manager_init_success(false)
    {
    }

    SDLFontManager(
        const SDLManager &sdlmanager,
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        const std::string &font_path,
        const int font_size)
        : m_font_manager_init_success(false)
        ////, m_font_properties_success(false)
        //, m_font_chars_init_success(false)
        ////, m_font_render_success(false)
        ////, m_chars_render_texture_success(false)
        ////, m_glyphmetrics_success(false)
        //, m_font_size(font_size)
    {
        font_manager_init(
            sdlmanager, sdlrenderer,
            font_path, font_size);
    }

    // TODO come back and check this later but this should work
    SDLFontManager(SDLFontManager &&other) noexcept
        : m_sdlfonttexturemanager(std::move(other.m_sdlfonttexturemanager))
        , m_font_manager_init_success(std::move(other.m_font_manager_init_success))
    {

    }

    SDLFontManager& operator=(SDLFontManager&& other)
    {
        m_sdlfonttexturemanager = other.m_sdlfonttexturemanager;
        m_font_manager_init_success = other.m_font_manager_init_success;

        other.m_font_manager_init_success = false;

        return *this;
    }

    ~SDLFontManager()
    {
        // TODO: not actually needed, since going out of scope
        // will delete the shared pointer
        font_manager_destroy();
    }

    int GetFontAscent() const
    {
        return m_sdlfonttexturemanager->m_font_ascent;
    }

    int GetFontLineSkip() const
    {
        return m_sdlfonttexturemanager->m_font_line_skip;
    }

/*
    std::weak_ptr<SDLFontTextureManager>
    GetSDLFontTextureManager()
    {
        return std::weak_ptr<SDLFontTextureManager>(
            m_sdlfonttexturemanager); // TODO: cast to weak?
    }
*/


    void font_manager_init(
        const SDLManager &sdlmanager,
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        const std::string &font_path,
        const int font_size);


    void font_manager_destroy()
    {
        // TODO: this function should be called once the objects
        //      std::shared_ptr<TTF_Font> m_font;
        //      std::unique_ptr<SDL_Surface> m_chars_surface;
        // are finished with 
        

        if(m_font_manager_init_success)
        {
            m_font_manager_init_success = false;
            m_sdlfonttexturemanager.reset(); // not really needed
        }
    }

    bool FontManagerInitSuccess() const;


    // private functions

    private:


    // create a string containing the characters to render
    std::string init_font_chars_string()
    {
        //m_font_chars_string_init_success = true;

        // add characters to be rendered to string
        std::string font_chars_string;
        for(char c = ' '; c <= '~'; ++ c)
        {
            font_chars_string.push_back(c);
        }

        return font_chars_string;
    }



    ////////////////////////////////////////////////////////////////////////////
    // private data members
    ////////////////////////////////////////////////////////////////////////////


    private:

    const int DEFAULT_FONT_SIZE = 12;

    bool m_font_manager_init_success;
    /*bool m_font_init_success;
    bool m_chars_render_success;
    bool m_chars_render_texture_success; // TODO: don't need to store all of these
    bool m_glyphmetrics_success;*/
    // the purpose of this class is to load a font from file and render it to
    // a texture - external objects only need to know if that whole process
    // was successful
    // throw exceptions depending on the specific function call which failed
    // however do not maintain the state of which function call failed
    // internally


    // shared pointer, this object is returned to functions
    // which need to access the SDL_Texture and associated
    // data (such as font size, glyph metrics)
    std::shared_ptr<SDLFontTextureManager> m_sdlfonttexturemanager;

    // map rendered font sizes to shared pointers to font texture manager
    // objects - this permits multiple font sizes to be rendered
    /*
    std::map
    <
        float?,
        std::shared_ptr<SDLFontTextureManager>
    >
        m_map_fonttexturemanager;
    */
    // TODO: too complex, removed
};

#endif // SDLFONTMANAGER_HPP