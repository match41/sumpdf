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
#include "graphics/RealTextLine.hh"

#include <map>

namespace pdf {

class Font ;
class Object ;
class ResourcesDict ;

///	brief description
/**	The RealText class represents a text object in a page. It is created for
	each BT/ET commands pair. It contains a number of text lines.
*/
class RealText : public Text
{
public :
	explicit RealText(
		const GraphicsState&	ts	= GraphicsState(),
		const Matrix&			ctm	= Matrix() ) ;

	void OnCommand( ContentOp& op, const ResourcesDict *res ) ;

	void Print(
		std::ostream&	os,
		ResourcesDict	*res,
		GraphicsState&	gs ) const ;

	const RealTextLine* At( std::size_t idx ) const ;
	RealTextLine* At( std::size_t idx ) ;
	
	RealTextLine& front() ;
	RealTextLine& back() ;
	const RealTextLine& front() const ;
	const RealTextLine& back() const ;
	
	std::size_t Count( ) const ;
	
	void AddLine( const TextLine *line ) ;
	void AddLine( double x, double y, const std::wstring& text ) ;
	
	void Visit( GraphicsVisitor *visitor ) ;

	bool operator==( const RealText& rhs ) const ;
	bool operator!=( const RealText& rhs ) const ;

	GraphicsState GetState() const ;

	Matrix Transform() const ;
	void Transform( const Matrix& mat ) ;

private :
	void AddLine( const RealTextLine& line ) ;
	
	/// command handler
	struct HandlerMap ;

	// position command handlers
	void OnTd( ContentOp& op, const ResourcesDict *res ) ;
	void OnTD( ContentOp& op, const ResourcesDict *res ) ;
	void OnTm( ContentOp& op, const ResourcesDict *res ) ;
	void OnTstar( ContentOp& op, const ResourcesDict *res ) ;
	
	// text show command handlers
	void OnTj( ContentOp& op, const ResourcesDict *res ) ;
	void OnTJ( ContentOp& op, const ResourcesDict *res ) ;
	void OnSingleQuote( ContentOp& op, const ResourcesDict *res ) ;
	void OnDoubleQuote( ContentOp& op, const ResourcesDict *res ) ;
	
	std::wstring DecodeString( const std::string& s, Font *font ) ;

	friend std::ostream& operator<<( std::ostream& os, const RealText& t ) ;

private :
	///	Lines inside this text object
	std::vector<RealTextLine>	m_lines ;
	
	///	Current graphics state
	GraphicsState	m_state ;
	
	///	Current text line matrix.
	double			m_offset ;
	
	///	Current text matrix.
	/**	This matrix is used to create new TextLine objects.
	*/
	Matrix			m_text_mat ;
	
	Matrix			m_transform ;
} ;

} // end of namespace

#endif // REALTEXT_HH_
