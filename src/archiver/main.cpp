#include "window.h"


int main( int argc, char ** argv )
{
	Uint32 len = strlen(argv[0]);
	int lastSepPos = 0;
	for (int i = 0; argv[0][i] != '\0'; i++)
	{
		if (argv[0][i] == '\\' || argv[0][i] == '/')
			lastSepPos = i;
	}

	char sBasePath[ 1024 ];
	strcpy(sBasePath,argv[0]);
	sBasePath[lastSepPos] = '\0';

	Window cWindow;

	cWindow.Initialize( sBasePath );

	while ( 1 )
	{
		cWindow.ProcessInputEvents();
		cWindow.Refresh();
	}

	return 0;
}