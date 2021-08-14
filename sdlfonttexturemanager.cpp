#include "sdlfonttexturemanager.hpp"





void SDLFontTextureManager::set_glyph_metrics(const std::string &font_chars_string)
{
    
    m_glyphmetrics_success = false;

    // get the glyph metrics for each character to be rendered
    if(m_font_init_success)
        // technically only need the init function to have succeeded
        // to be able to call this function body
    {
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
    }
    else
    {
        throw TTFLibError("Error in set_glyph_metrics(), TTF font failed to initialize");
    }

    m_glyphmetrics_success = true;

    m_font_texture_manager_init_success = m_glyphmetrics_success & m_font_render_success;

}




void SDLFontTextureManager::render_ascii_chars(const std::string &font_chars_string)
{
    
    m_font_render_success = false;

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
        //m_chars_render_success = true;
    }
    else
    {
        //m_chars_render_success = false;

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

    m_font_render_success = true;

    m_font_texture_manager_init_success = m_glyphmetrics_success & m_font_render_success;

}