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
        Window(std::string windowTitle, int windowWidth, int windowHeight);

        // Destructor: destroys the window handle and frees up the OpenGL context
        ~Window();

        // Initialises SDL, creates the window, and the OpenGL context
        bool InitializeWindow();

        // Changes the height and width variables, and makes a call to glViewport()
        void ResizeWindow(int windowWidth, int windowHeight);

        // Get method for the window handle
        SDL_Window* GetWindowHandle();

        // Get method for window width
        int GetWindowWidth();

        // Get method for window height
        int GetWindowHeight();

    private:
        // Title of the window
        std::string m_windowTitle;

        // Width of the window
        int m_windowWidth;

        // Height of the window
        int m_windowHeight;

        // Window handle
        SDL_Window* m_windowHandle;

        // OpenGL context
        SDL_GLContext m_windowContext;
};

#endif // WINDOW_H
