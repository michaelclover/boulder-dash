#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Window
{
    public:
        // Constructor: assigns title of the window, width of the window, and height of the window
        Window(const std::string t, const int w, const int h);

        // Destructor: destroys the window handle and frees up the OpenGL context
        ~Window();

        // Initialises SDL, creates the window, and the OpenGL context
        bool Initialise();

        // Changes the height and width variables, and makes a call to glViewport()
        void Resize(int windowWidth, int windowHeight);

        // Get method for the window handle
        SDL_Window* GetHandle();

        // Get method for window width
        int GetWidth();

        // Get method for window height
        int GetHeight();

    private:
        // Title of the window
        std::string m_title;

        // Width of the window
        int m_w;

        // Height of the window
        int m_h;

        // Window handle
        SDL_Window* m_wnd;

        // OpenGL context
        SDL_GLContext m_context;
};

#endif // WINDOW_H
