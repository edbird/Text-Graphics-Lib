#include "sdlfontmanager.hpp"






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
    std::unique_ptr<SDL_Surface> m_chars_surface;



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
        const int font_size = DEFAULT_FONT_SIZE; // 12
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
                    new SDLFontTextureManager(
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
                }
                else
                {
                    // TODO: what type of error here?
                    throw SDLLibException("error in SDLFontManager::font_manager_init, failed to allocate instance of SDLFontTextureManager");
                }
            }
        }
        else
        {
            std::cout << "Error: Could not load font from file: "
                    << font_path << std::endl;
            std::cout << TTF_GetError() << std::endl;

            throw TTFLibException("Error in font_init(), failed to load font from file");
        }
    }
    else
    {
        throw TTFLibException("Error in font_init(), TTF library previously failed to initialize");
    }
}










// moved from sdlfonttexturemanager.hpp

#include "sdlhelper.hpp"

void draw(
    const SDLFontManager &sdlfontmanager,
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    //SDL_Renderer &sdlrenderer,
    const char c,
    int &x, const int y)
{

    // get offset as index
    //int offset_x = 0;
    //int index = index_of_printable_char(c);

    // TODO: get the index, and the offset by iterating through
    // the map

    std::shared_ptr<SDLFontTextureManager> sdlfonttexturemanager
        = sdlfontmanager.m_sdlfonttexturemanager;

    SDL_Rect rsrc = sdlfonttexturemanager->map_rendered_chars_srect.at(c);
    SDL_Rect rdst = sdlfonttexturemanager->map_rendered_chars_drect.at(c);
    //SDL_Rect rdst = {x, y, rsrc.w, rsrc.h};

    //std::cout << "x=" << x << std::endl;

    rdst.x += x;
    rdst.y += y;

    SDL_RenderCopy(
        sdlrenderer.get(),
        sdlfonttexturemanager->m_chars_texture.get(),
        &rsrc, &rdst);

    const int advance =
        sdlfonttexturemanager->map_rendered_chars_advance.at(c);
    x += advance;

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

}

// TODO: convert SDL Renderer to a smart pointer?

void draw_with_background(
    const SDLFontManager &sdlfontmanager,
    std::shared_ptr<SDL_Renderer> sdlrenderer,
    //SDL_Renderer &sdlrenderer,
    const char c,
    int &x, const int y,
    const SDL_Color &background_color)
{

    std::shared_ptr<SDLFontTextureManager> sdlfonttexturemanager
        = sdlfontmanager.m_sdlfonttexturemanager;

    SDL_Rect rsrc = sdlfonttexturemanager->map_rendered_chars_srect.at(c);
    SDL_Rect rdst = sdlfonttexturemanager->map_rendered_chars_drect.at(c);
    //SDL_Rect rdst = {x, y, rsrc.w, rsrc.h};

    rdst.x += x;
    rdst.y += y;

    SDL_SetRenderDrawColor(sdlrenderer.get(), background_color);
    SDL_RenderFillRect(sdlrenderer.get(), &rdst);
    SDL_RenderCopy(
        sdlrenderer.get(),
        sdlfonttexturemanager->m_chars_texture.get(),
        &rsrc, &rdst);

    const int advance =
        sdlfonttexturemanager->map_rendered_chars_advance.at(c);
    x += advance;

}