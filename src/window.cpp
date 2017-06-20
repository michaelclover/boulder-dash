#include "window.h"

Window::Window(const std::string t, const int w, const int h)
{
    m_title = t;
    m_w = w;
    m_h = h;
}

Window::~Window()
{
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_wnd);
}

bool Window::Initialise()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1; // Indicate failure
    }

    m_wnd = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_w, m_w, SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
    if(m_wnd == nullptr)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1; // Indicate failure
    }

	m_context = SDL_GL_CreateContext(m_wnd);
	if(m_context == NULL)
	{
		std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		return 1; // Indicate failure
	}

    return 0; // Indicate success
}

void Window::Resize(int windowWidth, int windowHeight)
{
    m_w = windowWidth;
    m_h = windowHeight;
}

SDL_Window* Window::GetHandle()
{
    return m_wnd;
}

int Window::GetWidth()
{
    return m_w;
}

int Window::GetHeight()
{
    return m_h;
}
