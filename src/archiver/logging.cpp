#include "logging.h"

using namespace logging;

#if defined( MAGMA_MSVC )
#include <Windows.h>

namespace logging
{
DebugWindowStream dout = DebugWindowStream();
}

int DebugWindowStream::DebugWindowStreamBuf::sync()
{
	OutputDebugString( str().c_str() );
	str("");
	return 0;
}

#endif

int MultiStream::MultiStreamBuf::sync()
{
	std::vector< std::ostream* >::iterator iter = m_cStreams.begin();
	for ( ; iter != m_cStreams.end(); ++iter )
	{
		*(*iter) << str();
		(*iter)->flush();
	}
	str("");
	return 0;
}