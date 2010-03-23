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
    \file	Matrix.hh
    \brief	definition the Matrix class
    \date	Jan 6, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_MATRIX_HEADER_INCLUDED__
#define __PDF_MATRIX_HEADER_INCLUDED__

#include "libpdfdoc.hh"

#include <iosfwd>

namespace pdf {

/**	\brief  2 dimention matrix.

	This class represents a 2D matrix. It stores six real numbers which
	are the first and second column of a 3x3 matrix:
	
<pre>
	a b 0
	c d 0
	e f 0
</pre>

*/
class LIBPDFDOC_API Matrix
{
public :
	Matrix( ) ;
	Matrix( double m11, double m12, double m21, double m22,
		double dx, double dy ) ;
	Matrix( const Matrix& m ) ;

	static Matrix Translation( double dx, double dy ) ;
	Matrix& Translate( double dx, double dy ) ;

	Matrix& operator=( const Matrix& m ) ;
	bool operator==( const Matrix& m ) const ;
	bool operator!=( const Matrix& m ) const ;
	Matrix& operator*=( const Matrix& m ) ;

	bool IsIdentity( ) const ;

	double M11() const ;
	double M12() const ;
	double M21() const ;
	double M22() const ;
	double Dx() const ;
	double Dy() const ;

	void M11( double val ) ;
	void M12( double val ) ;
	void M21( double val ) ;
	void M22( double val ) ;
	void Dx( double val ) ;
	void Dy( double val ) ;

	typedef double*			iterator ;
	typedef const double*	const_iterator ;
	
	iterator begin() ;
	iterator end() ;
	const_iterator begin() const ;
	const_iterator end() const ;
	
	double Det( ) const ;
	Matrix Inverse( bool *ok = 0 ) const ;

	bool IsTranslate( ) const ;

private :
	double	m_mat[6] ;
} ;

Matrix operator*( const Matrix& a, const Matrix& b ) ;
std::ostream& operator<<( std::ostream& os, const Matrix& m ) ;

} // end of namespace

#endif
