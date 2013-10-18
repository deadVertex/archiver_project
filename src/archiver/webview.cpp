#include "webview.h"

#include "base_assert.h"
#include "logging.h"

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