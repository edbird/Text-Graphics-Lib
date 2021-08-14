#ifndef SDLFONTMANAGER_HPP
#define SDLFONTMANAGER_HPP


#include "errortypes.hpp"


#include "sdlmanager.hpp"
#include "sdlfonttexturemanager.hpp"


#include <memory>
#include <iostream>



class SDLFontManager
{


    public:

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


    ~SDLFontManager()
    {
        // TODO: not actually needed, since going out of scope
        // will delete the shared pointer
        font_manager_destroy();
    }


    std::weak_ptr<SDLFontTextureManager>
    GetSDLFontTextureManager()
    {
        return std::weak_ptr<SDLFontTextureManager>(
            m_sdlfonttexturemanager); // TODO: cast to weak?
    }



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