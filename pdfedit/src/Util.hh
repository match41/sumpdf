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

/**	\file	Util.hh
    \brief	definition the Util class
    \date	Jan 24, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_UTIL_HH_EADER_INCLUDED__
#define __PDF_UTIL_HH_EADER_INCLUDED__

#include <QString>

#include <string>

class QTransform ;
class QColor ;

namespace pdf
{
	class Matrix ;
	class Color ;
	
	QTransform ToQtMatrix( const Matrix& m ) ;
	Matrix FromQtMatrix( const QTransform& m ) ;

	std::wstring ToWStr( const QString& str ) ;
	QString FromWStr( const std::wstring& str ) ;

	std::string ToStr( const QString& str ) ;
	QString FromStr( const std::string& str ) ;

	template <typename T>
	QString operator%( QString s, T t )
	{
		return s.arg( t ) ;
	}

	Color FromQColor( const QColor& c ) ;
	QColor ToQColor( const Color& c ) ;

} // end of namespace

#endif // UTIL_HH_
