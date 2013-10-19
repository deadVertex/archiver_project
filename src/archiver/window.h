#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>
#include "webview.h"

#include "archive_worker.h"

class QuitEvent : public Event
{
public:
	QuitEvent() : Event( event_types::QUIT ) {}

	int m_iExitCode;
};

class Window : public EventHandler
{
public:
	Window();
	void Initialize( const char *pStr );
	void Shutdown();
	void ProcessInputEvents();
	void Refresh();
	void HandleEvent( const Event *event );
	template< typename T >
	T* QueueEvent()
	{ return m_cEventQueue.QueueEvent< T >(); }

private:
	SDL_Window *m_pWindow;
	SDL_Renderer *m_pRenderer;
	SDL_Texture *m_pTexture;
	Webview m_cWebview;
	EventQueue m_cEventQueue;
	ArchiveWorker worker;
};

#endif