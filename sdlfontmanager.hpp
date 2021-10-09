#ifndef SDLFONTMANAGER_HPP
#define SDLFONTMANAGER_HPP


#include "exceptiontypes.hpp"


#include "sdlmanager.hpp"
#include "sdlfonttexture.hpp"


#include "fc_helper.hpp"


#include <memory>
#include <iostream>


#include "fontloadproperties.hpp"



// TODO: implement the swap ideom here


// this class loads instances of SDLFontTexture into a map which
// is indexed using unique keys generated from the font filename
// and the font size
//
// TODO: font colors, styles (?)
class SDLFontManager
{

// TODO: note these are no longer needed here, no member variables
// accessed
/*
    friend void write(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        //const SDLFontManager &sdlfontmanager,
        std::shared_ptr<SDLFontTexture> sdlfonttexture,
        //SDL_Renderer &sdlrenderer,
        const char c,
        int &x, const int y,
        const bool advance);

    
    friend void write_string(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        //const SDLFontManager &sdlfontmanager,
        std::shared_ptr<SDLFontTexture> sdlfonttexture,
        //SDL_Renderer &sdlrenderer,
        const std::string &text,
        int &x, const int y,
        const bool advance);


    friend void write_with_background(
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        //const SDLFontManager &sdlfontmanager,
        std::shared_ptr<SDLFontTexture> sdlfonttexture,
        //SDL_Renderer &sdlrenderer,
        const char c,
        int &x, const int y,
        const bool advance,
        const SDL_Color &background_color);
*/


    // Note: The field
    // m_font_manager_init_success
    // does NOT track whether the actual FontTexture class initialized
    // successfully
    // it only tracks whether memory for the FontTextureManager class
    // was allocated successfully.
    //
    // NOTE: this has now changed, see below
    //
    // Therefore at the moment it is pointless.
    // It would make more sense if it was intialized only if the
    // FontTextureManager initialized successfully, but in this case
    // it might as well be a member variable of the FontTextureManager
    // class, which makes this class completely redundant, unless it is used
    // to supply functions to load different sizes of font or different
    // types of font.
    //
    // NOTE: it is now used to prevent passing of the sdlmanager to each
    // member function. now the variable is used to track is sdlmanager
    // initialized correctly
    //
    // NOTE: this is not how this class works, it is how the other class
    // works (sdlresourcemanager)
    //
    // Should the fc font helper functions be made members of this class?
    // Probably not, but this class should probably use them to load
    // fonts.
    // Fonts should be able to change color and size.
    // Provide a map to a font type? And a map to a font size?
    // How to handle colors?
    // NOTE: this is now managed by the fontloadproperties class
    // Perhaps the FontTextureManager class should provide a SetColor
    // function which can set the foreground color and background color
    // if one is provided (ie: not clear color).


    public:

    SDLFontManager(const SDLManager &sdlmanager)
        : m_sdl_ttf_init_success(false)
    {
        if(sdlmanager.TTFInitSuccess())
        {
            // this variable is checked in all functions which
            // require the sdl ttf library to be initialized
            // correctly before said function can be called
            m_sdl_ttf_init_success = true;
        }
        else
        {
            throw TTFLibException("Error in SDLFontManager(), TTF library previously failed to initialize");
        }
    }


    // TODO come back and check this later but this should work
    SDLFontManager(SDLFontManager &&other) noexcept = default;
    /*
        : m_sdl_ttf_init_success(std::move(other.m_sdl_ttf_init_success))
        , map_font_texture(std::move(other.map_font_texture))
    {
    }
    */


    // TODO: use swap function
    SDLFontManager& operator=(SDLFontManager&& other) noexcept = default;
    /*
    {
        m_sdl_ttf_init_success = other.m_sdl_ttf_init_success;
        map_font_texture = other.map_font_texture;

        other.m_sdl_ttf_init_success = false;

        return *this;
    }
    */


    ~SDLFontManager()
    {
        if(m_sdl_ttf_init_success == true)
        {
            m_sdl_ttf_init_success = false;

            map_font_texture.clear();
            // not actually needed, since going out of scope
            // will delete the shared pointers and the map
        }
    }

    // pass heavy argument by const reference, becase
    // we will only access the data read only
/*    int GetFontAscent(const FontLoadProperties& fontloadproperties) const
    {
    }*/

    // TODO: should the font ascent and font line skip
    // variables be part of the FontTexture class or
    // part of the SDLFontManager class?
    // Note: They are already part of the SDLFontManager
    // class so this is a non issue

    // TODO: probably remove these interface exposing functions




    // To figure out how to pass the arguments, need to know
    // how the function we call works
    // NOTE: done
    void LoadFontTexture(
        //const SDLManager &sdlmanager,
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        const std::string& font_full_path,
        const int font_size
        , std::string &valid_ascii_font_chars_string
        //SDL_Color font_color
        );

    
    // This function is the same as the LoadFontTexture function
    // but with an additional step of converting a font description
    // to a filename
    std::string LoadFontTextureFromDescription(
        //const SDLManager &sdlmanager,
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        const std::string& font_filename_search_string,
        const int font_size
        , std::string &valid_ascii_font_chars_string
        //SDL_Color font_color
        );


    std::shared_ptr<SDLFontTexture>
    GetFontTexture(
        const std::string& font_full_path,
        const int font_size) const;


    bool TTFLibInitSuccess() const;


    // private functions

    private:


    // TODO: update this function
    // create a string containing the characters to render
    /*std::string init_font_chars_string() const;*/
    // Notes: This function is used from sdlfontmanager.cpp
    //
    // this function has been removed: 2021-10-07
    // it was confusing to leave in since it is not used



    ////////////////////////////////////////////////////////////////////////////
    // private data members
    ////////////////////////////////////////////////////////////////////////////


    private:

    const int DEFAULT_FONT_SIZE = 12;

    // TODO: remove this?
    bool m_sdl_ttf_init_success;



    // map a unique key produced by FontLoadProperties class
    // to a std::shared_ptr of SDLFontTexture
    // this object is returned to functions
    // which need to access the SDL_Texture and associated
    // data (such as font size, glyph metrics)
    std::map<std::string, std::shared_ptr<SDLFontTexture>> map_font_texture;
};

#endif // SDLFONTMANAGER_HPP

