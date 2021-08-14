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