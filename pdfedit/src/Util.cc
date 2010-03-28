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

/**	\file	Util.cc
	\brief	implementation of the Util class
	\date	Jan 24, 2010
	\author	Nestal Wan
*/

#include "Util.hh"

#include <util/Matrix.hh>

#include <QString>
#include <QTransform>
#include <QTextCodec>

namespace pdf {

QTransform ToQtMatrix( const Matrix& m )
{
	return QTransform( m.M11(), m.M12(), m.M21(), m.M22(), m.Dx(), m.Dy() ) ;
}

Matrix FromQtMatrix( const QTransform& m )
{
	return Matrix( m.m11(), m.m12(), m.m21(), m.m22(), m.dx(), m.dy() ) ;
}

QString FromWStr( const std::wstring& s )
{
#if defined __GNUC__ && !defined QT_NO_STL
	// gcc wchar_t has no problems
	return QString::fromStdWString( s ) ;
#elif defined MSVC
	// msvc may not have the /Zc:wchar_t- option used, which Qt expects
	// avoid using wchar_t related functions
	return QString::fromStdWString( s ) ;
#endif
}

std::wstring ToWStr( const QString& s )
{
#if defined __GNUC__ && !defined QT_NO_STL
    return s.toStdWString( ) ;
#elif defined MSVC
    return s.toStdWString( ) ;
#endif
}

std::string ToStr( const QString& str )
{
	QByteArray qba = str.toUtf8() ;
	return std::string( qba.constData(), qba.size() ) ;
}

QString FromStr( const std::string& str )
{
	return QString::fromUtf8( str.c_str(), str.size() ) ;
}

} // end of namespace
