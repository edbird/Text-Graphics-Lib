#include "sdlfonttexturemanager.hpp"


#include "color.hpp"



void SDLFontTextureManager::set_glyph_metrics(
    //const SDLFontManager &sdlfontmanager,
    const std::shared_ptr<TTF_Font> &sdlfont,
    const int font_ascent,
    const std::string &font_chars_string)
{
    
    std::cout << "font_ascent=" << font_ascent << std::endl;

    m_glyphmetrics_success = false;

    // get the glyph metrics for each character to be rendered
    ////if(sdlfontmanager.FontManagerInitSuccess())
    // NOTE: this boolean signifies whether or not
    // an instance of SDLFontTextureManager (this class)
    // was allocated. Since this function is being called,
    // the *this pointer must exist and therefore this
    // check is redundant

        // technically only need the init function to have succeeded
        // to be able to call this function body
    {
        map_rendered_chars_advance.clear();
        map_rendered_chars_srect.clear();
        map_rendered_chars_drect.clear();

        int rx_offset = 0;

        for(char c: font_chars_string)
        {
            int advance = 0;

            int xmin = 0;
            int xmax = 0;
            int ymin = 0;
            int ymax = 0;

            if(TTF_GlyphMetrics(sdlfont.get(), c, &xmin, &xmax,
                &ymin, &ymax, &advance) == -1)
            {
                printf("%s\n", TTF_GetError());

                // TODO: this is probably invalid syntax (const inside constructor)
                const std::string str_error(
                    std::string("Error in set_glyph_metrics(), failed to read glyph metrics for char ") +
                    std::string{c}
                );
                throw TTFLibException(str_error);
            }
            else
            {
                map_rendered_chars_advance[c] = advance;

                ////int rx = rx_offset + xmin;
                int rx = xmin;
                //int ry = ymin + font_ascent;
                ////int ry = font_ascent - ymax;
                int ry = font_ascent - ymax;
                int rwidth = xmax - xmin;
                int rheight = ymax - ymin;

            /*
                chars_rect.at(c).h = maxy - miny
                chars_rect.at(c).y = miny
                dst.y = text_ascent - chars_rect.at(c).h - chars_rect.at(c).y;
                dst.y = text_ascent - maxy + miny - miny;
                dst.y = text_ascent - maxy;
            */


                SDL_Rect rs{rx + rx_offset, ry, rwidth, rheight};
                SDL_Rect rd{rx, ry, rwidth, rheight};
                std::cout << "c=" << c
                        << " " << xmin << " " << xmax
                        << " " << ymin << " " << ymax
                        << std::endl;
                map_rendered_chars_srect[c] = rs;
                map_rendered_chars_drect[c] = rd; // TODO: currently srcrect and dstrect are the same!!!

                rx_offset += advance;
            }
        }
    }
    //else
    //{
    //    throw TTFLibException("Error in set_glyph_metrics(), TTF font failed to initialize");
    //}

    m_glyphmetrics_success = true;

    m_font_texture_manager_init_success = m_glyphmetrics_success & m_font_render_success;

}




void SDLFontTextureManager::render_ascii_chars(
    //const SDLFontManager &sdlfontmanager,
    const std::shared_ptr<TTF_Font> &sdlfont,
    const std::shared_ptr<SDL_Renderer> sdlrenderer,
    const std::string &font_chars_string)
{
    
    m_font_render_success = false;

    // for: add characters to be rendered to string

    //if(sdlfontmanager.FontManagerInitSuccess())
    {
        // convert the sdl surface to texture 

        //SDL_Surface *text_surface = 
            // etc choose other later
            //TTF_RenderText_Solid(
            //TTF_RenderText_Shaded(

        std::shared_ptr<SDL_Surface> chars_surface(
            TTF_RenderText_Blended(
                sdlfont.get(),
                font_chars_string.c_str(),
                COLOR_TEXT_DEFAULT),
            SDL_FreeSurface);

        if(chars_surface.get() != nullptr)
        {
            //m_chars_render_success = true;
        }
        else
        {
            //m_chars_render_success = false;

            throw TTFLibException("Error in render_ascii_chars(), failed to render text surface");
        }

        // at this point can now render any character from the surface

        // convert the surface into a renderable texture

        // TODO: what is the best way to get this
        //std::shared_ptr<SDL_Renderer> renderer(); // TODO
        //SDL_Renderer *renderer = sdl_manager.m_renderer;

        if(sdlrenderer.get() != nullptr)
        {
            // TODO: since in the same function, do not need this flag
            //if(m_chars_render_success)
            if(chars_surface.get() != nullptr)
            {
                m_chars_texture.reset(
                    SDL_CreateTextureFromSurface(
                        sdlrenderer.get(),
                        chars_surface.get()),
                    SDL_DestroyTexture);

                if(m_chars_texture.get() != nullptr)
                {
                    m_font_render_success = true; // TODO: duplicated below
                }
                else
                {
                    throw SDLLibException("Error in render_ascii_chars(), SDL_CreateTextureFromSurface error");
                }
            }
            // this code is never reachable because the error
            // will have already been thrown
            else
            {
                throw TTFLibException("Error in render_ascii_chars(), failed to render text surface");
            }
        }
        else
        {
            throw SDLLibException("Error in render_ascii_chars(), failed to get SDL renderer object");
        }
    }


    m_font_render_success = true; // TODO: can remove and replace with const
                                  // true

    m_font_texture_manager_init_success = m_glyphmetrics_success & m_font_render_success;

}