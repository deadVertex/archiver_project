#ifndef __MAGMA_BASE_ASSERT_H__
#define __MAGMA_BASE_ASSERT_H__

#include "platform.h"

namespace magma_assert
{

	void ReportFailure(const char *condition,
		const char *file, const char *pFunc, int line, const char *msg, ...);

#if defined (MAGMA_MSVC) 
	#define MAGMA_HALT() __debugbreak();
#elif defined(MAGMA_GNU)
	#define MAGMA_HALT() asm("int $3");
#endif
}

#if defined(MAGMA_USE_ASSERTS)
#define ASSERT(exp) \
	do \
	{ \
		if (!(exp)) \
		{ \
			magma_assert::ReportFailure(#exp,__FILE__,__FUNCTION__,__LINE__, 0); \
			MAGMA_HALT(); \
		} \
	} \
	while(0)

#define ASSERT_MSG(exp,msg) \
	do \
	{ \
		if (!(exp)) \
		{ \
			magma_assert::ReportFailure(#exp,__FILE__,__FUNCTION__,__LINE__,msg); \
			MAGMA_HALT(); \
		} \
	} \
	while(0)

#define ASSERT_FAIL(msg) \
	ASSERT_MSG( 1 != 1, msg );

#define ASSERT_FIX_MSG( exp, msg ) \
	( ( !(exp) ) ? \
		magma_assert::ReportFailure(#exp,__FILE__,__FUNCTION__,__LINE__,msg), \
		true \
	: false )

#define ASSERT_FIX( exp ) \
	ASSERT_FIX_MSG( exp, 0 )

#define ASSERT_FIX_SLOW( exp ) \
	ASSERT_FIX( exp )

#define ASSERT_FIX_RETURN( exp ) \
	if ( ASSERT_FIX( exp ) ) return

#else
	#define ASSERT(exp) 
	#define ASSERT_MSG(exp,msg)
	#define ASSERT_FIX_MSG( exp, msg ) \
		( ( !exp ) ? true : false )
	#define ASSERT_FIX(exp) \
		ASSERT_FIX_MSG( exp, 0 )
	#define ASSERT_FIX_SLOW( exp ) false;
#endif


#endif
