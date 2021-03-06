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

/**	\file	TestBase.hh
    \brief	definition the TestBase class
    \date	Jan 23, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TESTBASE_HH_EADER_INCLUDED__
#define __PDF_TESTBASE_HH_EADER_INCLUDED__

#include <cppunit/TestFixture.h>

namespace pdf
{
	class FontDb ;
}

namespace pdfut {

class MockFontDb ;

///	brief description
/**	this class represents
*/
class TestBase : public CppUnit::TestFixture
{
public :
	TestBase( ) ;

public :
	void setUp( ) ;
	void tearDown( ) ;

protected :
	pdf::FontDb	*m_font_db ;
	MockFontDb	*m_mock_fdb ;
} ;

} // end of namespace

#endif // TESTBASE_HH_
