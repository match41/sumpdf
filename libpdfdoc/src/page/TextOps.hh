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

/*!
	\file	TextOps.hh
	\brief	definition the TextSegment class
	\date	Mon Mar 2 2009
	\author	Nestal Wan
*/

#ifndef __PDF_TEXT_OPS_HEADER_INCLUDED__
#define __PDF_TEXT_OPS_HEADER_INCLUDED__

// stdc++ headers
#include <iosfwd>
#include <vector>
#include <string>

namespace pdf
{
    class Font ;
    class Object ;

    template <typename Op>
    struct ArgCount
    {
    	enum { value = 0 } ;
    } ;

	enum OpCode
	{
		char_space, word_space, text_scale, text_leading, text_render,
		text_rise, text_font, text_position, text_string, text_pos_string,
		
		end_text, begin_text
	} ;
	
	struct CharSpace
	{
		double val ;
	} ;
	template <> struct ArgCount<CharSpace> { enum { value = 1 } ; } ; 
	
	struct WordSpace
	{
		double val ;
	} ;
	template <> struct ArgCount<WordSpace> { enum { value = 1 } ; } ; 
	
	struct TextScale
	{
		double val ;
	} ;
	template <> struct ArgCount<TextScale> { enum { value = 1 } ; } ; 

	struct TextLeading
	{
		double val ;
	} ;
	template <> struct ArgCount<TextLeading> { enum { value = 1 } ; } ; 
	
	struct TextRise
	{
		double val ;
	} ;
	template <> struct ArgCount<TextRise> { enum { value = 1 } ; } ; 
	
	struct TextFont
	{
		Font    *font ;
		double  size ;
		TextFont( Font *f, double s ) : font( f ), size( s ) {}
	} ;
	template <> struct ArgCount<TextFont> { enum { value = 2 } ; } ; 
	
	struct TextRender
	{
		int	mode ;
	} ;
	template <> struct ArgCount<TextRender> { enum { value = 1 } ; } ; 
	
	struct TextPosition
	{
		double offx ;
		double offy ;
		TextPosition( double x, double y ) : offx(x), offy(y) {}
	} ;
	template <> struct ArgCount<TextPosition> { enum { value = 2 } ; } ; 
	
	struct TextMatrix
	{
		double mat[6] ;
		TextMatrix( double a, double b, double c,
		            double d, double e, double f )
		{
			mat[0] = a ;
			mat[1] = b ;
			mat[2] = c ;
			mat[3] = d ;
			mat[4] = e ;
			mat[5] = f ;
		}
	} ;
	template <> struct ArgCount<TextMatrix> { enum { value = 6 } ; } ; 
	
	struct TextString
	{
		std::string	str ;
	} ;
	template <> struct ArgCount<TextString> { enum { value = 1 } ; } ; 

	struct TextPosString
	{
	} ;

	struct EndText { } ;
	struct BeginText { } ;

} // end of namespace

#endif
