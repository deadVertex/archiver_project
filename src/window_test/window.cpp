#include "window.h"

#include <cstdlib>
#include <cassert>

Window::Window()
{
}

void Window::Initialize( const char *pStr )
{
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		exit( -1 );

	m_pWindow = SDL_CreateWindow( "Window Test", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );

	m_pRenderer = SDL_CreateRenderer( m_pWindow, -1, SDL_RENDERER_ACCELERATED );

	m_pTexture = SDL_CreateTexture( m_pRenderer, SDL_PIXELFORMAT_RGB888,
		SDL_TEXTUREACCESS_STREAMING, 1280, 720 );

	m_cWebview.Initialize();

	char buffer[ 1200 ];
	memset( buffer, 0, 1200 );
	sprintf( buffer, "file:///%s/%s", pStr, "test.html" );
	m_cWebview.LoadHtmlFromFile( buffer );
}

void Window::Shutdown()
{
	m_cWebview.Shutdown();

	SDL_DestroyTexture( m_pTexture );

	SDL_DestroyRenderer( m_pRenderer );

	SDL_DestroyWindow( m_pWindow );
	SDL_Quit();
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

void Window::Refresh()
{
	Berkelium::update();

	char *pPixels = m_cWebview.GetPixelData();
	//memset( pPixels, 0, Webview::g_nWidth * Webview::g_nHeight * 4 );

	SDL_UpdateTexture( m_pTexture, NULL, pPixels, Webview::g_nWidth * 4 );
	SDL_RenderCopy( m_pRenderer, m_pTexture, NULL, NULL );

	SDL_RenderPresent( m_pRenderer );

	/*SDL_Surface *m_pCanvas = SDL_CreateRGBSurfaceFrom( pPixels, Webview::g_nWidth, Webview::g_nHeight, 4,
		4 * Webview::g_nWidth, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF );
	


	SDL_Surface *m_pWindowSurface = SDL_GetWindowSurface( m_pWindow );
	SDL_LockSurface( m_pWindowSurface );
	SDL_BlitSurface( m_pCanvas, NULL, m_pWindowSurface, NULL );
	SDL_UnlockSurface( m_pWindowSurface );*/

	//SDL_UpdateWindowSurface( m_pWindow );
}