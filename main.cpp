
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
#include <SDL2/SDL_ttf.h>

#include <fontconfig/fontconfig.h>


#include <iostream>

#include <map>

#include <vector>



#include "sdlmanager.hpp"
#include "sdlresourcemanager.hpp"
#include "sdlfonttexturemanager.hpp"




int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    SDL_Color &color)
{
    return SDL_SetRenderDrawColor(renderer,
        color.r, color.g, color.b, 255);
}


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











void fontConfigGetFontFilename(
    char return_buffer[],
    const unsigned int BUFFER_SIZE,
    const char* font_name_search)
{

    // font_name_search: example "Mono"

    //const unsigned short BUFFER_SIZE = 4096;
    //char return_buffer[BUFFER_SIZE];

    FcInit();
	FcConfig* config = FcInitLoadConfigAndFonts();
	FcPattern* pat = FcNameParse((const FcChar8*)font_name_search);
	FcConfigSubstitute(config, pat, FcMatchPattern);
	FcDefaultSubstitute(pat);

	char* fontFile;
	FcResult result;

	FcPattern* font = FcFontMatch(config, pat, &result);

	if(font)
	{
		FcChar8* file = NULL; 

		if(FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
		{
			fontFile = (char*)file;
            snprintf(return_buffer, BUFFER_SIZE, "%s", fontFile);
		}
	}
	FcPatternDestroy(font);

	FcPatternDestroy(pat);
	FcConfigDestroy(config);
	FcFini();

    //return &return_buffer[0];
}


int main(int argc, char* argv[])
{


    //Config config;
    //config.Load("config.txt");


    SDLManager &sdl_manager(SDLManager::getInstance());


    // "/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMono.ttf"
    const int FONT_FILENAME_BUFFER_SIZE = 4096;
    char font_filename_buffer[FONT_FILENAME_BUFFER_SIZE];
    fontConfigGetFontFilename(font_filename_buffer,
        FONT_FILENAME_BUFFER_SIZE, "Liberation Mono");

    std::string font_filename(font_filename_buffer); 
    std::cout << "Matched font filename: " << font_filename << std::endl;
    

    SDLFontTextureManager font_manager_liberation_mono(font_filename);
    //std::shared_ptr<TTF_Font> font_liberation_mono = font_manager_liberation_mono.get();
    //TTF_Font *font = font_liberation_mono.get();

        // TODO: this class should not return the TTF_Font directly

    //TTF_Font *font = nullptr;
    //const int font_size = 16; // 12
    //font = TTF_OpenFont(font_filename.c_str(), font_size);

    if(font == nullptr)
    {
        std::cout << "Error: Could not load font from file: "
                  << font_filename << std::endl;
        std::cout << TTF_GetError() << std::endl;
    }
    else
    {

        // API sketch
        //SDL_TextGridWindow textgridwindow = sdl_manager.CreateTextGridWindow();
        //textgridwindow.SetFont(const SDLFontTextureManager &font_manager_liberation_mono)

        SDL_Window *window = sdl_manager.CreateWindow();

        TextGrid textgrid(font_liberation_mono);
        textgrid.SetFont(font_liberation_mono);
        textgrid.Draw(window);


        //SDL_Color COLOR_BLACK = SDL_Color(0, 0, 0);
        //SDL_Color COLOR_TEXT_DEFAULT = COLOR_BLACK;

        // some stuff here

        if(text_texture == nullptr)
        {
            std::cout << SDL_GetError() << std::endl;
            SDL_FreeSurface(text_surface);
            text_surface = nullptr;
            
            // failure: exit program
        }
        else
        {
            SDL_FreeSurface(text_surface);
            text_surface = nullptr;

            // continue to render stuff
            SDL_Color COLOR_WHITE = SDL_Color(255, 255, 255);
            SDL_Color COLOR_BACKGROUND = COLOR_WHITE;


            
            for(bool quit = false; quit == false; )
            {
                SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND);
                SDL_RenderClear(renderer);

                //SDL_Rect rsrc = map_text_chars_rect.at()

                int text_texture_w = 0;
                int text_texture_h = 0;
                if(SDL_QueryTexture(text_texture, nullptr, nullptr,
                    &text_texture_w, &text_texture_h) != 0)
                {
                    std::cout << SDL_GetError() << std::endl;
                }
                else
                {
                    // query was ok
                    // draw whole character texture block
                    int rdst_y = 0;

                    SDL_Rect rsrc;
                    rsrc.x = 0;
                    rsrc.y = 0;
                    rsrc.w = text_texture_w;
                    rsrc.h = text_texture_h;
                    SDL_Rect rdst(0, rdst_y, text_texture_w, text_texture_h);
                    SDL_Color COLOR_GREEN(0, 255, 0);
                    SDL_SetRenderDrawColor(renderer, COLOR_GREEN);
                    SDL_RenderFillRect(renderer, &rdst);
                    SDL_RenderCopy(renderer, text_texture, &rsrc, &rdst);

                    // draw arbitary strings
                    //rdst_y += text_line_skip;
                    //rdst.y = rdst_y;
                    rdst.x = 0;
                    rdst.y += text_line_skip;

                    std::string mytext("hello world 0123456789'''");
                    int ticktock = 0;
                    for(char c: mytext)
                    {
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
                        const int maxy = map_text_chars_rect.at(c).h + map_text_chars_rect.at(c).y;
                            rsrc.y = text_ascent - maxy;
                            //rsrc.y = text_texture_h - map_text_chars_rect.at(c).y;
                            rdst.w = rsrc.w = map_text_chars_rect.at(c).w;
                            rdst.h = rsrc.h = map_text_chars_rect.at(c).h;

                            //rdst.y += rsrc.y;
                            //const int maxy = map_text_chars_rect.at(c).h + map_text_chars_rect.at(c).y;
                            rdst.y += text_ascent - maxy;
                            rdst.x += map_text_chars_rect.at(c).x;
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

                    }
                }


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

                SDL_RenderPresent(renderer);
            }

            // clean up
            SDL_DestroyTexture(text_texture);
        }


    }

    TTF_CloseFont(font);

    //SDLResourceManager sdl_resource_manager;


};