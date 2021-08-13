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


#include "errortypes.hpp"


class SDLFontTextureManager
{

    public:

    SDLFontTextureManager(
        const SDLManager &sdlmanager,
        std::weak_ptr<SDL_Renderer> renderer,
        const std::string &font_path,
        const int font_size)
        : m_font_init_success(false)
        , m_font_properties_success(false)
        //, m_font_chars_init_success(false)
        , m_font_render_success(false)
        , m_chars_render_texture_success(false)
        , m_glyphmetrics_success(false)
        , m_font_size(font_size)
    {
        if(font_init(font_path, font_size) == 0)
        {
            // nothing here
        }
    }


    ~SDLFontTextureManager()
    {
        // TODO: not actually needed, since going out of scope
        // will delete the shared pointer
        font_destroy();
    }


    // TODO: do not return the TTF_Font directly
    /*
    std::weak_ptr<TTF_Font> get() const
    {
        return std::weak_ptr<TTF_Font>(m_font);
    }
    */


    void font_init(
        const SDLManager &sdlmanager,
        std::weak_ptr<SDL_Renderer> renderer,
        const std::string &font_path,
        const int font_size)
    {
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
            const int font_size = 16; // 12
            m_font.reset(TTF_OpenFont(font_path.c_str(), font_size),
                TTF_CloseFont);

            if(m_font.get() != nullptr)
            {
                m_font_init_success = true;

                m_font_line_skip = TTF_FontLineSkip(m_font);
                m_font_ascent = TTF_FontAscent(m_font);
                m_font_properties_success = true; // TODO

                if(m_font_properties_success)
                    // no point continuing if the previous step didn't work
                {
                    // TODO: const here?
                    const std::string font_chars_string = init_font_chars_string();

                    set_glyph_metrics(font_chars_string);

                    render_ascii_chars();
                }
            }
            else
            {
                std::cout << "Error: Could not load font from file: "
                        << font_filename << std::endl;
                std::cout << TTF_GetError() << std::endl;

                throw TTFLibError("Error in font_init(), failed to load font from file");
            }
        }
        else
        {
            throw TTFLibError("Error in font_init(), TTF library previously failed to initialize");
        }
    }


    void font_destroy()
    {
        if(m_font_render_success)
        {
            
        }

        if(m_font_init_success)
        {
            m_font.reset(nullptr);
            m_font_init_success = false;
        }
    }



    private:

    void init_font_chars_string()
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


    void set_glyph_metrics(const std::string &font_chars_string)
    {

        // get the glyph metrics for each character to be rendered
        if(m_font_init_success)
            // technically only need the init function to have succeeded
            // to be able to call this function body
        {
            m_glyphmetrics_success = true;

            map_text_chars_advance.clear();
            map_text_chars_rect.clear();

            for(char c: m_chars)
            {
                int advance = 0;
                int xmin = 0;
                int xmax = 0;
                int ymin = 0;
                int ymax = 0;

                if(TTF_GlyphMetrics(font, c, &xmin, &xmax,
                    &ymin, &ymax, &advance) == -1)
                {
                    printf("%s\n", TTF_GetError());
                    m_glyphmetrics_success = false;

                    // TODO: this is probably invalid syntax (const inside constructor)
                    const std::string str_error(const std::string("Error in set_glyph_metrics(), failed to read glyph metrics for char ") + const std::string(c));
                    throw TTFLibError(str_error);
                }
                else
                {
                    map_text_chars_advance[c] = advance;
                    SDL_Rect r(xmin, ymin, xmax - xmin, ymax - ymin);
                    std::cout << "c=" << c
                            << " " << xmin << " " << xmax
                            << " " << ymin << " " << ymax
                            << std::endl;
                    map_text_chars_rect[c] = r;
                }
            }

            if(m_glyphmetrics_success)
            {
                //return EC_SUCCESS;
            }
            else
            {
                throw TTFLibError("Error in set_glyph_metrics(), failed to read glyph metrics");
            }
        }
        else
        {
            throw TTFLibError("Error in set_glyph_metrics(), TTF font failed to initialize");

        }
    }


    void render_ascii_chars(const std::string &font_chars_string)
    {
        

        // for: add characters to be rendered to string


        // convert the sdl surface to texture 

        //SDL_Surface *text_surface = 
            // etc choose other later
            //TTF_RenderText_Solid(
            //TTF_RenderText_Shaded(
        m_chars_surface.reset(
            TTF_RenderText_Blended(
                (TTF_Font*)m_font,
                font_chars_string.c_str(),
                COLOR_TEXT_DEFAULT),
            SDL_FreeSurface);

        if(m_chars_surface != nullptr)
        {
            m_chars_render_success = true;
        }
        else
        {
            m_chars_render_success = false;

            throw TTFLibError("Error in render_ascii_chars(), failed to render text surface");
        }

        // at this point can now render any character from the surface

        // convert the surface into a renderable texture

        // TODO: what is the best way to get this
        SDL_Renderer *renderer = sdl_manager.m_renderer;

        if(renderer != nullptr)
        {
            // TODO: since in the same function, do not need this flag
            if(m_chars_render_success)
            {
                m_chars_texture.reset(
                    SDL_CreateTextureFromSurface(renderer, m_chars_surface),
                    SDL_DestroyTexture);

                if(m_chars_texture.get() != nullptr)
                {
                    m_chars_render_texture_success = true;
                }
                else
                {
                    throw SDLLibError("Error in render_ascii_chars(), SDL_CreateTextureFromSurface error");
                }
            }
            // this code is never reachable because the error
            // will have already been thrown
            else
            {
                throw TTFLibError("Error in render_ascii_chars(), failed to render text surface");
            }
        }
        else
        {
            throw SDLLibError("Error in render_ascii_chars(), failed to get SDL renderer object");
        }




    }




    private:

    const int DEFAULT_FONT_SIZE = 12;

    bool m_font_init_success;
    bool m_chars_render_success;
    bool m_chars_render_texture_success;
    bool m_glyphmetrics_success;


    int m_font_size;

    int m_font_line_skip;
    int m_font_ascent;

    //std::string m_chars;
    std::map<char, int> map_rendered_chars_advance;
    std::map<char, SDL_Rect> map_rendered_chars_rect;
        // rect properties:
        // x, y position of coordinates of char on texture surface
        // w, h size of char on texture surface


    std::shared_ptr<TTF_Font> m_font;
    std::unique_ptr<SDL_Surface> m_chars_surface;
    std::shared_ptr<SDL_Texture> m_chars_texture;


};


#endif // SDLFONTTEXTUREMANAGER_HPP