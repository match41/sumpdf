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
    \file	TextFormat.hh
    \brief	definition the TextFormat class
    \date	Jan 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TEXTFORMAT_HH_EADER_INCLUDED__
#define __PDF_TEXTFORMAT_HH_EADER_INCLUDED__

#include <cstddef>
#include <map>

namespace pdf {

class Font ;
class Token ;
class Object ;
class Resources ;

/**	\brief	brief description

	The TextFormat class represent
*/
class TextFormat
{
public :
	TextFormat( ) ;

	void OnCommand(
		const Token& 	cmd,
		Object 			*args,
		std::size_t		count,
		Resources		*res ) ;

	double Leading( ) const ;
	void SetLeading( double val ) ;
	
	void SetFont( double size, Font *font ) ;

private :
	typedef void (TextFormat::*Handler)(
		Object			*args,
		std::size_t		count,
		Resources		*res ) ;
	typedef std::map<Token, Handler>	HandlerMap ;

private :
	double	m_char_space ;
	double	m_word_space ;
	double	m_hori_scale ;
	double	m_leading ;
	double	m_font_size ;
	Font	*m_font ;
	int		m_render_mode ;
	double	m_text_rise ;
} ;

} // end of namespace

#endif // TEXTFORMAT_HH_
