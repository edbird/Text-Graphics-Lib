
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



int SDL_SetRenderDrawColor(
    SDL_Renderer* renderer,
    SDL_Color &color)
{
    return SDL_SetRenderDrawColor(renderer,
        color.r, color.g, color.b, 255);
}


// singleton
class SDLManager
{




    public:


    static SDLManager& getInstance()
    {
        static SDLManager sdl_manager_instance;
        return sdl_manager_instance;
    }


    SDLManager()
        : m_init_success{false}
        , m_window{nullptr}
        , m_font_size{12}
        , m_window_size_x{DEFAULT_WINDOW_SIZE_X}
        , m_window_size_y{DEFAULT_WINDOW_SIZE_Y}
    {

        // standard SDL init sequence
        if(libs_init() == 0)
        {
            if(window_init() == 0)
            {
                if(renderer_init() == 0)
                {
                    if(font_init() == 0)
                    {
                        // nothing here
                    }
                }
            }
        }

    }


    ~SDLManager()
    {
        // TODO: what if SDL did not init correctly?
        font_destroy();
        renderer_destroy();
        window_destroy();
        libs_destroy();
    }


    SDLManager(SDLManager const&) = delete;
    void operator=(SDLManager const&) = delete;



    private:

    int libs_init()
    {   
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;

            SDL_Quit(); // not sure if required

            return 1;
        }
        else
        {
            if(TTF_Init() < 0)
            {
                std::cerr << TTF_GetError() << std::endl;

                TTF_Quit();
                SDL_Quit();

                return 2;
            }
            else
            {
                m_init_success = true;
                return 0;
            }
        }
    }

    void libs_destroy()
    {
        m_init_success = false;

        TTF_Quit();
        SDL_Quit();
    }

    int window_init()
    {
        if(m_init_success)
        {
            m_window = SDL_CreateWindow("SDL Text Graphics Library",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                m_window_size_x, m_window_size_y,
                SDL_WINDOW_SHOWN);
            std::cout << "Window Size: " << m_window_size_x << " "
                      << m_window_size_y << std::endl;

            if(m_window == nullptr)
            {
                std::cerr << SDL_GetError() << std::endl;

                libs_destroy();

                return 0;
            }
            else
            {
                m_window_init_success = true;
                return 0;
            }
        }
        else
        {
            return 1;
        }
    }

    void window_destroy()
    {
        if(m_window_init_success)
        {
            m_window_init_success = false;

            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
    }

    int renderer_init()
    {
        if( m_init_success &&
            m_window_init_success)
        {
            m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
            if(m_renderer == nullptr)
            {
                std::cerr << SDL_GetError() << std::endl;

                window_destroy();
                libs_destroy();

                return 2;
            }
            else
            {
                m_renderer_init_success = true;

                return 0;
            }
        }
        else
        {
            return 1;
        }
    }

    void renderer_destroy()
    {
        if(m_renderer_init_success)
        {
            m_renderer_init_success = false;

            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }
    }

    int font_init()
    {
        if( m_init_success &&
            m_window_init_success &&
            m_renderer_init_success)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    void font_destroy()
    {
        if(m_font_init_success)
        {
            m_font_init_success = false;
        }
    }


    private:

    const unsigned int DEFAULT_WINDOW_SIZE_X = 800;
    const unsigned int DEFAULT_WINDOW_SIZE_Y = 600;

    // signals that libraries initialized correctly
    // and that window was opened successfully
    // libs: SDL, TTF
    // does not include loading of fonts
    bool m_init_success;
    bool m_window_init_success;
    bool m_renderer_init_success;
    bool m_font_init_success;

    SDL_Window *m_window;
    unsigned int m_window_size_x;
    unsigned int m_window_size_y;


    public:
    SDL_Renderer *m_renderer;

    private:
    // TODO: am I using this?
    int m_font_size;

};





class SDLResourceManager
{



};


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
    


    TTF_Font *font = nullptr;
    const int font_size = 12;
    font = TTF_OpenFont(font_filename.c_str(), font_size);

    if(font == nullptr)
    {
        std::cout << "Error: Could not load font from file: "
                  << font_filename << std::endl;
        std::cout << TTF_GetError() << std::endl;
    }
    else
    {

         	

        SDL_Color COLOR_BLACK = SDL_Color(0, 0, 0);
        SDL_Color COLOR_TEXT_DEFAULT = COLOR_BLACK;

        int text_line_skip = TTF_FontLineSkip(font);

        std::string text_chars_string;
        std::map<char, std::string> map_text_chars_advance;
        std::map<char, SDL_Rect> map_text_chars_rect;
        for(char c = ' '; c <= '~'; ++ c)
        {
            text_chars_string.push_back(c);

            // get the glyph metric for the letter c in a loaded font
            int advance = 0;
            int xmin = 0;
            int xmax = 0;
            int ymin = 0;
            int ymax = 0;
            if(TTF_GlyphMetrics(font, c, &xmin, &xmax,
                &ymin, &ymax, &advance) == -1)
            {
                printf("%s\n", TTF_GetError());
            }
            else
            {
                map_text_chars_advance[c] = advance;
                SDL_Rect r(xmin, ymin, xmax - xmin, ymax - ymin);
                std::cout << "c=" << c
                          << " " << xmin << " " << xmax
                          << " " << ymin << " " << ymax
                          << std::endl;
            }
        }

        SDL_Surface *text_surface = 
            // etc choose other later
            //TTF_RenderText_Solid(
            //TTF_RenderText_Shaded(
            TTF_RenderText_Blended(
                (TTF_Font*)font,
                text_chars_string.c_str(),
                COLOR_TEXT_DEFAULT);
            

        // at this point can now render any character from the surface

        // convert the surface into a renderable texture

        SDL_Renderer *renderer = sdl_manager.m_renderer;

        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(
            renderer, text_surface);

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

                    SDL_Rect rsrc;
                    rsrc.x = 0;
                    rsrc.y = 0;
                    rsrc.w = text_texture_w;
                    rsrc.h = text_texture_h;
                    SDL_Rect rdst(0, 0, text_texture_w, text_texture_h);
                    SDL_RenderCopy(renderer, text_texture, &rsrc, &rdst);
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