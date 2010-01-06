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

namespace pdf {

Matrix::Matrix( )
{
	m_mat[0] = m_mat[3] = 1 ;
	m_mat[1] = m_mat[2] = m_mat[4] = m_mat[5] = 0 ;
}

Matrix::Matrix( double a, double b, double c, double d, double e, double f )
{
	m_mat[0] = a ;
	m_mat[1] = b ;
	m_mat[2] = c ;
	m_mat[3] = d ;
	m_mat[4] = e ;
	m_mat[5] = f ;
}

Matrix::Matrix( const Matrix& m )
{
	std::copy( m.begin(), m.end(), begin() ) ;
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

} // end of namespace
