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
	void Refresh();

private:
	SDL_Window *m_pWindow;
	SDL_Renderer *m_pRenderer;
	SDL_Texture *m_pTexture;
	Webview m_cWebview;
};

#endif