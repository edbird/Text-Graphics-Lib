
/*

    TODO:

    Other libraries to write:
        - config
        - program arguments parser
        - SDL font / resources manager
        - gui objects for text editor?
        - text editor program

*/



#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>



#include <iostream>

#include <map>

#include <vector>



#include "sdlmanager.hpp"
#include "sdlresourcemanager.hpp"
#include "sdlfontmanager.hpp"





#include "sdlhelper.hpp"


#include "color.hpp"




int index_of_printable_char(const char c)
{
    if(c >= ' ' && c <= '~')
    {
        return c - ' ';
    }
    else
    {
        return -1;
    }
}











#include "fc_helper.hpp"




int main(int argc, char* argv[])
{


    //Config config;
    //config.Load("config.txt");


    SDLManager &sdl_manager(SDLManager::getInstance());

    SDLResourceManager sdl_resource_manager(sdl_manager);


    // "/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMono.ttf"
    /*const int FONT_FILENAME_BUFFER_SIZE = 4096;
    char font_filename_buffer[FONT_FILENAME_BUFFER_SIZE];
    fontConfigGetFontFilename(font_filename_buffer,
        FONT_FILENAME_BUFFER_SIZE, "Liberation Mono");

    std::string font_filename(font_filename_buffer);*/
/*
    std::string font_filename;
    try
    {
        font_filename = fontConfigGetFontFilename("Liberation Mono");
    }
    catch(std::exception &e)
    {
        std::cout << "e.what() = " << e.what() << std::endl;
        throw;
    }
    std::cout << "Matched font filename: " << font_filename << std::endl;
*/


    // testing the case of a font that does not exist
    std::cout << "does this crash? ..." << std::endl;
    try
    {
        std::string font_filename = fontConfigGetFontFilename("Nothing");
        std::cout << font_filename << std::endl;
    }
    catch(std::exception &e)
    {
        // TODO: test this code as exe
    }

    std::cout << "Did not crash where expected" << std::endl;

    // create window and renderer - required to create font texture
    std::shared_ptr<SDL_Window> window(
        //sdl_resource_manager.CreateWindow(sdl_manager));
        sdl_resource_manager.CreateWindow());

    std::shared_ptr<SDL_Renderer> renderer(
        sdl_resource_manager.GetWindowRenderer());


    std::cout << "loading font" << std::endl;



    
    std::cout << "init font manager..." << std::endl;
    SDLFontManager font_manager(sdl_manager);
    std::cout << "font manager initialized" << std::endl;
    std::string font_filename_liberation_mono;

    std::string font_filename_times_new_roman;

    try
    {
        // TODO: put this in a try-catch block as well?
        // No: Don't need to because it was already inside a try-catch block
        std::string string_liberation_mono("Liberation Mono");

        std::string font_filename =
            // TODO: change back, calling the c-style argument version
            // SEGFAULT?
            //fontConfigGetFontFilename(string_liberation_mono.c_str());
            fontConfigGetFontFilename(string_liberation_mono);
        std::cout << "font_filename=" << font_filename << std::endl;

        font_filename_liberation_mono = font_filename;

        // this function loads a font using the SDL TTF functions
        // taking a font filename (full path) as an argument.
        // The path to the font to be loaded is obtained from the fc
        // font config functions.
        font_manager.LoadFontTexture(renderer, font_filename_liberation_mono, 12);
        // font_manager.LoadFontTexture(font_filename, 12, font_color)

        // This function does the same as the above, however instead of
        // passing a pre-determined full font file path the function
        // argument is a font description to be passed to the fc font
        // config function.
        // This function needs to have some way to signal whether the
        // fc font config function succeeded.
        std::string string_times_new_roman("Times New Roman");
        font_filename_times_new_roman =
        font_manager.LoadFontTextureFromDescription(
            //renderer, string_times_new_roman, 12, font_color);
            renderer, string_times_new_roman, 12);

        // testing the case of a font that does not exist
        std::string string_invalid("nothing");
        font_manager.LoadFontTextureFromDescription(
            //renderer, string_invalid, 12, font_color);
            renderer, string_invalid, 12);
            // seems to return something random
    }
    catch(const SDLLibException &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "catch ... case" << std::endl;
        throw;
    }
    // catch font_manager throws here
    // catch fontConfig throws here

    // the to get the actual FontTexture
    // possibly the LoadFontTexture should return the same value
    std::shared_ptr<SDLFontTexture> font_texture_liberation_mono =
        font_manager.GetFontTexture(
            font_filename_liberation_mono, 12);
        // return a shared pointer; good, can pass this to other
        // functions and classes


    // load a larger version of Liberation Mono font
    try
    {
        font_manager.LoadFontTexture(
            renderer, font_filename_liberation_mono, 18);
    }
    catch(const SDLLibException &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "catch ... case" << std::endl;
        throw;
    }
    std::shared_ptr<SDLFontTexture> font_texture_liberation_mono_large(
        font_manager.GetFontTexture(
            font_filename_liberation_mono, 18));

    std::shared_ptr<SDLFontTexture> font_texture_times_new_roman(
        font_manager.GetFontTexture(
            font_filename_times_new_roman, 12));


    {

        // API sketch
        //SDL_TextGridWindow textgridwindow = sdl_manager.CreateTextGridWindow();
        //textgridwindow.SetFont(const SDLFontTexture &font_manager_liberation_mono)


/*
        TextGrid textgrid(font_liberation_mono);
        textgrid.SetFont(font_liberation_mono);
*/

        //SDL_Color COLOR_BLACK = SDL_Color(0, 0, 0);
        //SDL_Color COLOR_TEXT_DEFAULT = COLOR_BLACK;

        // some stuff here

        
        {


            // continue to render stuff
            SDL_Color COLOR_BACKGROUND = COLOR_WHITE;

            std::cout << "Main loop" << std::endl;





            // main infinite loop
            for(bool quit = false; quit == false; )
            {
                


                SDL_Event event;
                while(SDL_PollEvent(&event) != 0)
                {
                    if(event.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    else
                    {
                        // nothing
                    }
                }



                SDL_SetRenderDrawColor(
                    std::shared_ptr<SDL_Renderer>(renderer).get(),
                    COLOR_BACKGROUND);

                SDL_RenderClear(std::shared_ptr<SDL_Renderer>(renderer).get());


                //textgrid.Draw(window);

                /*
                std::shared_ptr<SDLFontTexture> font_texture_liberation_mono =
                    font_manager.GetFontTexture(
                        font_filename_liberation_mono, 12);
                */

                int font_line_skip =
                    font_texture_liberation_mono->GetFontLineSkip();
                    
                int font_ascent =
                    font_texture_liberation_mono->GetFontAscent();

                ///int rdst_y = 0;
                ///SDL_Rect rdst(0, rdst_y, 0, 0);

                int pos_x = 10;
                int pos_y = 10;

                // draw arbitary strings
                //rdst_y += font_line_skip;
                //rdst.y = rdst_y;
                ///rdst.x = 0;
                ///rdst.y += font_line_skip;

                std::string mytext("hello world 0123456789'''");
                int ticktock = 0;
                int t_pos_x = pos_x;
                for(char c: mytext)
                {
                    write(
                        //std::shared_ptr<SDL_Renderer>(renderer),
                        renderer,
                        font_texture_liberation_mono,
                        c, t_pos_x, pos_y,
                        true);
                }
                write_string(
                    renderer,
                    font_texture_liberation_mono_large,
                    "some random text", t_pos_x, pos_y, true);

                t_pos_x = pos_x;
                for(char c: mytext)
                {
                    write_with_background(
                        //std::shared_ptr<SDL_Renderer>(renderer),
                        renderer,
                        font_texture_liberation_mono,
                        c, t_pos_x, pos_y + 40,
                        true,
                        COLOR_GREEN);
                }

                std::string mystring("this is a test string!");
                t_pos_x = pos_x;
                write_string(
                    //std::shared_ptr<SDL_Renderer>(renderer),
                    renderer,
                    //font_texture_liberation_mono_large,
                    font_texture_times_new_roman,
                    mystring, t_pos_x, pos_y + 80,
                    false);


                    /*
                    int offset_x = 0;
                    int index = index_of_printable_char(c);
                    if(index >= 0)
                    {
                        for(int count = 0;
                            count < index;
                            ++ count)
                        {
                            //rsrc_x += map_text_chars_rect.at(c).w;
                            offset_x += map_text_chars_advance.at(c);
                        }
                        rsrc.x = map_text_chars_rect.at(c).x + offset_x;
                        //rsrc.y = map_text_chars_rect.at(c).y;
                    const int maxy = map_text_chars_rect.at(c).h + map_text_chars_rect.at(c).y; // TODO: remove?
                        rsrc.y = text_ascent - maxy;
                        //rsrc.y = text_texture_h - map_text_chars_rect.at(c).y;
                        rdst.w = rsrc.w = map_text_chars_rect.at(c).w;
                        rdst.h = rsrc.h = map_text_chars_rect.at(c).h;

                        //rdst.y += rsrc.y;
                        //const int maxy = map_text_chars_rect.at(c).h + map_text_chars_rect.at(c).y;
                        rdst.y += text_ascent - maxy;
                        rdst.x += map_text_chars_rect.at(c).x;

                        // TODO: remove this block
                        if(ticktock == 0)
                        {
                            //SDL_Color COLOR_GREEN(0, 255, 0);
                            SDL_SetRenderDrawColor(renderer, COLOR_GREEN);
                            SDL_RenderFillRect(renderer, &rdst);
                        }
                        ticktock += 1;
                        ticktock %= 2;

                        SDL_RenderCopy(renderer, text_texture, &rsrc, &rdst);
                        rdst.x -= map_text_chars_rect.at(c).x;
                        rdst.y -= text_ascent - maxy;
                        int advance = map_text_chars_advance.at(c);
                        
                        //rdst.y -= rsrc.y;
                        
                        rdst.x += advance;
                        rdst.y += 0;
                    }
                    else
                    {
                        std::cout << "Error in index_of_printable_char" << std::endl;
                    }
                    */

                


                //SDL_RenderPresent(std::shared_ptr<SDL_Renderer>(renderer).get());
                SDL_RenderPresent(renderer.get());
            }



        }


    }


    //SDLResourceManager sdl_resource_manager;


}