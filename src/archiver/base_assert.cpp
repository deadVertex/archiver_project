#include "base_assert.h"

#include <stdio.h>
#include <time.h>

#define MAGMA_ASSERT_MSGBOX

#if defined(MAGMA_MSVC)
#include <Windows.h>
#endif

namespace magma_assert
{
	void ReportFailure(const char *condition, const char *file,
		const char *pFunc, int line, const char *msg, ...)
	{
		char buffer[400];
		if (msg != NULL)
			sprintf(buffer,"Assertion Failed: %s(%d): %s: %s\n",file,line,pFunc,msg);
		else
			sprintf(buffer,"Assertion Failed: %s(%d): %s: %s\n",file,line,pFunc,condition);

	#if defined(MAGMA_MSVC)
			OutputDebugString(buffer);
	#else
			fprintf(stderr,buffer);
	#endif
	#if defined(MAGMA_LOG_ASSERTIONS)
		time_t cTime;
		time( &cTime );
		FILE* p = fopen("assertion_failed.log","a");
		fprintf(p,"(%s): %s",ctime(&cTime),buffer);
		fclose(p);
	#endif
	}
}