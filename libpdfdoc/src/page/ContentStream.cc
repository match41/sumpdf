/***************************************************************************\
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2.                              *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
\***************************************************************************/

/**	\file	ContentStream.cc
	\brief	implementation of the ContentStream class
	\date	Feb 20, 2010
	\author	Nestal Wan
*/

#include "ContentStream.hh"

#include "core/Object.hh"
#include "core/Token.hh"
#include "core/TokenSrc.hh"
#include "graphics/GraphicsState.hh"
#include "graphics/GraphicsVisitor.hh"
#include "graphics/RealText.hh"
#include "stream/Stream.hh"

#include "util/Debug.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>

#include <algorithm>

namespace pdf {

struct ContentStream::HandlerMap
{
	/// command handler
	typedef void (ContentStream::*Handler)(
		Object			*args,
		std::size_t		count ) ;
	typedef std::map<Token, Handler>	Map ;

	static const Map::value_type	m_val[] ;
	static const Map				m_map ;
} ;

const ContentStream::HandlerMap::Map::value_type
	ContentStream::HandlerMap::m_val[] =
{
	// text state commands
	std::make_pair( "BT",	&ContentStream::OnBT ),
	std::make_pair( "ET",	&ContentStream::OnET ),
	std::make_pair( "cm",	&ContentStream::Oncm ),
	std::make_pair( "Q",	&ContentStream::OnQ ),
	std::make_pair( "q",	&ContentStream::Onq ),
} ;


const ContentStream::HandlerMap::Map ContentStream::HandlerMap::m_map(
    Begin( ContentStream::HandlerMap::m_val ),
    End( ContentStream::HandlerMap::m_val ) ) ;

/**	constructor
	
*/
void ContentStream::Decode( )
{
	std::for_each(
		m_strs.begin(),
		m_strs.end(),
		boost::bind( &ContentStream::Decode, this, _1 ) ) ;
}

void ContentStream::Decode( Stream& str )
{
	PDF_ASSERT( m_res != 0 ) ;
	PDF_ASSERT( m_current == 0 ) ;

	// rewind to stream start for reading
	str.Rewind( ) ;
	
//	std::ostringstream ss ;
//	str.CopyData( ss.rdbuf() ) ;
//	std::cout << ss.str() << std::endl ;
//	str.Rewind() ;
	
	std::istream s( str.InStreamBuf() ) ;
	TokenSrc src( s ) ;
	std::vector<Object> args ;

	GraphicsState	gstate ;

	while ( true )
	{
		Token  cmd ;
		Object obj ;

		if ( src >> obj )
		{
			// swapping is faster
			args.push_back( Object() ) ;
			obj.Swap( args.back() ) ;
		}
		
		// if it is not an object, then it should be a command operator
		else
		{
			src.ResetState( ) ;
			if ( src >> cmd )
			{
/*std::cout << cmd.Get() << " " ;
std::copy( args.begin(), args.end(), std::ostream_iterator<Object>( std::cout, " " ) ) ;
std::cout << std::endl ;
*/

//				current = ProcessCommand(
//					cmd,
//					args.empty() ? 0 : &args[0],	// don't touch args[0]
//					args.size(),					// if empty.
//					gstate,
//					current,
//					res ) ;

				args.clear( ) ;
			}
			else
				break ;
		}
	}
}

void ContentStream::ProcessCommand(
	const Token& 	cmd,
	Object 			*args,
	std::size_t 	count )
{
	if ( m_current != 0 )
		m_current->OnCommand( cmd, args, count, m_res ) ;
	else
	{
		HandlerMap::Map::const_iterator i = HandlerMap::m_map.find( cmd ) ;
		if ( i != HandlerMap::m_map.end() )
		{
			PDF_ASSERT( i->second != 0 ) ;
			(this->*(i->second))( args, count ) ;
		}
	}
}

void ContentStream::OnBT( Object *, std::size_t )
{
	if ( m_current == 0 )
		m_current = new RealText( m_state ) ;
}

void ContentStream::OnET( Object *, std::size_t )
{
	if ( m_current != 0 )
	{
		m_state = m_current->GetState( ) ;
		
		m_gfx.push_back( m_current ) ;
		m_current = 0 ;
	}
}

void ContentStream::Oncm( Object *args, std::size_t count )
{
}

void ContentStream::OnQ( Object *args, std::size_t count )
{
}

void ContentStream::Onq( Object *args, std::size_t count )
{
}

} // end of namespace
