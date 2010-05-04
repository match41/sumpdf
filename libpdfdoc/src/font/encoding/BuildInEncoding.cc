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

/**	\file	BuildInEncoding.cc
	\brief	implementation of the BuildInEncoding class
	\date	May 4, 2010
	\author	Nestal Wan
*/

#include "BuildInEncoding.hh"

#include "CodeMap.hh"
#include "font/FontException.hh"

#include "core/Name.hh"
#include "core/Object.hh"

#include "util/Debug.hh"

#include <iostream>

namespace pdf {

namespace
{
	const char *winansi_map[256] =
	{
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		"space",
		"exclam",
		"quotedbl",
		"numbersign",
		"dollar",
		"percent",
		"ampersand",
		"quotesingle",
		"parenleft",
		"parenright",
		"asterisk",
		"plus",
		"comma",
		"hyphen",
		"period",
		"slash",
		"zero",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
		"colon",
		"semicolon",
		"less",
		"equal",
		"greater",
		"question",
		"at",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"bracketleft",
		"backslash",
		"bracketright",
		"asciicircum",
		"underscore",
		"grave",
		"a",
		"b",
		"c",
		"d",
		"e",
		"f",
		"g",
		"h",
		"i",
		"j",
		"k",
		"l",
		"m",
		"n",
		"o",
		"p",
		"q",
		"r",
		"s",
		"t",
		"u",
		"v",
		"w",
		"x",
		"y",
		"z",
		"braceleft",
		"bar",
		"braceright",
		"asciitilde",
		"bullet",
		"Euro",
		"bullet",
		"quotesinglbase",
		"florin",
		"quotedblbase",
		"ellipsis",
		"dagger",
		"daggerdbl",
		"circumflex",
		"perthousand",
		"Scaron",
		"guilsinglleft",
		"OE",
		"bullet",
		"Zcaron",
		"bullet",
		"bullet",
		"quoteleft",
		"quoteright",
		"quotedblleft",
		"quotedblright",
		"bullet",
		"endash",
		"emdash",
		"tilde",
		"trademark",
		"scaron",
		"guilsinglright",
		"oe",
		"bullet",
		"zcaron",
		"Ydieresis",
		"space",
		"exclamdown",
		"cent",
		"sterling",
		"currency",
		"yen",
		"brokenbar",
		"section",
		"dieresis",
		"copyright",
		"ordfeminine",
		"guillemotleft",
		"logicalnot",
		"hyphen",
		"registered",
		"macron",
		"degree",
		"plusminus",
		"twosuperior",
		"threesuperior",
		"acute",
		"mu",
		"paragraph",
		"periodcentered",
		"cedilla",
		"onesuperior",
		"ordmasculine",
		"guillemotright",
		"onequarter",
		"onehalf",
		"threequarters",
		"questiondown",
		"Agrave",
		"Aacute",
		"Acircumflex",
		"Atilde",
		"Adieresis",
		"Aring",
		"AE",
		"Ccedilla",
		"Egrave",
		"Eacute",
		"Ecircumflex",
		"Edieresis",
		"Igrave",
		"Iacute",
		"Icircumflex",
		"Idieresis",
		"Eth",
		"Ntilde",
		"Ograve",
		"Oacute",
		"Ocircumflex",
		"Otilde",
		"Odieresis",
		"multiply",
		"Oslash",
		"Ugrave",
		"Uacute",
		"Ucircumflex",
		"Udieresis",
		"Yacute",
		"Thorn",
		"germandbls",
		"agrave",
		"aacute",
		"acircumflex",
		"atilde",
		"adieresis",
		"aring",
		"ae",
		"ccedilla",
		"egrave",
		"eacute",
		"ecircumflex",
		"edieresis",
		"igrave",
		"iacute",
		"icircumflex",
		"idieresis",
		"eth",
		"ntilde",
		"ograve",
		"oacute",
		"ocircumflex",
		"otilde",
		"odieresis",
		"divide",
		"oslash",
		"ugrave",
		"uacute",
		"ucircumflex",
		"udieresis",
		"yacute",
		"thorn",
		"ydieresis"
	} ;
}

/**	constructor
	
*/
BuildInEncoding::BuildInEncoding( const Name& name )
	: m_enc( Parse( name ) )
{
	static const char **map[] = { winansi_map } ;
	
	if ( m_enc >= win && m_enc <= win )
	{
		Build( map[m_enc] ) ;
	}
}

void BuildInEncoding::Build( const char **map )
{
	for ( unsigned short i = 0 ; i < 256U ; ++i )
	{
		wchar_t unicode = 0 ;
		if ( map[i] != 0 && NameToUnicode( map[i], unicode ) )
		{
//std::cout << "code: " << i << " name: " << map[i] << " unicode: " << (int)unicode << std::endl ;
			Add( i, unicode ) ;
		}
	}
}

BuildInEncoding::Encoding BuildInEncoding::Parse( const Name& name )
{
	if ( name == "WinAnsiEncoding" )
		return win ;
	else if ( name == "MacRomanEncoding" )
		return macRoman ;
	else if ( name == "MacExpertEncoding" )
		return macExpert ;
	else if ( name == "StandardEncoding" )
		return standard ;
	else
		throw FontException( "invalid encoding" ) ;
}

Object BuildInEncoding::Write( File * ) const
{
	PDF_ASSERT( m_enc >= win && m_enc <= standard ) ;
	static const Name names[] =
	{
		"WinAnsiEncoding",
		"MacRomanEncoding",
		"MacExpertEncoding",
		"StandardEncoding"
	} ;
	return names[m_enc] ;
}

} // end of namespace
