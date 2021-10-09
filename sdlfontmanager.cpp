#include "sdlfontmanager.hpp"

void
SDLFontManager::LoadFontTexture(
    //const SDLManager &sdlmanager,
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    const std::string& font_full_path,
    const int font_size
    , std::string &valid_ascii_font_chars_string
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

    std::cout << "unique_key=" << unique_key << std::endl;
    
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
        if(m_sdl_ttf_init_success == true)
        {
            m_font.reset(TTF_OpenFont(
                font_full_path.c_str(),
                font_size),
                TTF_CloseFont);

            if(m_font.get() != nullptr)
            {

                // this block is just to wrap the font_chars_string
                // object
                {
                    // TODO: const here?
                    /*const std::string font_chars_string =
                        init_font_chars_string();*/
                    // TODO: this function has now been removed because (I thought)
                    // it was not used anywhere... turns out it WAS used here, but
                    // it SHOULD be moved inside of SDLFontTexture regardless

                    std::shared_ptr<SDLFontTexture> sdlfonttexture;

                    // create the FontTextureManager
                    // TODO: this needs to return the renderable characters
                    // for now do this using std::string, but some fonts
                    // support unicode / 16bit codes, therefore a std::string
                    // is not sufficient (but perhaps a wide string is)
                    /*std::string valid_ascii_font_chars_string;*/
                    sdlfonttexture.reset(
                        new SDLFontTexture(
                            sdlrenderer,
                            //*this,
                            m_font,
                            font_size,
                            ////font_line_skip,
                            ////font_ascent,
                            valid_ascii_font_chars_string
                            /*,
                            font_chars_string*/));
                    // this will (may?) fail here if font_chars_string
                    // contains invalid characters which cannot be rendered
                    // ... the point of failure was confusing because of the
                    // throwing and then catching of errors


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

    std::cout << "LoadFontTexture return ok" << std::endl;
}



std::string
SDLFontManager::LoadFontTextureFromDescription(
    //const SDLManager &sdlmanager,
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    const std::string& font_filename_search_string,
    const int font_size
    , std::string &valid_ascii_font_chars_string
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
    // this is the new check, since the constructor checks the sdlmanager
    if(m_sdl_ttf_init_success == true)
    {

        // DEBUG
        /*std::cout << "fontConfigGetFontFilename("
                    << font_filename_search_string << ")" << std::endl;*/

        std::string font_full_path =
            fontConfigGetFontFilename(font_filename_search_string);

        // DEBUG
        
        std::cout << "font_full_path=" << font_full_path << std::endl;
        

        // TODO: some kind of check that fontConfigGetFontFilename
        // exited correctly and did not fail?

        LoadFontTexture(sdlrenderer, font_full_path, font_size
            , valid_ascii_font_chars_string);
        std::cout << "finished LoadFromTexture function, return: " << font_full_path << std::endl;

        // Valid return path
        return font_full_path;
    }
    else
    {
        throw TTFLibException(
            "Error in LoadFontTextureFromDescription(): Class SDLFontManager failed to initialize. Probably the TTF library failed to initialize");
    }

    // failure return path - never reached?
    // removed because it was confusing
    //return std::string();
}


std::shared_ptr<SDLFontTexture>
SDLFontManager::GetFontTexture(
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


// TODO: in order for this function to check if each character
// can be rendered by the font, it needs to call the function
// TTF_GlyphIsProvided, which requires a TTF_Font object
// therefore this function is not universal and needs to be
// placed in the sdlfonttexture class
//
// Note: This function is NOT used in this class. It WAS used
// in the removed function: SDLFontManager::font_manager_init
// but this function no longer exists
/*
std::string
SDLFontManager::init_font_chars_string() const
{
    //m_font_chars_string_init_success = true;

    // add characters to be rendered to string
    std::string font_chars_string;
    //for(char c = ' '; c <= '~'; ++ c)

    // try to initialize all available glyphs
    //int is_glyph_provided = TTF_GlyphIsProvided(font, code);
    const int c_start = 0; // ' '
    const int c_end = (int)255; // '~'
    for(int c = c_start; c <= c_end; ++ c)
    {
        unsigned char unsigned_c = (unsigned char)c;
        font_chars_string.push_back(unsigned_c);
    }

    return font_chars_string;
}
*/


// TODO: check the sdlfonttexture.cpp file and ensure all the code here
// has been migrated there
// currently a check for the correct initialization of the sdlmanager
// (sdl libs) is missing
/*
void SDLFontManager::font_manager_init(
    const SDLManager &sdlmanager,
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    const std::string &font_path,
    const int font_size)
{

    // TODO: these shared pointers were members of the
    // SDLFontManager class but they do not need to be saved
    // for future use, therefore move them to be local variables
    // such that storage will be freed after use

    std::shared_ptr<TTF_Font> m_font;
    // TODO: m_chars_surface is not used here, remove?
    //std::unique_ptr<SDL_Surface> m_chars_surface;



        // TODO: these checks related to the initialization of the
        // SDL libraries
        // so this class needs to be managed from within
        // the SDLManager class

        // Font only needs to check ttf_init
    if( sdlmanager.TTFInitSuccess() //&&
        //m_window_init_success &&
        //m_renderer_init_success
        )
    {
        //TTF_Font *font = nullptr;
        //const int font_size = DEFAULT_FONT_SIZE; // 12
        m_font.reset(TTF_OpenFont(font_path.c_str(), font_size),
            TTF_CloseFont);

        if(m_font.get() != nullptr)
        {

            // NOTE: presumably these cannot fail
            const int font_line_skip = TTF_FontLineSkip(m_font.get());
            const int font_ascent = TTF_FontAscent(m_font.get());
            //m_font_properties_success = true; // TODO

            //if(m_font_properties_success)
                // no point continuing if the previous step didn't work
            {
                // TODO: const here?
                const std::string font_chars_string = init_font_chars_string();


                // create the FontTextureManager
                m_sdlfonttexturemanager.reset(
                    new SDLFontTexture(
                        sdlrenderer,
                        //*this,
                        m_font,
                        font_size,
                        font_line_skip,
                        font_ascent,
                        font_chars_string));

                // NOTE: managed internally
                //m_sdlfonttexturemanager->set_glyph_metrics(font_chars_string);
                //m_sdlfonttexturemanager->render_ascii_chars(font_chars_string);

                if(m_sdlfonttexturemanager.get() != nullptr)
                {
                    m_font_manager_init_success = true;
                    // TODO: maybe can remove this variable, does nothing?

                    // NOTE: this variable tracks whether the memory was 
                    // allocated and not whether the actual FontTexture
                    // was able to initialize successfully
                }
                else
                {
                    // TODO: what type of error here?
                    throw SDLLibException("error in SDLFontManager::font_manager_init, failed to allocate instance of SDLFontTexture");
                }
            }
        }
        else
        {
            std::cout << "Error: Could not load font from file: "
                      << font_path << std::endl;
            std::cout << TTF_GetError() << std::endl;

            throw TTFLibException(
                "Error in font_init(), failed to load font from file");
        }
    }
    else
    {
        throw TTFLibException(
            "Error in font_init(), TTF library previously failed to initialize");
    }
}
*/



