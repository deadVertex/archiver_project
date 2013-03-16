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

public:
	static const Uint32 g_nWidth;
	static const Uint32 g_nHeight;
	static Berkelium::Context *g_pContext;

private:
	Berkelium::Window *m_pWindow;
	char *m_pPixelStorage, *m_pWorkingPixelStorage;
	bool m_bNeedsFullRefresh;
};

#endif