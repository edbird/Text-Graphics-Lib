
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





int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    SDL_Color &color)
{
    return SDL_SetRenderDrawColor(renderer,
        color.r, color.g, color.b, 255);
}


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
    const int FONT_FILENAME_BUFFER_SIZE = 4096;
    char font_filename_buffer[FONT_FILENAME_BUFFER_SIZE];
    fontConfigGetFontFilename(font_filename_buffer,
        FONT_FILENAME_BUFFER_SIZE, "Liberation Mono");

    std::string font_filename(font_filename_buffer); 
    std::cout << "Matched font filename: " << font_filename << std::endl;


    // create window and renderer - required to create font texture
    std::weak_ptr<SDL_Window> window =
        sdl_resource_manager.CreateWindow(sdl_manager);

    std::weak_ptr<SDL_Renderer> renderer =
        sdl_resource_manager.GetWindowRenderer();


    // create font texture
    SDLFontManager font_manager_liberation_mono(
        sdl_manager,
        std::shared_ptr<SDL_Renderer>(renderer),
        font_filename,
        12);
    


    {

        // API sketch
        //SDL_TextGridWindow textgridwindow = sdl_manager.CreateTextGridWindow();
        //textgridwindow.SetFont(const SDLFontTextureManager &font_manager_liberation_mono)


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


                SDL_RenderPresent(std::shared_ptr<SDL_Renderer>(renderer).get());
            }



        }


    }


    //SDLResourceManager sdl_resource_manager;


};