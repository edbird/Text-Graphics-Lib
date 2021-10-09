#include "sdlfonttexture.hpp"


#include "color.hpp"




std::string SDLFontTexture::init_valid_font_chars_string(std::shared_ptr<TTF_Font> sdlfont) /*const*/ // cannot be const because modifies map_rendered_chars_glyph_value_valid
{
        // from the check_ascii_chars function
        /*
        map_rendered_chars_glyph_value_valid.clear();

        std::string valid_font_chars_string;
        for(char c: requested_font_chars_string)
        {
            int is_glyph_provided = TTF_GlyphIsProvided(sdlfont.get(), c);

            if(is_glyph_provided)
            {
                map_rendered_chars_glyph_value_valid.insert(std::make_pair(c, true));

                valid_font_chars_string.push_back(c);
            }
            else
            {
                map_rendered_chars_glyph_value_valid.insert(std::make_pair(c, false));

                // do not add valud to font_chars_string
            }
        }
        */

    // TODO: probably do not want to keep both objects:
    // map_rendered_chars_glyph_value_valid
    // and
    // valid_font_chars_string
    // since they contain the same data

    std::string valid_font_chars_string;

    // TODO: this function returns a temporary object (std::string)
    // it is a bit weird that it also sets map_rendered_chars_glyph_value_valid
    // TODO: do I need this clear call?
    map_rendered_chars_glyph_value_valid.clear();

    // TODO: might want to remove this variable, however it is
    // currently used by the write() function
    // but I don't think it needs to be???

    for(int c = 0; c <= 255; ++ c)
    {
        // TODO: the fonts do not seem to render properly when
        // characters outside the range of ' ' to '~' are
        // renderered. Not sure why this is the case at the
        // present time
        
        // TODO: skip "bad" characters
        if(c < ' ') continue; // this seems to have no effect on the rendering results
        //if(c > '~' + 50) continue; // this renders all the "normal" chars correctly, but NOT the extended ones
        if(c > '~') continue;

        // my conclusion is that the special characters such as the caret
        // have to be rendered manually

        unsigned char unsigned_c = (unsigned char)c;
        // TODO: this should take a Uint16
        int is_glyph_provided = TTF_GlyphIsProvided(sdlfont.get(), (unsigned short)unsigned_c); // TODO: there was a bug here, perhaps this was the cause of the error?
        if(is_glyph_provided)
        {
            valid_font_chars_string.push_back(c);

            // TODO: remove this map?

            // this conversion back to char is super weird
            map_rendered_chars_glyph_value_valid.insert(std::make_pair((char)unsigned_c, true)); // TODO: there was a bug here, perhaps this was the cause of the error?
        }
        else
        {
            // this conversion back to char is super weird
            map_rendered_chars_glyph_value_valid.insert(std::make_pair((char)unsigned_c, false)); // TODO: there was a bug here, perhaps this was the cause of the error?

            // do not add valud to valid_font_chars_string
        }
    }

    return valid_font_chars_string;
}





// note: this assumes that font_chars_string contains only valid
// renderable characters for this font
void SDLFontTexture::set_glyph_metrics(
    //const SDLFontManager &sdlfontmanager,
    const std::shared_ptr<TTF_Font> &sdlfont,
    const int font_ascent,
    const std::string &font_chars_string)
{
    
    std::cout << "font_ascent=" << font_ascent << std::endl;

    m_glyphmetrics_success = false;
    //m_font_texture_init_success = m_glyphmetrics_success & m_font_render_success;
    m_font_texture_init_success = false;
    // NOTE: this needs to be set to false, otherwise this logic only
    // works once

    // get the glyph metrics for each character to be rendered
    ////if(sdlfontmanager.FontManagerInitSuccess())
    // NOTE: this boolean signifies whether or not
    // an instance of SDLFontTexture (this class)
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
                    std::string("Error in set_glyph_metrics(), failed to read glyph metrics for char ") + std::string{c});
                    
                throw TTFLibException(str_error);
            }
            else
            {
                /*
                std::cout << "Next Glyph Metric: "
                          << "c=" << (int)c
                          << " xmin=" << xmin
                          << " xmax=" << xmax
                          << " ymin=" << ymin
                          << " ymax=" << ymax
                          << " advance=" << advance
                          << std::endl;
                */

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
                // DEBUG: printing characters and sdl rect parameters
                /*
                std::cout << "c=" << c
                        << " " << xmin << " " << xmax
                        << " " << ymin << " " << ymax
                        << std::endl;
                */
                map_rendered_chars_srect[c] = rs;
                map_rendered_chars_drect[c] = rd;
                // TODO: currently srcrect and dstrect are the same!!!

                rx_offset += advance;
            }
        }
    }
    //else
    //{
    //    throw TTFLibException("Error in set_glyph_metrics(), TTF font failed to initialize");
    //}

    m_glyphmetrics_success = true;

    // TODO: add a variable for the function check_ascii_chars
    m_font_texture_init_success = m_glyphmetrics_success & m_font_render_success;

}



// note: this assumes that font_chars_string contains only valid
// renderable characters for this font
void SDLFontTexture::render_ascii_chars(
    //const SDLFontManager &sdlfontmanager,
    const std::shared_ptr<TTF_Font> &sdlfont,
    const std::shared_ptr<SDL_Renderer> sdlrenderer,
    const std::string &font_chars_string)
{
    
    m_font_render_success = false;
    //m_font_texture_init_success = m_glyphmetrics_success & m_font_render_success;
    m_font_texture_init_success = false;
    // NOTE: this needs to be set to false, otherwise this logic only
    // works once

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

            int width = chars_surface->w;
            int height = chars_surface->h;

            std::cout << "Render success: "
                      << "width=" << width << " height=" << height
                      << std::endl;

        }
        else
        {
            //m_chars_render_success = false;

            throw TTFLibException("Error in render_ascii_chars(), failed to render text surface");
        }

        // at this point can now render any character from the surface

        // convert the surface into a renderable texture
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

    // TODO: add a variable for the function check_ascii_chars
    m_font_texture_init_success = m_glyphmetrics_success & m_font_render_success;

}


// NOTE: function NOT used
// TODO: TTF_GlyphIsProvided is supposed to take a 16 bit value
// not just an 8 bit char value, so there may be more renderable
// values outside of the value range 0 - 255 which we do not
// have access to because of the choice to use "char" everywhere
std::string SDLFontTexture::check_ascii_chars(
    const std::shared_ptr<TTF_Font> &sdlfont,
    const std::string &requested_font_chars_string)
{

    map_rendered_chars_glyph_value_valid.clear();

    std::string valid_font_chars_string;
    for(char c: requested_font_chars_string)
    {
        int is_glyph_provided = TTF_GlyphIsProvided(sdlfont.get(), c);

        if(is_glyph_provided)
        {
            map_rendered_chars_glyph_value_valid.insert(std::make_pair(c, true));

            valid_font_chars_string.push_back(c);
        }
        else
        {
            map_rendered_chars_glyph_value_valid.insert(std::make_pair(c, false));

            // do not add valud to font_chars_string
        }
    }



    // TODO: add a variable for the function check_ascii_chars
    //m_font_texture_init_success = m_glyphmetrics_success & m_font_render_success;

    return valid_font_chars_string;

}


// NOTE: function NOT used
// should not use both this function and the previous function, use only one
// TODO: might be able to reduce duplicate code between this function and
// the one above
void SDLFontTexture::render_ascii_chars_safe(
    const std::shared_ptr<TTF_Font> &sdlfont,
    const std::shared_ptr<SDL_Renderer> sdlrenderer,
    const std::string &font_chars_string_request)
{

    // this is new code

    // valid values are inserted into this string
    std::string font_chars_string;
    for(char c: font_chars_string_request)
    {
        int is_glyph_provided = TTF_GlyphIsProvided(sdlfont.get(), c);

        if(is_glyph_provided)
        {
            map_rendered_chars_glyph_value_valid.insert(std::make_pair(c, true));

            font_chars_string.push_back(c);
        }
        else
        {
            map_rendered_chars_glyph_value_valid.insert(std::make_pair(c, false));

            // do not add valud to font_chars_string
        }
    }
    // TODO: put this code block into a new function call and call that function
    // from this class constructor before calling the functions
    // set_glyph_metrics and render_ascii_chars

    // this is copied and pasted from above

    m_font_render_success = false;
    //m_font_texture_init_success = m_glyphmetrics_success & m_font_render_success;
    m_font_texture_init_success = false;
    // NOTE: this needs to be set to false, otherwise this logic only
    // works once

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

    // TODO: add a variable for the function check_ascii_chars
    m_font_texture_init_success = m_glyphmetrics_success & m_font_render_success;

}