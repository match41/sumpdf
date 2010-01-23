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
    \file	TextBlock.hh
    \brief	definition the TextBlock class
    \date	Jan 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TEXTBLOCK_HH_EADER_INCLUDED__
#define __PDF_TEXTBLOCK_HH_EADER_INCLUDED__

#include "TextState.hh"

#include <iosfwd>
#include <string>

namespace pdf {

///	brief description
/**	The TextBlock class represent a string of characters with the same
	format.
*/
class TextBlock
{
public :
	explicit TextBlock(
		const std::string&	text	= std::string(),
		const TextState&	format	= TextState() ) ;

	void SetText( const std::wstring& text ) ;
	void AppendText( const std::wstring& text ) ;

	const TextState& Format() const ;
	void SetFormat( const TextState& fmt ) ;

	const std::wstring& Text() const ; 

	bool IsEmpty() const ;

	double Width( ) const ;

	std::ostream& Print(
		std::ostream& 		os,
		const TextState& 	state,
		Resources			*res ) const ;

	bool operator==( const TextBlock& rhs ) const ;
	bool operator!=( const TextBlock& rhs ) const ;
	
private :
	std::wstring	m_chars ;
	TextState		m_format ;
} ;

} // end of namespace

#endif // TEXTBLOCK_HH_
