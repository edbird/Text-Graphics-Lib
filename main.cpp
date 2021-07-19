

#include <SDL.h>
#include <SDL_ttf.h>




class SDLManager
{


    // singleton

    public:

    SDLManager()
        ; m_init_success{false}
        , window{nullptr}
        , m_font_size{12}
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

            if(m_window == nullptr)
            {
                std::cerr << SDL_GetError() << std::endl;

                libs_destroy();

                return 0;
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
            m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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

    // signals that libraries initialized correctly
    // and that window was opened successfully
    // libs: SDL, TTF
    // does not include loading of fonts
    bool m_init_success;
    bool m_window_init_success;
    bool m_renderer_init_success;
    bool m_font_init_success;

    SDL_Window *window;
    unsigned int m_window_size_x;
    unsigned int m_window_size_y;

    SDL_Renderer *m_renderer;

    int m_font_size;

};


class SDLResourceManager
{



}



int main(int argc, char* argv[])
{


    Config config;
    config.Load("config.txt");





    SDLManager sdl_manager;

    SDLResourceManager sdl_resource_manager;


};