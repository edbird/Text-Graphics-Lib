#ifndef SDLFONTMANAGER_HPP
#define SDLFONTMANAGER_HPP


#include "exceptiontypes.hpp"


#include "sdlmanager.hpp"
#include "sdlfonttexture.hpp"


#include "fc_helper.hpp"


#include <memory>
#include <iostream>


class FontLoadProperties
{


    public:

    FontLoadProperties(
        std::string font_file_path,
        //std::string font_load_description,
        const int font_size
    )
        : font_file_path(std::move(font_file_path))
        //, font_load_description(std::move(font_load_description))
        , font_size(font_size)
    {
        build_unique_key();
    }

#if 0
    // usage examples in main function
    {
        FontLoadProperties properties(
            // this argument is a temporary
            // the std::string constructor will be called with this argument,
            // the string will be constructed and then std::move'd to the
            // class variable font_file_path
            "/home/ecb/fonts/arial.ttf",
            // this argument is a temporary
            // the std::string constructor will be called with this argument
            // (is that true when the type is a const reference?)
            // the string will then be constructed (?) and then copied to the
            // class variable font_load_description
            // this is SLOWER than the above argument implementation
            // (I think)
            "ariel.ttf",
            12);
        // therefore the first argument is faster and the second argument
        // is slower ?

        // another usage example
        std::string font_path("/home/ecb/fonts/arial.ttf");
        std::string font_description("ariel.ttf");

        FontLoadProperties properties(font_path, font_description, 12);
        // this version:
        // first argument is copied, then move'd (one slow operation, one fast)
        // second argument is passed as a reference (fast operation) and then
        // copied (slow operation)
        // so in this case, either implementation is basically the same speed?
    }
#endif

    std::string GetUniqueKey() const
    {
        return unique_key;
    }

    private:

    void build_unique_key()
    {
        // a font is uniquely identified by a full path to
        // a file and a rendered font size
        // the description used to search for a match to
        // a file name is not included, because this may be
        // non unique and multiple values may match to the
        // same filename
        unique_key = font_file_path + std::to_string(font_size);
    }

    // the full path to the loaded font
    std::string font_file_path;

    // the string description passed to font config to
    // find the font to load, based on the string matching
    ////std::string font_load_description;
    // removed, because not relevant

    // rendered size of font
    int font_size;

    // TODO: color?
    //SDL_Color font_color;

    // some unique key which describes the font
    std::string unique_key;


};


//
// TODO: there doesn't seem to be much purpose in separating the
// Font Manager and the Font Texture Manager
// however this feature was in place for when multiple textures
// for different rendered font sizes (or possibly colors/styles)
// might be loaded from the same font manager class
//

// TODO: implement the swap ideom here

class SDLFontManager
{


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



    // Note: The field
    // m_font_manager_init_success
    // does NOT track whether the actual FontTexture class initialized
    // successfully
    // it only tracks whether memory for the FontTextureManager class
    // was allocated successfully.
    //
    // Therefore at the moment it is pointless.
    // It would make more sense if it was intialized only if the
    // FontTextureManager initialized successfully, but in this case
    // it might as well be a member variable of the FontTextureManager
    // class, which makes this class completely redundant, unless it is used
    // to supply functions to load different sizes of font or different
    // types of font.
    //
    // Should the fc font helper functions be made members of this class?
    // Probably not, but this class should probably use them to load
    // fonts.
    // Fonts should be able to change color and size.
    // Provide a map to a font type? And a map to a font size?
    // How to handle colors?
    // Perhaps the FontTextureManager class should provide a SetColor
    // function which can set the foreground color and background color
    // if one is provided (ie: not clear color).


    public:

    SDLFontManager(const SDLManager &sdlmanager)
        : m_font_manager_init_success(false)
    {
        if(sdlmanager.TTFInitSuccess())
        {
            // TODO: added this here
            // TODO: remove this variable from elsewhere (it might be
            // set somewhere else)
            // TODO: check this variable in other functions, for example
            // the LoadFont... functions
            m_font_manager_init_success = true;
        }
        else
        {
            throw TTFLibException("Error in SDLFontManager(), TTF library previously failed to initialize");
        }
    }


// only one constructor needed now
#if 0
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
        // TODO: remove the font_manager_init function ?
        // or only have this code in the font_manager_init function?
        if(sdlmanager.TTFInitSuccess())
        {
            // TODO: added this here
            // TODO: remove this variable from elsewhere (it might be
            // set somewhere else)
            // TODO: check this variable in other functions, for example
            // the LoadFont... functions
            m_font_manager_init_success = true;
        }
        else
        {
            throw TTFLibException("Error in font_init(), TTF library previously failed to initialize");
        }

        /*
        font_manager_init(
            sdlmanager, sdlrenderer,
            font_path, font_size);*/
    }
    // TODO: remove this constructor, call with blank constructor
#endif

    // TODO come back and check this later but this should work
    SDLFontManager(SDLFontManager &&other) noexcept
        : m_font_manager_init_success(std::move(other.m_font_manager_init_success))
        , map_font_texture(std::move(other.map_font_texture))
    {
    }


    SDLFontManager& operator=(SDLFontManager&& other)
    {
        map_font_texture = other.map_font_texture;
        m_font_manager_init_success = other.m_font_manager_init_success;

        other.m_font_manager_init_success = false;

        return *this;
    }


    ~SDLFontManager()
    {
        // TODO: not actually needed, since going out of scope
        // will delete the shared pointer
        font_manager_destroy();
        // TODO: no longer relevant? remove?
    }

    // pass heavy argument by const reference, becase
    // we will only access the data read only
/*    int GetFontAscent(const FontLoadProperties& fontloadproperties) const
    {
        std::shared_ptr<SDLFontTexture> fonttexture =
            map_font_texture.at(fontloadproperties);
            // will throw exception if this does not exist
            // inside the map

        //return m_sdlfonttexturemanager->m_font_ascent;
    }*/

    // TODO: should the font ascent and font line skip
    // variables be part of the FontTexture class or
    // part of the SDLFontManager class?
    // Note: They are already part of the SDLFontManager
    // class so this is a non issue

    // TODO: probably remove these interface exposing functions
/*    int GetFontLineSkip() const
    {
        return m_sdlfonttexturemanager->m_font_line_skip;
    }*/

/*
    std::weak_ptr<SDLFontTexture>
    GetSDLFontTextureManager()
    {
        return std::weak_ptr<SDLFontTexture>(
            m_sdlfonttexturemanager); // TODO: cast to weak?
    }
*/

    // To figure out how to pass the arguments, need to know
    // how the function we call works
    void LoadFontTexture(
        //const SDLManager &sdlmanager,
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        const std::string& font_full_path,
        const int font_size
        //SDL_Color font_color
        )
    {



        // TODO: this is a temporary check in case the fc functions
        // fail to find a filename for an appropriate font
        // TODO: check the fc function return values, in case of error
        if(font_full_path.size() == 0)
            throw std::runtime_error("font_full_path.size() == 0");


        // there are two choices here:
        // either convert the arguments to the FontLoadProperties
        // structure here and use as key
        // the input arguments to this function are regular
        // variables, not a structure of type FontLoadProperties
        // the alternative is to pass the arguments in the
        // structure FontLoadProperties

        // here we implement the first option
        FontLoadProperties fontloadproperties(
            //font_full_path, "", font_size);
            font_full_path, font_size);
            // the description is irrelevant / ignored
            // TODO: when the LoadFontTextureFromDescription
            // function is called, the font_filename_search_string
            // argument is not propagated to this function
            // could just remove that field from FontLoadProperties

        // TODO: perform this check first, before bothering
        // to do a heavyweight operation of loading the texture
        std::string unique_key(fontloadproperties.GetUniqueKey());
        
        // check if key exists in map first, as this
        // is a fast operation, and what follows is slow
        if(map_font_texture.find(unique_key) ==
            map_font_texture.cend())
        {
            // key does not exist in map



            // TODO: these shared pointers were members of the
            // SDLFontManager class but they do not need to be saved
            // for future use, therefore move them to be local variables
            // such that storage will be freed after use

            // the TTF_Font object is not retained by SDLFontTexture
            // it is used to initialize the font texture
            // it is used by the functions:
            //      render_ascii_chars
            //      set_glyph_metrics
            //
            std::shared_ptr<TTF_Font> m_font;
            // TODO: m_chars_surface is not used here, remove?
            //std::unique_ptr<SDL_Surface> m_chars_surface;

            // this is also a lightweight operation
            // arguably this check should come before the other
            if(m_font_manager_init_success == true)
            {
                m_font.reset(TTF_OpenFont(
                    font_full_path.c_str(),
                    font_size),
                    TTF_CloseFont);

                if(m_font.get() != nullptr)
                {

                    // NOTE: presumably these cannot fail
                    const int font_line_skip = TTF_FontLineSkip(m_font.get());
                    const int font_ascent = TTF_FontAscent(m_font.get());

                    // this block is just to wrap the font_chars_string
                    // object
                    {
                        // TODO: const here?
                        const std::string font_chars_string =
                            init_font_chars_string();

                        std::shared_ptr<SDLFontTexture> sdlfonttexture;

                        // create the FontTextureManager
                        sdlfonttexture.reset(
                            new SDLFontTexture(
                                sdlrenderer,
                                //*this,
                                m_font,
                                font_size,
                                font_line_skip,
                                font_ascent,
                                font_chars_string));

                        if(sdlfonttexture.get() != nullptr)
                        {
                            // save the sdlfonttexture in the map
                            map_font_texture.insert(
                                std::make_pair(unique_key, sdlfonttexture));
                        }
                        else
                        {
                            throw TTFLibException(
                                "Error: Failed to create sdlfonttexture");
                        }
                    }
                }
                else
                {
                    std::cout << "Error: Could not load font from file: "
                            << font_full_path << std::endl;
                    std::cout << TTF_GetError() << std::endl;

                    throw TTFLibException(
                        "Error in font_init(), failed to load font from file");
                }
            }
            else
            {
                throw TTFLibException(
                    "Error in LoadFontTexture(): Class SDLFontManager failed to initialize. Probably the TTF library failed to initialize");
            }

        }
        else
        {
            std::string serror =
                std::string("Error: map_font_texture already contains unique key: ") + unique_key;

            throw TTFLibException(serror);
        }


    }
    
    // This function is the same as the LoadFontTexture function
    // but with an additional step of converting a font description
    // to a filename
    std::string LoadFontTextureFromDescription(
        //const SDLManager &sdlmanager,
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        const std::string& font_filename_search_string,
        const int font_size
        //SDL_Color font_color
        )
    {
        // both functions start with the code to check that the sdlmanager
        // has been initialized correctly



            // TODO: these checks related to the initialization of the
            // SDL libraries
            // so this class needs to be managed from within
            // the SDLManager class

            // Font only needs to check ttf_init
        //if( sdlmanager.TTFInitSuccess() //&&
            //m_window_init_success &&
            //m_renderer_init_success
        //    )
            // TODO: re-enable the renderer init success check?

        // this is the new check, since the constructor checks the sdlmanager.
        // TTFInitSuccess()
        if(m_font_manager_init_success == true)
        {
            // TODO: Might not need this check in either LoadFont...
            // function if I put this in the constructor?
            // TODO: move this check to constructor as well

            // this function takes a c style string, which is a pointer
            // to a null terminated array
            // DEBUG
            /*std::cout << "fontConfigGetFontFilename("
                      << font_filename_search_string << ")" << std::endl;*/
            std::string font_full_path =
                //fontConfigGetFontFilename(font_filename_search_string.c_str());
                fontConfigGetFontFilename(font_filename_search_string);
                // without .c_str() here, this causes a segfault... why?
                //"Liberation Mono"

            // DEBUG
            /*
            std::cout << "font_full_path=" << font_full_path << std::endl;
            */

            // TODO: some kind of check that fontConfigGetFontFilename
            // exited correctly and did not fail?

            LoadFontTexture(sdlrenderer, font_full_path, font_size);

            return font_full_path;
        }
        else
        {
            throw TTFLibException(
                "Error in LoadFontTextureFromDescription(): Class SDLFontManager failed to initialize. Probably the TTF library failed to initialize");
        }

        return std::string();
    }


    std::shared_ptr<SDLFontTexture>
    GetFontTexture(
        const std::string& font_full_path,
        const int font_size) const
    {
        // dont think I need to check if the sdl ttf lib initialized
        // correctly here

        FontLoadProperties fontloadproperties(
            font_full_path, font_size);
            // the description is irrelevant / ignored
            // TODO: when the LoadFontTextureFromDescription
            // function is called, the font_filename_search_string
            // argument is not propagated to this function
            // could just remove that field from FontLoadProperties

        std::string unique_key(fontloadproperties.GetUniqueKey());

        if(map_font_texture.find(unique_key) ==
            map_font_texture.cend())
        {
            std::string serror =
                std::string("Error: map_font_texture does not contain unique key: ") + unique_key;

            throw TTFLibException(serror);
        }
        else
        {
            return map_font_texture.at(unique_key);
        }

        // TODO: what to do about the return value in case of an error?
    }


    /*
        font_manager.LoadFontTextureFromDescription(
            font_filename_search_string, 12, font_color);
    */

    // TODO: update this function
    /*
    void font_manager_init(
        const SDLManager &sdlmanager,
        std::shared_ptr<SDL_Renderer> sdlrenderer,
        const std::string &font_path,
        const int font_size);
    */

    // TODO: update this function
    void font_manager_destroy()
    {
        // TODO: this function should be called once the objects
        //      std::shared_ptr<TTF_Font> m_font;
        //      std::unique_ptr<SDL_Surface> m_chars_surface;
        // are finished with 
        

        if(m_font_manager_init_success)
        {
            m_font_manager_init_success = false;
            //m_sdlfonttexturemanager.reset(); // not really needed

            map_font_texture.clear();
        }
    }

    bool FontManagerInitSuccess() const;


    // private functions

    private:


    // TODO: update this function
    // create a string containing the characters to render
    std::string init_font_chars_string() const
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
    // Notes: This function is used from sdlfontmanager.cpp
    //



    ////////////////////////////////////////////////////////////////////////////
    // private data members
    ////////////////////////////////////////////////////////////////////////////


    private:

    const int DEFAULT_FONT_SIZE = 12;

    // TODO: remove this?
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
    //std::shared_ptr<SDLFontTexture> m_sdlfonttexturemanager;
    // removed, using map below

    // map rendered font sizes to shared pointers to font texture manager
    // objects - this permits multiple font sizes to be rendered
    /*
    std::map
    <
        float?,
        std::shared_ptr<SDLFontTexture>
    >
        m_map_fonttexturemanager;
    */
    // TODO: too complex, removed

    // map a unique key produced by FontLoadProperties class
    // to a std::shared_ptr of SDLFontTexture
    std::map<std::string, std::shared_ptr<SDLFontTexture>> map_font_texture;
};

#endif // SDLFONTMANAGER_HPP


/* some documentation from elsewhere

    // the internal storage should probably be
    std::map<some object, std::shared_ptr<SDLFontTexture>> map_font_texture;
    // some object is a way to reference the loaded fonts
    // it could be an integer or a string concatinating the fields
    // inside an object containing a collection of parameters used to
    // load the font

    // when loading and "getting" the fonts, we need to know
    // a collection of parameters
    // these might be
    // 1: the font description passed to fc font config to find the
    // full font path
    // 2: the full font path
    // 3: the font size
    // 4: the font color, or any other parameters which affect
    // the font texture once loaded (these will be custom font
    // rendering parameters)

    class FontParameters
    {
        std::string font_config_search_string; // change name?
        std::string font_fullpath;
        unsigned int font_size;
        SDL_Color font_render_color;

        // TODO: is this the best way to manage rendering with
        // different colors?
    }

*/