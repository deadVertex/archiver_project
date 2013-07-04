#ifndef __LOGGING_H__
#define __LOGGING_H__

#include "platform.h"

#include <ostream>
#include <sstream>
#include <vector>

namespace logging
{
#if defined( MAGMA_MSVC )
/*
DebugWindowStream

An output stream class which writes to the Visual Studio debug output window.
Note: 
	- There is no need to have more than one of these.
	- Only outputs when std::endl is outputted.
*/
class DebugWindowStream : public std::ostream
{
private:
	class DebugWindowStreamBuf : public std::stringbuf
	{
	public:
		virtual int sync();
	};

	DebugWindowStreamBuf m_cStreamBuf;

public:
	DebugWindowStream() : std::ostream( &m_cStreamBuf ) {}
};

extern DebugWindowStream dout;

#endif

/*
MultiStream

A simple output stream which tries to forward its output to any of its
registered streams.
Note: Only outputs when std::endl is outputted.
*/
class MultiStream : public std::ostream
{
private:
	class MultiStreamBuf : public std::stringbuf
	{
	public:
		virtual int sync();

		void AddStream( std::ostream *pStream )
		{ m_cStreams.push_back( pStream ); }

	private:
		std::vector< std::ostream* > m_cStreams;
	};

	MultiStreamBuf m_cStreamBuf;

public:
	MultiStream() : std::ostream( &m_cStreamBuf ) {}

	void AddStream( std::ostream *pStream )
	{ m_cStreamBuf.AddStream( pStream ); }
};

}

#endif