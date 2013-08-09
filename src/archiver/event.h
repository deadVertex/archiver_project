#ifndef __SYSTEM_EVENT_H__
#define __SYSTEM_EVENT_H__

/**********************************************************
	File: system_event.h
	Date Created: 28/5/2013
	Date Last Modified: 2/6/2013 22:40
	Author: David Goosen

	ChangeLog:
		- 2/6/2013 22:40
			Finished documenting.
**********************************************************/

#include <queue>
#include <vector>
#include <algorithm>
#include "platform.h"
#include "base_assert.h"

namespace event_types
{
enum Event_t
{
	UNKNOWN,
	TEST,
	INPUT,
	QUIT,
	HTML_EVENT,
};
}

/*
Event

Basic event class which all other events are derived from.
*/
class Event
{
public:
	Event( event_types::Event_t nType ) : m_nType( nType ),
		m_nDeliveryTime( 0 ) {}
	event_types::Event_t GetType() const { return m_nType; };
	void SetDeliveryTime( Uint32 nTicks ) { m_nDeliveryTime = nTicks; }
	Uint32 GetDeliveryTime() const { return m_nDeliveryTime; }

protected:
	event_types::Event_t m_nType;
	Uint32 m_nDeliveryTime;
};

template< typename T >
inline const T* event_cast( const Event *pEvent )
{
	return static_cast< const T* >( pEvent );
}

/*
EventHandler

An abastract class which all other event handlers are derived from.
*/
class EventHandler
{
public:
	virtual void HandleEvent( const Event *pEvent ) = 0;
};

/*
EventQueue

Encapsulates the queueing and dispatcing of events as well as the
hooking up of event handers to receive the events.
*/
class EventQueue
{
private:
	typedef std::pair< const Event*, bool > QueueEntry_t;

public:
	// Queue an event.
	// Note: The event will not be freed once it is popped from the queue.
	void QueueEvent( const Event *pEvent );

	// Dispatch the given event to all registered event handlers.
	void DispatchEvent( const Event *pEvent );

	// Register an event handler.
	void AddHandler( EventHandler *p ) { m_cHandlers.push_back( p ); }

	// Dispatch all events that are currently in the queue.
	void ProcessEvents( Uint32 curTime );

	// Queue an event object of the given type.
	// Note: The created event will be freed once it is popped from the queue.
	template< class T >
	T* QueueEvent();

private:
	std::queue< QueueEntry_t > m_cEvents;
	std::vector< EventHandler* > m_cHandlers;
};

// QueueEvent Implementation
inline void EventQueue::QueueEvent( const Event *pEvent )
{ 
	ASSERT( pEvent );
	m_cEvents.push( QueueEntry_t( pEvent, false ) ); 
}

// DispatchEvent Implementation
inline void EventQueue::DispatchEvent( const Event *pEvent )
{
	for ( Uint32 i = 0; i < m_cHandlers.size(); ++i )
	{
		m_cHandlers[ i ]->HandleEvent( pEvent );
	}
}

// ProcessEvent implementation
inline void EventQueue::ProcessEvents( Uint32 curTime )
{
	std::queue< QueueEntry_t > queue;
	while( !m_cEvents.empty() )
	{
		if ( m_cEvents.front().first->GetDeliveryTime() > curTime )
		{
			queue.push( m_cEvents.front() );
		}
		else
		{
			DispatchEvent( m_cEvents.front().first );
			if ( m_cEvents.front().second )
				delete m_cEvents.front().first;
		}
		m_cEvents.pop();
	}

	m_cEvents = queue;
}

// QueueEvent implementation
template< class T >
T* EventQueue::QueueEvent()
{
	T *p = new T();
	ASSERT( p );
	m_cEvents.push( QueueEntry_t( p, true ) );
	return p;
}

#endif