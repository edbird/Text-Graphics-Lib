#ifndef SDLRESOURCEMANAGER_HPP
#define SDLRESOURCEMANAGER_HPP


// TODO: add a create_window() function to the singleton
// class and have this class manage creation / destruction
// of the windows
// the singleton can then check if SDL/TTF was init correctly
// before creating a window
// function should return a shared pointer to a window?




#include "errortypes.hpp"


// not a singleton
class SDLResourceManager
{

    public:

    // TODO: reference here?
    SDLResourceManager(const SDLManager &manager)
    {

        if(window_init() == 0)
        {
            if(renderer_init() == 0)
            {
                // nothing

                // TODO: font init? - or this goes elsewhere
            }
        }
    }


    ~SDLResourceManager()
    {
        if(m_renderer_init_success)
        {
            // TODO: what if SDL did not init correctly?
            font_destroy();
            renderer_destroy();
            window_destroy();
        }
    }




    void window_init()
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
            }
            else
            {
                m_window_init_success = true;
            }
        }
        else
        {
            throw SDLLibError("Error in window_init(), could not create SDL window object");
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


    void renderer_init()
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

                throw SDLLibError("Error in renderer_init(), could not create SDL renderer object");
            }
            else
            {
                m_renderer_init_success = true;
            }
        }
        else
        {
            throw SDLLibError("Error in renderer_init(), previous error caused SDL init or window init to fail");
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


    private:

    bool m_window_init_success;
    bool m_renderer_init_success;

    const unsigned int DEFAULT_WINDOW_SIZE_X = 800;
    const unsigned int DEFAULT_WINDOW_SIZE_Y = 600;

    //SDL_Window *m_window;
    std::shared_ptr<SDL_Window> m_window;
    unsigned int m_window_size_x;
    unsigned int m_window_size_y;

    public:
    //SDL_Renderer *m_renderer;
    std::shared_ptr<SDL_Renderer> m_renderer;


};


#endif // SDLRESOURCEMANAGER_HPP