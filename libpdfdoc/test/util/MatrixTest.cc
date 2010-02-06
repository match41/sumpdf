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

/**	\file	MatrixTest.cc
	\brief	implementation of the MatrixTest class
	\date	Jan 26, 2010
	\author	Nestal Wan
*/

#include "MatrixTest.hh"

#include "util/Matrix.hh"

#include "mock/Assert.hh"

using namespace pdf ;

/**	constructor
*/
MatrixTest::MatrixTest( )
{
}

void MatrixTest::TestDet( )
{
	Matrix m( 1,1,1,1,0,0 ) ;
	PDFUT_ASSERT_EQUAL( m.Det(), 0.0 ) ;
}

void MatrixTest::TestInv( )
{
	Matrix m( 2,0,0,2, 100, 300 ), m2( 1,0,0,1, 500, 300 ) ;
	
	Matrix r = m * m2 * m2.Inverse() ;
	PDFUT_ASSERT_EQUAL( r, m ) ;
}

void MatrixTest::TestMul( )
{
	Matrix m( 1,0,0,1, 100, 300 ), m2( 1,0,0,1, 500, 300 ) ;
	Matrix r = m * m2 ;
	PDFUT_ASSERT_EQUAL( r, Matrix( 1,0,0,1, 600, 600 ) ) ;
}
