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

/**
    \file	Assert.hh
    \brief	definition the Assert class
    \date	Jan 7, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_ASSERT_HH_EADER_INCLUDED__
#define __PDF_ASSERT_HH_EADER_INCLUDED__

#include <cppunit/TestAssert.h>

namespace pdf {

	template <typename T1, typename T2>
	void AssertEquals(
		const T1&				expected,
		const T2& 				actual,
		CPPUNIT_NS::SourceLine	sourceLine,
		const std::string&		message )
	{
		if ( expected != actual )
		{
			CPPUNIT_NS::Asserter::failNotEqual(
				CPPUNIT_NS::assertion_traits<T1>::toString(expected),
				CPPUNIT_NS::assertion_traits<T2>::toString(actual),
				sourceLine,
				message );
		}
	}

	inline void AssertEquals(
		const std::wstring&		expected,
		const std::wstring& 	actual,
		CPPUNIT_NS::SourceLine	sourceLine,
		const std::string&		message )
	{
		AssertEquals(
			std::string(expected.begin(), expected.end() ),
			std::string(actual.begin(), actual.end() ),
			sourceLine,
			message ) ;
	}
	
} // end of namespace

#define PDFUT_ASSERT_EQUAL(actual, expected)          \
  ( pdf::AssertEquals( (expected),              \
                       (actual),                \
                       CPPUNIT_SOURCELINE(),    \
                       #actual" == "#expected) )

#define PDFUT_ASSERT_NULL(actual)          \
  ( pdf::AssertEquals( ((void*)0),              \
                       (actual),                \
                       CPPUNIT_SOURCELINE(),    \
                       #actual" != 0" ) )

#endif // ASSERT_HH_
