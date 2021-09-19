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
// this class manages a single SDL window
// TODO: update so that multiple windows are supported
class SDLResourceManager
{

    // Note: Since the boolen success flags
    // m_window_init_success and m_renderer_init_success
    // are not used by any of the (updated) functions
    // in this class, these variables can be removed

    // Note: the object SDLManager manager is required
    // to check whether the SDLManager initialized correctly
    // however this object is not required by other functions
    // of this class.
    // Therefore do not want the inconvenience of passing
    // this as an argument to all functions, but do want to
    // retain the check to see if SDLManager initialized
    // correctly when calling all functions of this class.
    // Therefore retain a member variable m_sdl_init_success;

    public:

    // TODO: reference here?
    SDLResourceManager(const SDLManager &manager)
        : m_window_size_x(DEFAULT_WINDOW_SIZE_X)
        , m_window_size_y(DEFAULT_WINDOW_SIZE_Y)
        , m_sdl_init_success(false)
        //, m_sdl_lib_init_success(false)
        /*, m_window_init_success(false)
        , m_renderer_init_success(false)*/        // TODO: part of old API, remove
    {
        //try()
        //window_init();
        //renderer_init();
        
        // TODO: should I implement this?
        if(manager.SDLInitSuccess())
        {
            // this function will not throw if sdl manager
            // has initialized correctly, which implies that
            // calling other functions of this class is ok
            // and that further checks for this are not
            // required
            // (unless the user abuses this by putting a
            // try-catch block around the construction
            // of an instance of this class)
            m_sdl_init_success = true;
        }
        else
        {
            throw TTFLibException(
                "Error in SDLResourceManager(), TTF library previously failed to initialize");
        }
    }


    ~SDLResourceManager()
    {
        /*
        if(m_renderer_init_success)
        */
        if(m_sdl_init_success == true)
        {
            // TODO: what if SDL did not init correctly?
            //try
            {
                //renderer_destroy();
                //window_destroy();
                DestroyWindow();
            }
            /*catch(const SDLLibException &error)
            {
                // suppress warnings
            }
            catch(const TTFLibException &error)
            {
                // do nothing
            }*/
            // TODO: Remove try-catch block and do not
            // throw an error from DestroyWindow()
            // DONE
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
        // m_sdl_init_success: m_window will only
        // be non-nullptr if m_sdl_init_success is true
        // therefore do not check this value (m_sdl_init_success)
        // here
        if(m_window.get() == nullptr)
        {
            // TODO: this should probably be a slient condition, since
            // a window may not have been created, as this requires calling
            // the CreateWindow() function
            // DONE

            //throw SDLLibException("Error, no window to destroy");
        }
        else
        {
            m_renderer.reset();
            m_window.reset();
        }
    }

    std::shared_ptr<SDL_Window>
    GetWindow()
    {
        // will return nullptr if m_sdl_init_success is false
        return m_window;
    }

    // TODO: should these return shared pointers?
    std::shared_ptr<SDL_Renderer>
    GetWindowRenderer()
    {
        // will return nullptr if m_sdl_init_success is false
        return m_renderer;
    }


    std::shared_ptr<SDL_Window>
    //CreateWindow(const SDLManager &manager)
    CreateWindow()
    {
        if(m_sdl_init_success == true)
        {
            const std::string window_title(
                "SDL Text Graphics Library (C) Ed Bird 2021");

            const unsigned short width = DEFAULT_WINDOW_SIZE_X;
            const unsigned short height = DEFAULT_WINDOW_SIZE_Y;

            //return CreateWindow(manager, window_title, width, height);
            return CreateWindow(window_title, width, height);
        }
        else
        {
            throw std::runtime_error(
                "Error: CreateWindow: SDL lib did not initialize correctly");
        }
    }



    std::shared_ptr<SDL_Window>
    CreateWindow(
        //const SDLManager &manager,
        const std::string& window_title,
        const unsigned short window_width,
        const unsigned short window_height)
    {
        // TODO: decide if all functions should check this
        // or if only the constructor should check this, then
        // either remove or make all functions check for this
        ////if(manager.SDLInitSuccess())
        if(m_sdl_init_success == true)
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
                    throw SDLLibException(
                        "Error: failed to create window");
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
                        throw SDLLibException(
                            "Error: failed to create renderer associated to window");
                    }
                    else
                    {
                        // set a flag here? TODO:
                        // no flag here, state not stored, errors managed
                        // by emission of exception
                        
                    }
                    
                    // return the successfully created window regardless
                    // of what happened when initializing the renderer
                    return m_window;
                }
            }
            else
            {
                throw SDLLibException(
                    "window already created");
            }
        }
        else
        {
            throw SDLLibException(
                "Error: CreateWindow: SDL lib did not initialize correctly");
                //"Error in window_init(), SDL library is not initialized");
        }
    }




    private:

/*#if 0
    bool m_window_init_success;
    bool m_renderer_init_success;
#endif*/


    const unsigned int DEFAULT_WINDOW_SIZE_X = 800;
    const unsigned int DEFAULT_WINDOW_SIZE_Y = 600;

    bool m_sdl_init_success;

    std::shared_ptr<SDL_Window> m_window;
    unsigned int m_window_size_x;
    unsigned int m_window_size_y;

    //public:
    std::shared_ptr<SDL_Renderer> m_renderer;


};


#endif // SDLRESOURCEMANAGER_HPP











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