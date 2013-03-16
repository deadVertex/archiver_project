#include "window.h"

#include <cstdlib>

Window::Window()
{
}

void Window::Initialize()
{
	m_pWindow = SDL_CreateWindow( "Window Test", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
}

void Window::Shutdown()
{
	SDL_DestroyWindow( m_pWindow );
}

void Window::ProcessInputEvents()
{
	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			case SDL_QUIT:
				Shutdown();
				exit( 0 );
				break;
		}
	}
}