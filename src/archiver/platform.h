#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <SDL2/SDL_config.h>
#include <SDL2/SDL_stdinc.h>

// For custom compile time warnings
#define GEN_PREPROCESSOR_STRING_IMPL(x) #x
#define GEN_PREPROCESSOR_STRING(x) GEN_PREPROCESSOR_STRING_IMPL(x)


// Check For Microsoft Visual C++ compiler
#if defined(_MSC_VER)
	#define FILE_LINE_LINK __FILE__ "(" GEN_PREPROCESSOR_STRING(__LINE__) ") : "
	#define __MESSAGE(msg) __pragma( messsage(__FILE__ "(" GEN_PREPROCESSOR_STRING(__LINE__) ")" msg) )
	#define WARN(exp) (FILE_LINE_LINK "WARNING: " exp)

	#define MAGMA_MSVC	

#pragma warning(disable: 4996) // Disable warning for using unsafe versions of C functions.
#pragma warning(disable: 4201) // Disable warning for nameless struct used in union.
#pragma warning(disable: 4099) // Disable warning for .pdb file of debuging static external libraries not being found.
#pragma warning(disable: 4530) // Disable warning the C++ exception handler used but unwind semantics not enabled.

// Check for G++ compiler
#elif defined(__GNUC__)
	#define WARN(exp) ("WARNING: " exp)

	#define MAGMA_GNU

#endif

#if defined( MAGMA_64BIT )
#elif defined( MAGMA_32BIT )
#else
	#error "Must define either MAGMA_64BIT or MAGMA_32BIT"
#endif

typedef Uint8 Byte;

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#endif
