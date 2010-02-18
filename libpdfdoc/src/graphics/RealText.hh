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

/**	\file	RealText.hh
    \brief	definition the RealText class
    \date	Jan 13, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REALTEXT_HEADER_INCLUDED__
#define __PDF_REALTEXT_HEADER_INCLUDED__

#include "graphics/Text.hh"

#include "core/Token.hh"
#include "graphics/GraphicsState.hh"
#include "graphics/TextLine.hh"

#include <map>

namespace pdf {

class Object ;
class Resources ;

///	brief description
/**	The RealText class represents a text object in a page. It is created for
	each BT/ET commands pair. It contains a number of text lines.
*/
class RealText : public Text
{
public :
	explicit RealText( const GraphicsState& ts = GraphicsState() ) ;
	explicit RealText( const TextState& ts ) ;

	void OnCommand(
		const Token& 	cmd,
		Object 			*args,
		std::size_t		count,
		Resources		*res ) ;

	void Print( std::ostream& os, const Resources *res ) const ;

	iterator begin() ;
	iterator end() ;
	
	const_iterator begin() const ;
	const_iterator end() const ;
	
	TextLine& front() ;
	TextLine& back() ;
	TextLine& at( std::size_t idx ) ;
	const TextLine& front() const ;
	const TextLine& back() const ;
	const TextLine& at( std::size_t idx ) const ;

	std::size_t Count( ) const ;
	
	void AddLine( const TextLine& line ) ;
	void AddLine( double x, double y, const std::wstring& text ) ;
	
	void Visit( GraphicsVisitor *visitor ) ;

	bool operator==( const RealText& rhs ) const ;
	bool operator!=( const RealText& rhs ) const ;

	GraphicsState GetState() const ;

private :
    /// command handler
	typedef void (RealText::*Handler)(
		Object			*args,
		std::size_t		count,
		Resources		*res ) ;
	typedef std::map<Token, Handler>	HandlerMap ;

	static const HandlerMap::value_type	m_handler_map_values[] ;
	static const HandlerMap				m_handler_map ;

    // position command handlers
	void OnTd( Object* args, std::size_t count, Resources *res ) ;
	void OnTD( Object* args, std::size_t count, Resources *res ) ;
	void OnTm( Object* args, std::size_t count, Resources *res ) ;
	void OnTstar( Object* args, std::size_t count, Resources *res ) ;
	
	// text show command handlers
	void OnTj( Object* args, std::size_t count, Resources *res ) ;
	void OnTJ( Object* args, std::size_t count, Resources *res ) ;
	void OnSingleQuote( Object* args, std::size_t count, Resources *res ) ;
	void OnDoubleQuote( Object* args, std::size_t count, Resources *res ) ;

	// text state command handlers
	void OnTf( Object* args, std::size_t count, Resources *res ) ;
	void OnTL( Object* args, std::size_t count, Resources *res ) ;

private :
	///	Lines inside this text object
	std::vector<TextLine>	m_lines ;
	
	///	Current graphics state
	GraphicsState	m_state ;
	
	///	Current text line matrix.
	/**	This matrix is only affect by the Td/TD and Tm commands. It will be
		changes when showing text.
	*/
	Matrix		m_line_mat ;
	
	///	Current text matrix.
	/**	This matrix is used to create new TextLine objects.
	*/
	Matrix		m_text_mat ;
} ;

std::ostream& operator<<( std::ostream& os, const RealText& t ) ;

} // end of namespace

#endif // REALTEXT_HH_
