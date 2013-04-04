#include "window.h"


int main( int argc, char ** argv )
{
	Window cWindow;

	cWindow.Initialize();

	while ( 1 )
	{
		cWindow.ProcessInputEvents();
		cWindow.Refresh();
	}

	return 0;
}