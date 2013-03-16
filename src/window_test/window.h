#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>
#include "webview.h"

class Window
{
public:
	Window();
	void Initialize();
	void Shutdown();
	void ProcessInputEvents();

private:
	SDL_Window *m_pWindow;
	Webview m_cWebview;
};

#endif