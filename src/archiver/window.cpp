#include "window.h"

#include <cstdlib>
#include <cassert>

Window::Window()
{
}

void Window::HandleEvent( const Event *event )
{
	if ( event->GetType() == event_types::HTML_EVENT )
	{
		const HtmlEvent *p = event_cast< HtmlEvent >( event );
		if ( p->arg == "quit" )
		{
			Shutdown();
			exit( 0 );
		}
	}
	else if ( event->GetType() == event_types::QUIT )
	{
		Shutdown();
		exit( 0 );
	}
}

void Quit( Window *window, Uint32 argc, Berkelium::Script::Variant *argv )
{
	window->QueueEvent< QuitEvent >();
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

	m_cWebview.Initialize( this );
	m_cWebview.RegisterFunction( "Quit", Quit );
	//m_cWebview.SetEventQueue( &m_cEventQueue );

	m_cEventQueue.AddHandler( this );

	char buffer[ 1200 ];
	memset( buffer, 0, 1200 );
	sprintf( buffer, "file:///%s/%s", pStr, "html/main.html" );
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
				m_cWebview.InjectMouseMotion( event.motion.x, event.motion.y );
				break;
			case SDL_MOUSEBUTTONDOWN:
				if ( event.button.button == 1 )
				{
					m_cWebview.InjectLeftMouseDown();
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if ( event.button.button == 1 )
				{
					m_cWebview.InjectLeftMouseUp();
				}
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
	m_cEventQueue.ProcessEvents( SDL_GetTicks() );

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