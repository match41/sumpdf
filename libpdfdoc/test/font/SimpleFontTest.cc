/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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

/*!
	\file	SimpleFontTest.cc
	\brief	implementation the SimpleFontTest class
	\date	Sun Mar 8 2009
	\author	Nestal Wan
*/

#include "SimpleFontTest.hh"

#include "font/ftwrap/Library.hh"
#include "font/SimpleFont.hh"

#include <iostream>

SimpleFontTest::SimpleFontTest( )
{
}

void SimpleFontTest::TestSimple( )
{
	ft::Library lib ;
	pdf::SimpleFont subject( &lib, std::string(TEST_DATA_DIR) +
	                         "FreeMonoBoldOblique.ttf" ) ;
	CPPUNIT_ASSERT( subject.BaseName( ) == "FreeMonoBoldOblique" ) ;
}
