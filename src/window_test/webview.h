#ifndef __WEBVIEW_H__
#define __WEBVIEW_H__

typedef unsigned int Uint32;

#include <berkelium/Berkelium.hpp>
#include <berkelium/Context.hpp>
#include <berkelium/Window.hpp>
#include <berkelium/WindowDelegate.hpp>

class Webview : public Berkelium::WindowDelegate
{
public:
	Webview();
	void Initialize();
	void Shutdown();
	void LoadHtmlFromFile( const char *pFile );

	virtual void onPaint(Berkelium::Window *wini,
		const unsigned char *bitmap_in, const Berkelium::Rect &bitmap_rect,
		size_t num_copy_rects, const Berkelium::Rect *copy_rects,
    int dx, int dy, const Berkelium::Rect &scroll_rect);

	inline char *GetPixelData() const { return m_pPixelStorage; }

	 virtual void onResponsive( Berkelium::Window *win)
	 {
		int a = 0;
	 }

	virtual void onStartLoading( Berkelium::Window *wini, Berkelium::URLString s )
	{
		int a = 0;
	}
	
	virtual void onLoadingStateChanged( Berkelium::Window *wini, bool b )
	{
		int a = 0;
	}

	virtual void onCrashed(Berkelium::Window *wini )
	{
		int a = 0;
	}

	virtual void onCrashedWorker( Berkelium::Window *wini )
	{
		int a = 0;
	}

	virtual void onUnresponsive( Berkelium::Window *wini )
	{
		int a = 0;
	}

public:
	static const Uint32 g_nWidth = 1280;
	static const Uint32 g_nHeight = 720;
	static Berkelium::Context *g_pContext;

private:
	Berkelium::Window *m_pWindow;
	char *m_pPixelStorage, *m_pWorkingPixelStorage;
	bool m_bNeedsFullRefresh;
};

#endif