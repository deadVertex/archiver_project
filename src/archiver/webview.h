#ifndef __WEBVIEW_H__
#define __WEBVIEW_H__

typedef unsigned int Uint32;

#include <berkelium/Berkelium.hpp>
#include <berkelium/Context.hpp>
#include <berkelium/Window.hpp>
#include <berkelium/WindowDelegate.hpp>

#include "logging.h"
#include "event.h"

class HtmlEvent : public Event
{
public:
	HtmlEvent() : Event( event_types::HTML_EVENT ) {}

	std::string arg;
};

class Webview : public Berkelium::WindowDelegate
{
public:
	Webview();
	void Initialize();
	void Shutdown();
	void LoadHtmlFromFile( const std::string path );

	virtual void onPaint(Berkelium::Window *wini,
		const unsigned char *bitmap_in, const Berkelium::Rect &bitmap_rect,
		size_t num_copy_rects, const Berkelium::Rect *copy_rects,
    int dx, int dy, const Berkelium::Rect &scroll_rect);

	inline char *GetPixelData() const { return m_pPixelStorage; }

	void ExecuteJavascript( const std::string &javascript );

	void InjectLeftMouseUp();
	void InjectLeftMouseDown();
	void InjectMouseMotion( Uint32 x, Uint32 y );

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

	void onConsoleMessage( Berkelium::Window *win, Berkelium::WideString message,
		Berkelium::WideString sourceId, int line_no )
	{
		logging::dout << sourceId << "(" << line_no << "): " << message << std::endl;
	}

	void onJavascriptCallback( Berkelium::Window *win, void* replyMsg,
		Berkelium::URLString url, Berkelium::WideString funcName,
		Berkelium::Script::Variant *args, size_t numArgs );

	void SetEventQueue( EventQueue *queue ) { m_pEventQueue = queue; }

public:
	static const Uint32 g_nWidth = 1280;
	static const Uint32 g_nHeight = 720;
	static Berkelium::Context *g_pContext;

private:
	Berkelium::Window *m_pWindow;
	char *m_pPixelStorage, *m_pWorkingPixelStorage;
	bool m_bNeedsFullRefresh;
	EventQueue *m_pEventQueue;
};

#endif