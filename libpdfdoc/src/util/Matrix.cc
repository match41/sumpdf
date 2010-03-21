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
    \file	Matrix.cc
    \brief	definition the Matrix class
    \date	Jan 6, 2010
    \author	Nestal Wan
*/

#include "util/Matrix.hh"

#include "Util.hh"

#include <algorithm>
#include <iterator>
#include <ostream>

namespace pdf {

///	Constructs an identify matrix.
Matrix::Matrix( )
{
	m_mat[0] = m_mat[3] = 1 ;
	m_mat[1] = m_mat[2] = m_mat[4] = m_mat[5] = 0 ;
}

///	Constructs a matrix with values.
Matrix::Matrix( double m11, double m12, double m21, double m22,
	double dx, double dy )
{
	m_mat[0] = m11 ;
	m_mat[1] = m12 ;
	m_mat[2] = m21 ;
	m_mat[3] = m22 ;
	m_mat[4] = dx ;
	m_mat[5] = dy ;
}

Matrix::Matrix( const Matrix& m )
{
	std::copy( m.begin(), m.end(), begin() ) ;
}

Matrix Matrix::Translation( double dx, double dy )
{
	return Matrix( 1, 0, 0, 1, dx, dy ) ;
}

Matrix& Matrix::Translate( double dx, double dy )
{
	*this = Matrix( 1, 0, 0, 1, dx, dy ) * (*this) ;
	return *this ;
}

Matrix::iterator Matrix::begin()
{
	return Begin( m_mat ) ;
}

Matrix::iterator Matrix::end()
{
	return End( m_mat ) ;
}

Matrix::const_iterator Matrix::begin() const
{
	return Begin( m_mat ) ;
}

Matrix::const_iterator Matrix::end() const
{
	return End( m_mat ) ;
}

Matrix& Matrix::operator=( const Matrix& m )
{
	std::copy( m.begin(), m.end(), begin() ) ;
	return *this ;
}

double Matrix::M11() const
{
	return m_mat[0] ;
}

double Matrix::M12() const
{
	return m_mat[1] ;
}

double Matrix::M21() const
{
	return m_mat[2] ;
}

double Matrix::M22() const
{
	return m_mat[3] ;
}

double Matrix::Dx() const
{
	return m_mat[4] ;
}

double Matrix::Dy() const
{
	return m_mat[5] ;
}

void Matrix::M11( double val )
{
	m_mat[0] = val ;
}

void Matrix::M12( double val )
{
	m_mat[1] = val ;
}

void Matrix::M21( double val )
{
	m_mat[2] = val ;
}
void Matrix::M22( double val )
{
	m_mat[3] = val ;
}
void Matrix::Dx( double val )
{
	m_mat[4] = val ;
}
void Matrix::Dy( double val )
{
	m_mat[5] = val ;
}

Matrix operator*( const Matrix& a, const Matrix& b )
{
	return Matrix(
		a.M11() * b.M11() + a.M12() * b.M21(),				// m11
		a.M11() * b.M12() + a.M12() * b.M22(),				// m12
		a.M21() * b.M11() + a.M22() * b.M21(),				// m11
		a.M21() * b.M12() + a.M22() * b.M22(),				// m12
		a.Dx()  * b.M11() + a.Dy()  * b.M21() + b.Dx(),		// dx
		a.Dx()  * b.M12() + a.Dy()  * b.M22() + b.Dy() ) ;	// dy
}

bool Matrix::IsIdentity( ) const
{
	static const Matrix identity ;
	return *this == identity ;
}

bool Matrix::operator==( const Matrix& m ) const
{
	return std::equal( begin(), end(), m.begin() ) ;
}

bool Matrix::operator!=( const Matrix& m ) const
{
	return !operator==( m ) ;
}

std::ostream& operator<<( std::ostream& os, const Matrix& m )
{
	std::copy( m.begin(), m.end(), std::ostream_iterator<double>( os, " " ) ) ;
	return os ;
}

/// Calculate the determinant of the matrix
double Matrix::Det( ) const
{
	// M11 * M22 - M12 * M21
	return m_mat[0] * m_mat[3] - m_mat[1] * m_mat[2] ;
}

Matrix Matrix::Inverse( bool *ok ) const
{
	double det = Det( ) ;
	if ( det == 0.0 )
	{
		if ( ok )
			*ok = false ;
		return Matrix() ;
	}
	else
	{
		if ( ok )
			*ok = false ;
        
		double detinv = 1.0 / det ;
		return Matrix(
			m_mat[3]*detinv,
			-m_mat[1]*detinv,
			-m_mat[2]*detinv,
			m_mat[0]*detinv,
			(m_mat[2]*m_mat[5] - m_mat[3]*m_mat[4])*detinv,
			(m_mat[1]*m_mat[4] - m_mat[0]*m_mat[5])*detinv);
	}
}

bool Matrix::IsTranslate( ) const
{
	return
		m_mat[0] == 1.0 && m_mat[1] == 0.0 &&
		m_mat[2] == 0.0 && m_mat[3] == 1.0 ;
}

} // end of namespace
