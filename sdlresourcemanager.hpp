#ifndef SDLRESOURCEMANAGER_HPP
#define SDLRESOURCEMANAGER_HPP


// TODO: add a create_window() function to the singleton
// class and have this class manage creation / destruction
// of the windows
// the singleton can then check if SDL/TTF was init correctly
// before creating a window
// function should return a shared pointer to a window?

// TODO: clean this class up, it contains some old code
// which was present before it was split out into the seperate
// SDLManager class

// TODO: should this class be able to create and destory multiple windows?
// or should there be one of these classes per window?
// probably the former, else this class is just a C++ wrapper around some
// SDL2 C code
// which is essentially the same API as SFML


#include "exceptiontypes.hpp"



#include <SDL2/SDL.h>



// not a singleton
class SDLResourceManager
{

    public:

    // TODO: reference here?
    SDLResourceManager(const SDLManager &manager)
        : m_window_size_x(DEFAULT_WINDOW_SIZE_X)
        , m_window_size_y(DEFAULT_WINDOW_SIZE_Y)
#if 0
        , m_window_init_success(false)
        , m_renderer_init_success(false)        // TODO: part of old API, remove
#endif
    {
        //try()
        //window_init();
        //renderer_init();
    }


    ~SDLResourceManager()
    {
        #if 0
        if(m_renderer_init_success)
        #endif
        {
            // TODO: what if SDL did not init correctly?
            try
            {
                //renderer_destroy();
                //window_destroy();
                DestroyWindow();
            }
            catch(const SDLLibException &error)
            {
                // suppress warnings
            }
            catch(const TTFLibException &error)
            {
                // do nothing
            }
        }
    }



    ////////////////////////////////////////////////////////////////////////////
    // New window creation API
    ////////////////////////////////////////////////////////////////////////////


    // functions relating to creation / destruction of
    // SDL_Window objects
    public:


    bool WindowCreated() const
    {
        return (m_window.get() != nullptr);
    }


    void
    DestroyWindow()
    {
        if(m_window.get() == nullptr)
        {
            throw SDLLibException("Error, no window to destroy");
        }
        else
        {
            m_renderer.reset();
            m_window.reset();
        }
    }


    std::weak_ptr<SDL_Renderer>
    GetWindowRenderer()
    {
        return std::weak_ptr<SDL_Renderer>(
            m_renderer);
    }


    std::weak_ptr<SDL_Window>
    CreateWindow(const SDLManager &manager)
    {
        const std::string window_title(
            "SDL Text Graphics Library (C) Ed Bird 2021");

        const unsigned short width = DEFAULT_WINDOW_SIZE_X;
        const unsigned short height = DEFAULT_WINDOW_SIZE_Y;

        return CreateWindow(manager, window_title, width, height);
    }



    std::weak_ptr<SDL_Window>
    CreateWindow(
        const SDLManager &manager,
        const std::string& window_title,
        const unsigned short window_width,
        const unsigned short window_height)
    {
        if(manager.SDLInitSuccess())
        {
            if(m_window.get() == nullptr)
            {
                //std::cout << "Window Size: " << window_width << " "
                //        << window_height << std::endl;
                        
                m_window.reset(
                    SDL_CreateWindow(
                        window_title.c_str(),
                        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                        window_width, window_height,
                        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
                    SDL_DestroyWindow);

                if(m_window.get() == nullptr)
                {
                    throw SDLLibException("Error: failed to create window");
                }
                else
                {
                    m_renderer.reset(
                        SDL_CreateRenderer(
                            m_window.get(),
                            -1,
                            SDL_RENDERER_ACCELERATED),
                        SDL_DestroyRenderer);

                    if(m_renderer.get() == nullptr)
                    {
                        throw SDLLibException("Error: failed to create renderer associated to window");
                    }
                    else
                    {
                        // set a flag here? TODO:
                        // no flag here, state not stored, errors managed
                        // by emission of exception
                    }
                }
            }
            else
            {
                throw SDLLibException("window already created");
            }
        }
        else
        {
            throw SDLLibException("Error in window_init(), SDL library is not initialized");
        }

        return m_window;
    }



    ////////////////////////////////////////////////////////////////////////////
    // Old window creation API
    ////////////////////////////////////////////////////////////////////////////
#if 0
    void window_init(const SDLManager &manager)
    {
        if(manager.SDLInitSuccess())
        {
            if(m_init_success)
            {
                std::cout << "Window Size: " << m_window_size_x << " "
                        << m_window_size_y << std::endl;

                m_window.reset(
                    SDL_CreateWindow(
                        "SDL Text Graphics Library",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        m_window_size_x, m_window_size_y,
                        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
                    SDL_DestroyWindow);

                if(m_window.get() == nullptr)
                {
                    std::cerr << SDL_GetError() << std::endl;

                    //libs_destroy();
                    throw SDLLibException("Error in window_init(), could not create SDL window object");
                }
                else
                {
                    m_window_init_success = true;
                }
            }
            else
            {
                throw SDLLibException("Error in window_init(), could not create SDL window object, window already initialized");
            }
        }
        else
        {
            throw SDLLibException("Error in window_init(), SDL library is not initialized");
        }
    }


    void window_destroy()
    {
        if(m_window_init_success)
        {
            m_window_init_success = false;
            m_window.reset();
        }
        else
        {
            // ignore
        }
    }


    void renderer_init(const SDLManager &manager)
    {
        if(manager.SDLInitSuccess())
        {
            if(m_window_init_success)
            {
                m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
                if(m_renderer == nullptr)
                {
                    std::cerr << SDL_GetError() << std::endl;

                    window_destroy();
                    libs_destroy();

                    throw SDLLibException("Error in renderer_init(), could not create SDL renderer object");
                }
                else
                {
                    m_renderer_init_success = true;
                }
            }
            else
            {
                throw SDLLibException("Error in renderer_init(), previous error caused SDL init or window init to fail");
            }
        }
        else
        {
            throw SDLLibException("Error in window_init(), SDL library is not initialized");
        }
    }


    void renderer_destroy()
    {
        #if 0
        if(m_renderer_init_success)
        {
            m_renderer_init_success = false;
            m_renderer.reset();
        }
        else
        {
            // ignore
        }
        #endif

            m_renderer.reset();
    }
#endif

    private:

#if 0
    bool m_window_init_success;
    bool m_renderer_init_success;
#endif

    const unsigned int DEFAULT_WINDOW_SIZE_X = 800;
    const unsigned int DEFAULT_WINDOW_SIZE_Y = 600;

    std::shared_ptr<SDL_Window> m_window;
    unsigned int m_window_size_x;
    unsigned int m_window_size_y;

    public:
    std::shared_ptr<SDL_Renderer> m_renderer;


};


#endif // SDLRESOURCEMANAGER_HPP