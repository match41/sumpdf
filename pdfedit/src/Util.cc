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

// libpdfdoc headers
#include <graphics/Color.hh>
#include <graphics/GraphicsState.hh>
#include <util/Exception.hh>
#include <util/Matrix.hh>

// Qt headers
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QString>
#include <QTransform>
#include <QTextCodec>

// boost headers
#include <boost/bind.hpp>

// stdc++ headers
#include <algorithm>
#include <iterator>

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
	// gcc wchar_t has no problems
	// msvc only when the /Zc:wchar_t- (i.e. _NATIVE_WCHAR_T_DEFINED is NOT defined)
	// in these two cases we use std::wstring directly
#if (defined __GNUC__ || (defined _MSC_VER && !defined _NATIVE_WCHAR_T_DEFINED) )
	#ifndef QT_NO_STL
		return QString::fromStdWString( s ) ;
	#else
		return QString::fromWCharArray( s.c_str(), s.size() ) ;
	#endif
#else
	// msvc may not have the /Zc:wchar_t- option used, which Qt expects
	// avoid using wchar_t related functions
	QString result ;
	for ( std::wstring::const_iterator i = s.begin() ; i != s.end() ; ++i )
		result.push_back( QChar( *i ) ) ;
	return result ;
#endif
}

std::wstring ToWStr( const QString& s )
{
#if (defined __GNUC__ || (defined _MSC_VER && !defined _NATIVE_WCHAR_T_DEFINED) )
	#ifndef QT_NO_STL
		return s.toStdWString( ) ;
	#else
		std::wstring result( L' ', s.length() ) ;
		s.toWCharArray( &result[0] ) ;
		return result ;
	#endif
#else
    std::wstring result ;
    const ushort *utf16 = s.utf16() ;
    while ( *utf16 )
		result.push_back( *utf16++ ) ;
	return result ;
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

Color FromQColor( const QColor& color )
{
	Color result ;
	QColor c = color ;
	if ( c.spec() != QColor::Rgb && c.spec() != QColor::Cmyk )
		c = c.convertTo( QColor::Rgb ) ;
	
	switch ( c.spec() )
	{
		case QColor::Rgb:
			result.AssignRGB( c.red(), c.green(), c.blue() ) ;
			break;
		case QColor::Cmyk:
			result.AssignCMYK( c.cyan(), c.magenta(), c.yellow(), c.black() ) ;
			break ;
		default :
			break ;
	}
	return result ;
}

QColor ToQColor( const Color& c )
{
	QColor result ;
	switch ( c.ColorSpace() )
	{
		case Color::rgb :
			result.setRgbF( c.Red(), c.Green(), c.Blue() ) ;
			break ;
		case Color::cmyk :
			result.setCmykF( c.Cyan(), c.Magenta(), c.Yellow(), c.Black() ) ;
			break ;
		case Color::gray :
			result.setRgbF( c.Gray(), c.Gray(), c.Gray() ) ;
			break ;
		default :
			throw Exception( "unsupported colour space" ) ;
	}
	return result ;
}

QBrush MakeBrush( const GraphicsState& gs )
{
	// fill colour
	return QBrush( ToQColor( gs.NonStrokeColour() ) ) ;
}

QPen MakePen( const GraphicsState& gs )
{
	// fill colour
	return QPen( ToQColor( gs.StrokeColour() ) ) ;
}

} // end of namespace
