#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>
#include "webview.h"

class Window : public EventHandler
{
public:
	Window();
	void Initialize( const char *pStr );
	void Shutdown();
	void ProcessInputEvents();
	void Refresh();
	void HandleEvent( const Event *event );

private:
	SDL_Window *m_pWindow;
	SDL_Renderer *m_pRenderer;
	SDL_Texture *m_pTexture;
	Webview m_cWebview;
	EventQueue m_cEventQueue;
};

#endif