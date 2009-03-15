/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
 ***************************************************************************/

/*!
	\file	StreamTest.cc
	\brief	implementation the StreamTest class
	\date	Tue Apr 1 2008
	\author	Nestal Wan
*/

#include "StreamTest.hh"

#include "core/Dictionary.hh"
#include "core/Stream.hh"

#include <sstream>

StreamTest::StreamTest( )
{
}

void StreamTest::TestRead( )
{
}

void StreamTest::TestWrite( )
{
	std::string str = "0 12 TD (string string) Tj" ;
	pdf::Stream stream( str ) ;
	
	std::stringstream ss ; 
	ss << stream ;

	pdf::Dictionary dict ;
	ss >> dict ;
	CPPUNIT_ASSERT( dict["Length"] == str.size( ) ) ;
}
