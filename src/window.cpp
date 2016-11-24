#include "window.h"

// Gather window information
Window::Window(std::string windowTitle, int windowWidth, int windowHeight)
{
    m_windowTitle = windowTitle;
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
}

// Properly clean up the window
Window::~Window()
{
    SDL_GL_DeleteContext(m_windowContext);
    SDL_DestroyWindow(m_windowHandle);
}

bool Window::InitializeWindow()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1; // Indicate failure
    }

    m_windowHandle = SDL_CreateWindow(m_windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
    if(m_windowHandle == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1; // Indicate failure
    }

	m_windowContext = SDL_GL_CreateContext(m_windowHandle);
	if(m_windowContext == NULL)
	{
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		return 1; // Indicate failure
	}

    return 0; // Indicate success
}

void Window::ResizeWindow(int windowWidth, int windowHeight)
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
}

SDL_Window* Window::GetWindowHandle()
{
    return m_windowHandle;
}

int Window::GetWindowWidth()
{
    return m_windowWidth;
}

int Window::GetWindowHeight()
{
    return m_windowHeight;
}

