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

#ifndef __PDF_REALTEXT_HH_EADER_INCLUDED__
#define __PDF_REALTEXT_HH_EADER_INCLUDED__

#include "graphics/Text.hh"

#include "core/Token.hh"
#include "graphics/TextState.hh"
#include "graphics/TextLine.hh"

#include <map>

namespace pdf {

class Object ;
class Resources ;

///	brief description
/**	The RealText class represents
*/
class RealText : public Text
{
public :
	RealText( ) ;

	void OnCommand(
		const Token& 	cmd,
		Object 			*args,
		std::size_t		count,
		Resources		*res ) ;

	void Output( std::ostream& os ) const ;

	iterator begin() ;
	iterator end() ;
	
	const_iterator begin() const ;
	const_iterator end() const ;
	
	std::size_t Count( ) const ;
	
	void AddLine( const TextLine& line ) ;

	void Visit( GraphicsVisitor *visitor ) ;

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

	void OnTf( Object* args, std::size_t count, Resources *res ) ;

	void AddNewLine( ) ;
	void AddNewLine( const Matrix& mat ) ;

private :
	std::vector<TextLine>	m_lines ;
	
	///	Current text line matrix. 
	Matrix		m_line_mat ;
	
	///	Current text state
	TextState	m_state ;
} ;

} // end of namespace

#endif // REALTEXT_HH_
