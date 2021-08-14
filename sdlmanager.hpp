#ifndef SDLMANAGER_HPP
#define SDLMANAGER_HPP




#include "errortypes.hpp"


#include <memory>
#include <iostream>


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>



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
        : m_sdl_init_success{false}
        , m_ttf_init_success{false}
        //, m_window{nullptr}
        //, m_font_size{DEFAULT_FONT_SIZE}
        //, m_window_size_x{DEFAULT_WINDOW_SIZE_X}
        //, m_window_size_y{DEFAULT_WINDOW_SIZE_Y}
    {

        // standard SDL init sequence
        try
        {
            libs_init();
        }
        catch(const SDLLibError &error)
        {
            throw;
        }
        catch(const TTFLibError &error)
        {
            throw;
        }
    }


    ~SDLManager()
    {
        libs_destroy();
    }


    // singleton
    SDLManager(SDLManager const&) = delete;
    void operator=(SDLManager const&) = delete;





    bool TTFInitSuccess() const
    {
        return m_ttf_init_success;
    }

    bool SDLInitSuccess() const
    {
        return m_sdl_init_success;
    }


    // functions relating to creation / destruction of
    // SDL_Window objects
    public:

    std::weak_ptr<SDL_Window>
    CreateWindow()
    {
        if(m_window.get() == nullptr)
        {
            std::cout << "Window Size: " << m_window_size_x << " "
                      << m_window_size_y << std::endl;
                      
            const std::string window_title(
                "SDL Text Graphics Library (C) Ed Bird 2021");
            const unsigned short width = 800;
            const unsigned short height = 600;
            m_window.reset(
                SDL_CreateWindow(
                    window_title.c_str(),
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    width, height,
                    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE),
                SDL_DestroyWindow);

            if(m_window.get() == nullptr)
            {
                throw SDLLibError("Error: failed to create window");
            }
            else
            {
                m_renderer.reset(
                    SDL_CreateRenderer(m_window.get(), some other stuff here?),
                    SDL_DestroyRenderer);

                if(m_renderer.get() == nullptr)
                {
                    throw SDLLibError("Error: failed to create renderer associated to window");
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
            throw SDLLibError("window already created");
        }

        return m_window;
    }


    bool WindowCreated() const
    {
        return (m_window.get() != nullptr);
    }


    void
    DestroyWindow()
    {
        if(m_window.get() == nullptr)
        {
            throw SDLLibError("Error, no window to destroy");
        }
        else
        {
            m.window.reset();
        }
    }
    

    private:

    void libs_init()
    {   
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;

            SDL_Quit(); // not sure if required

            throw SDLLibError("SDL_Init error");
        }
        else
        {
            m_sdl_init_success = true;

            if(TTF_Init() < 0)
            {
                std::cerr << TTF_GetError() << std::endl;

                TTF_Quit();
                SDL_Quit();

                throw TTFLibError("TTF_Init error");
            }
            else
            {
                m_ttf_init_success = true;
            }
        }
    }

    void libs_destroy()
    {
        if(m_ttf_init_success)
        {
            TTF_Quit();
            m_ttf_init_success = false;
        }

        if(m_sdl_init_success)
        {
            SDL_Quit();
            m_sdl_init_success = false;
        }
    }


    private:


    // signals that libraries initialized correctly
    // and that window was opened successfully
    // libs: SDL, TTF
    // does not include loading of fonts
    bool m_sdl_init_success;
    bool m_ttf_init_success;



    private:

    // TODO: This class should *only* manage the SDL library resources.
    // It should not manage the SDL windows and renderer - these are
    // managed by the SDLResourceManager class
    /*std::shared_ptr<SDL_Window> m_window;
    std::shared_ptr<SDL_Renderer> m_renderer;*/


};












#endif // SDLMANAGER_HPP