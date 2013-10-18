#include "webview.h"

#include "base_assert.h"
#include "logging.h"

enum BerkeliumKeys
{
	BK_KEYCODE_PRIOR = 0x21,
	BK_KEYCODE_NEXT = 0x22,
	BK_KEYCODE_END = 0x23,
	BK_KEYCODE_HOME = 0x24,
	BK_KEYCODE_LEFT = 0x25,
	BK_KEYCODE_UP = 0x26,
	BK_KEYCODE_RIGHT = 0x27,
	BK_KEYCODE_DOWN = 0x28,
	BK_KEYCODE_INSERT = 0x2D,
	BK_KEYCODE_DELETE = 0x2E
};

Berkelium::Context *Webview::g_pContext = NULL;

Webview::Webview()
	: m_pWindow( NULL ), m_pPixelStorage( NULL ),
		m_bNeedsFullRefresh( true ), m_pAppWindow( NULL )
{
}

void Webview::Initialize( Window *window )
{
	m_pPixelStorage = new char[ g_nWidth * ( g_nHeight + 1 ) * 4 ];
	m_pWorkingPixelStorage = new char[ g_nWidth * ( g_nHeight + 1 ) * 4 ];

	Berkelium::init( Berkelium::FileString::empty() );
	g_pContext = Berkelium::Context::create();

	m_pWindow = Berkelium::Window::create( g_pContext );
	m_pWindow->resize( g_nWidth, g_nHeight );
	m_pWindow->setDelegate( this );
	m_pWindow->setTransparent( false );
	m_pAppWindow = window;
	//m_pWindow->addBindOnStartLoading( Berkelium::WideString::point_to(L"QueueEvent"),
	//	Berkelium::Script::Variant::bindFunction( 
	//	Berkelium::WideString::point_to(L"QueueEvent"), false) );
}

void Webview::Shutdown()
{
	delete[] m_pPixelStorage;
	delete[] m_pWorkingPixelStorage;
	
	m_pWindow->destroy();

	g_pContext->destroy();
	Berkelium::destroy();
}

void Webview::LoadHtmlFromFile( const std::string path )
{
	m_pWindow->navigateTo( path.c_str(), path.length() );
}

bool MapOnPaintToTexture(
    Berkelium::Window *wini,
    const unsigned char* bitmap_in, const Berkelium::Rect& bitmap_rect,
    size_t num_copy_rects, const Berkelium::Rect *copy_rects,
    int dx, int dy,
    const Berkelium::Rect& scroll_rect,
    bool ignore_partial,
    char* scroll_buffer,
		char* pFinalOutput )
{
	const int kBytesPerPixel = 4;

	if (ignore_partial) 
	{
		if (bitmap_rect.left() != 0 || bitmap_rect.top() != 0 ||
				bitmap_rect.right() != Webview::g_nWidth ||
				bitmap_rect.bottom() != Webview::g_nHeight ) 
		{
				return false;
		}
		memcpy( pFinalOutput, bitmap_in,
			Webview::g_nWidth * Webview::g_nHeight * 4 );
    return true;
	}

	// Now, we first handle scrolling. We need to do this first since it
  // requires shifting existing data, some of which will be overwritten by
  // the regular dirty rect update.
  if (dx != 0 || dy != 0) {
      // scroll_rect contains the Rect we need to move
      // First we figure out where the the data is moved to by translating it
      Berkelium::Rect scrolled_rect = scroll_rect.translate(-dx, -dy);
      // Next we figure out where they intersect, giving the scrolled
      // region
      Berkelium::Rect scrolled_shared_rect = scroll_rect.intersect(scrolled_rect);
      // Only do scrolling if they have non-zero intersection
      if (scrolled_shared_rect.width() > 0 && scrolled_shared_rect.height() > 0) {
          // And the scroll is performed by moving shared_rect by (dx,dy)
          Berkelium::Rect shared_rect = scrolled_shared_rect.translate(dx, dy);

          int wid = scrolled_shared_rect.width();
          int hig = scrolled_shared_rect.height();
          int inc = 1;
          char *outputBuffer = scroll_buffer;
          // source data is offset by 1 line to prevent memcpy aliasing
          // In this case, it can happen if dy==0 and dx!=0.
          char *inputBuffer = scroll_buffer+(Webview::g_nWidth*1*kBytesPerPixel);
          int jj = 0;
          if (dy > 0) {
              // Here, we need to shift the buffer around so that we start in the
              // extra row at the end, and then copy in reverse so that we
              // don't clobber source data before copying it.
              outputBuffer = scroll_buffer+(
                  (scrolled_shared_rect.top()+hig+1)*Webview::g_nWidth
                  - hig*wid)*kBytesPerPixel;
              inputBuffer = scroll_buffer;
              inc = -1;
              jj = hig-1;
          }

					// Annoyingly, OpenGL doesn't provide convenient primitives, so
          // we manually copy out the region to the beginning of the
          // buffer
          for(; jj < hig && jj >= 0; jj+=inc) {
              memcpy(
                  outputBuffer + (jj*wid) * kBytesPerPixel,
                  inputBuffer + (
                      (scrolled_shared_rect.top()+jj)*Webview::g_nWidth
                      + scrolled_shared_rect.left()) * kBytesPerPixel,
                  wid*kBytesPerPixel
              );
          }
        }
    }

		for (size_t i = 0; i < num_copy_rects; i++) {
        int wid = copy_rects[i].width();
        int hig = copy_rects[i].height();
        int top = copy_rects[i].top() - bitmap_rect.top();
        int left = copy_rects[i].left() - bitmap_rect.left();
        for(int jj = 0; jj < hig; jj++) {
            memcpy(
                scroll_buffer + jj*wid*kBytesPerPixel,
                bitmap_in + (left + (jj+top)*bitmap_rect.width())*kBytesPerPixel,
                wid*kBytesPerPixel
                );
        }

				Uint32 xOffset = copy_rects[ i ].left();
				Uint32 yOffset = copy_rects[ i ].top();
				
				if ( wid == Webview::g_nWidth )
				{
					for ( Uint32 y = yOffset; y < (Uint32)hig; y++ )
					{
						memcpy( pFinalOutput + y * Webview::g_nWidth * kBytesPerPixel + xOffset * kBytesPerPixel,
							scroll_buffer + ( y - yOffset ) * wid * kBytesPerPixel,
							wid*kBytesPerPixel );
					}
				}
				else
				{
					for ( Uint32 y = yOffset; y < yOffset + (Uint32)hig; y++ )
					{
						memcpy( pFinalOutput + y * Webview::g_nWidth * kBytesPerPixel + xOffset * kBytesPerPixel,
							scroll_buffer + ( y - yOffset ) * wid * kBytesPerPixel,
							wid*kBytesPerPixel );
					}
				}
    }

		return true;
}

void Webview::onPaint( Berkelium::Window *wini,
		const unsigned char *bitmap_in, const Berkelium::Rect &bitmap_rect,
		size_t num_copy_rects, const Berkelium::Rect *copy_rects,
    int dx, int dy, const Berkelium::Rect &scroll_rect)
{
		bool bUpdated = MapOnPaintToTexture(
			wini, bitmap_in, bitmap_rect, num_copy_rects, copy_rects,
			dx, dy, scroll_rect,
			m_bNeedsFullRefresh, m_pWorkingPixelStorage, m_pPixelStorage );

		if ( bUpdated )
		{
			m_bNeedsFullRefresh = false;
		}
}

void Webview::onConsoleMessage( Berkelium::Window *win,
	Berkelium::WideString message, Berkelium::WideString sourceId,
	int line_no)
{
	std::string msg = m_cStringConverter.to_bytes( message.data() );
	std::string src = m_cStringConverter.to_bytes( sourceId.data() );

	logging::dout << "Webview console messsage: " << src << " (" << line_no <<
		 ") \"" << msg << "\"." << std::endl;
}

void Webview::RegisterFunction( const std::string &functionName, JavascriptCallback callback )
{
	m_cCallbacksMap[ functionName ] = callback; 
	std::wstring wide = m_cStringConverter.from_bytes( functionName );
	Berkelium::WideString name = Berkelium::WideString::point_to( wide.c_str() );
	m_pWindow->addBindOnStartLoading( name,
		Berkelium::Script::Variant::bindFunction( name, false ) );
}

void Webview::onJavascriptCallback( Berkelium::Window *win, void* replyMsg,
	Berkelium::URLString url, Berkelium::WideString funcName,
	Berkelium::Script::Variant *args, size_t numArgs )
{
	ASSERT( m_pAppWindow );
	std::string name = m_cStringConverter.to_bytes( funcName.data() );

	Map_t::iterator iter = m_cCallbacksMap.find( name );
	if ( iter != m_cCallbacksMap.end() )
		iter->second( m_pAppWindow, numArgs, args );
}

void Webview::ExecuteJavascript( const std::string &str )
{
	WideStringConverter_t converter;

	m_pWindow->executeJavascript( Berkelium::WideString::point_to< std::wstring >(
		converter.from_bytes( str ) ) );
}

void Webview::InjectLeftMouseUp()
{
	ASSERT( m_pWindow );
	m_pWindow->mouseButton( 0, false );
}
void Webview::InjectLeftMouseDown()
{
	ASSERT( m_pWindow );
	m_pWindow->mouseButton( 0, true );
}
void Webview::InjectMouseMotion( Uint32 x, Uint32 y )
{
	ASSERT( m_pWindow );
	m_pWindow->mouseMoved( x, y );
}

bool IsSpecialKey( Uint32 nKey )
{
  unsigned char ASCII_BACKSPACE = 8;
  unsigned char ASCII_TAB       = 9;
  unsigned char ASCII_ESCAPE    = 27;
  unsigned char ASCII_DELETE    = 127;

	return ( ( nKey == ASCII_BACKSPACE ) || 
					 ( nKey == ASCII_TAB ) || 
					 ( nKey == ASCII_ESCAPE ) || 
					 ( nKey == ASCII_DELETE ) ||
					 ( nKey == SDLK_LSHIFT ) ||
					 ( nKey == SDLK_RSHIFT ) ||
					 ( nKey == SDLK_LCTRL ) || 
					 ( nKey == SDLK_RCTRL ) ||
					 ( nKey == SDLK_LALT ) ||
					 ( nKey == SDLK_RALT ) );
}

Uint32 MapKeyToBerkeliumKey( Uint32 nKey )
{
#define MAP_VK( A, B ) case SDLK_##A: return BK_KEYCODE_##B;
	switch( nKey )
	{
		MAP_VK(INSERT, INSERT);
    MAP_VK(HOME, HOME);
    MAP_VK(END, END);
    MAP_VK(PAGEUP, PRIOR);
    MAP_VK(PAGEDOWN, NEXT);
    MAP_VK(LEFT, LEFT);
    MAP_VK(RIGHT, RIGHT);
    MAP_VK(UP, UP);
    MAP_VK(DOWN, DOWN);
		MAP_VK(DELETE, DELETE);
		case SDLK_RETURN: return 13;
		case SDLK_KP_ENTER: return 13;
    default: return 0;
	}
}

Uint32 TranslateKey( Uint32 nKey, bool isShiftActive )
{
#define R( A, B ) case A: return B;

	if ( nKey > 96 && nKey < 122 )
	{
		if ( isShiftActive )
		{
			nKey -= 'a' - 'A';
			return nKey;
		}
	}
	else if ( nKey > 122 )
	{

		switch ( nKey )
		{
			R( SDLK_KP_DIVIDE, '/' )
			R( SDLK_KP_MULTIPLY, '*' )
			R( SDLK_KP_MINUS, '-' )
			R( SDLK_KP_PLUS, '+' )
			R( SDLK_KP_ENTER, 13 )
			R( SDLK_KP_5, '5' )
		}

		if ( isShiftActive )
		{
			switch( nKey )
			{
				R( SDLK_KP_0, BK_KEYCODE_INSERT )
				R( SDLK_KP_1, BK_KEYCODE_END )
				R( SDLK_KP_2, BK_KEYCODE_DOWN )
				R( SDLK_KP_3, BK_KEYCODE_NEXT )
				R( SDLK_KP_4, BK_KEYCODE_LEFT )
				R( SDLK_KP_6, BK_KEYCODE_RIGHT )
				R( SDLK_KP_7, BK_KEYCODE_HOME )
				R( SDLK_KP_8, BK_KEYCODE_UP )
				R( SDLK_KP_9, BK_KEYCODE_PRIOR )
				R( SDLK_KP_PERIOD, BK_KEYCODE_DELETE )
			}
		}
		else
		{
			switch( nKey )
			{
				R( SDLK_KP_0, '0' )
				R( SDLK_KP_1, '1' )
				R( SDLK_KP_2, '2' )
				R( SDLK_KP_3, '3' )
				R( SDLK_KP_4, '4' )
				R( SDLK_KP_6, '6' )
				R( SDLK_KP_7, '7' )
				R( SDLK_KP_8, '8' )
				R( SDLK_KP_9, '9' )
				R( SDLK_KP_PERIOD, '.' )
			}
		}
	}
	else if ( nKey < 96 )
	{
		if ( isShiftActive )
		{
			switch ( nKey )
			{
				R( '1', '!' )
				R( '2', '@' )
				R( '3', '#' )
				R( '4', '$' )
				R( '5', '%' )
				R( '6', '^' )
				R( '7', '&' )
				R( '8', '*' )
				R( '9', '(' )
				R( '0', ')' )
				R( '-', '_' )
				R( '=', '+' )
				R( '[', '{' )
				R( ']', '}' )
				R( '\\', '|' )
				R( ';', ':' )
				R( '\'', '\"' )
				R( ',', '<' )
				R( '.', '>' )
				R( '/', '?' )
			}
		}
	}
		
	return nKey;
}

void Webview::InjectKeyEvent( SDL_KeyboardEvent *event )
{
	Uint16 sdlMod = event->keysym.mod;
	int mod = 0;
	if ( sdlMod & KMOD_ALT )
		mod |= Berkelium::ALT_MOD;
	if ( sdlMod & KMOD_CTRL )
		mod |= Berkelium::CONTROL_MOD;
	if ( sdlMod & KMOD_SHIFT )
		mod |= Berkelium::SHIFT_MOD;

	Uint32 key = event->keysym.sym;

	Uint32 mappedKey = MapKeyToBerkeliumKey( key );
	if ( mappedKey )
	{
		m_pWindow->keyEvent( event->state == SDL_PRESSED, mod, mappedKey,
			event->keysym.scancode );
	}

	
	if ( key == '\b' || key == '\r' || key == '\n' || key == ' ' || key == 127 ||
		key >= 'a' && key <= 'z' || key >= 'A' && key <= 'Z')
	{
		int nVirKey = tolower( key );
		if ( key == 127 )
			nVirKey = BK_KEYCODE_DELETE;
		m_pWindow->keyEvent( event->state == SDL_PRESSED, mod, nVirKey, 0 );
	}

	if ( !IsSpecialKey( key ) )
	{
		wchar_t sOutChars[ 2 ];
		sOutChars[ 0 ] = TranslateKey( key, ( mod & Berkelium::SHIFT_MOD ) != 0 );
		sOutChars[ 1 ] = 0;
		m_pWindow->textEvent( sOutChars, 2 );
	}
}