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

/**	\file	GraphicsState.hh
    \brief	definition the GraphicsState class
    \date	Feb 16, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GRAPHICSSTATE_HEADER_INCLUDED__
#define __PDF_GRAPHICSSTATE_HEADER_INCLUDED__

#include <boost/shared_ptr.hpp>

#include <iosfwd>
#include <vector>

namespace pdf {

class Color ;
class ColorValue ;
class ContentOp ;
class DictReader ;
class Font ;
class Name ;
class Object ;
class ResourcesDict ;
class Token ;
class TextState ;

///	The PDF graphics state.
/**	\ingroup graphics
	The graphics state is a set of graphics control parameters. These
	parameters define the way that the graphics operators execute. 
*/
class GraphicsState
{
public :
	enum PenCapStyle	{ butt_cap, round_cap, project_cap } ;
	enum LineJoinStyle	{ miter_join, round_join, bevel_join } ;

public :
	GraphicsState( ) ;
	explicit GraphicsState( const TextState& ts ) ;
	~GraphicsState( ) ;

	// use default generated copy constructor

	const TextState& Text() const ;
	TextState& Text() ;

	Font* FontFace( ) const ;

	std::ostream& Print(
		std::ostream&			os,
		ResourcesDict			*res,
		const GraphicsState&	prev = GraphicsState() ) const ;

	/// Handle PDF content operators.
	bool OnCommand( ContentOp& op, const ResourcesDict *res ) ;

	static bool IsGSCommand( const Token& cmd ) ;

	bool operator==( const GraphicsState& gs ) const ;
	bool operator!=( const GraphicsState& gs ) const ;

	friend std::ostream& operator<<(std::ostream& os, const GraphicsState& gs) ;

	bool LineWidth( double value ) ;
	double LineWidth( ) const ;

	bool PenCap( PenCapStyle value ) ;
	PenCapStyle PenCap( ) const ;
	
	bool LineJoin( LineJoinStyle value ) ;
	LineJoinStyle LineJoin( ) const ;
	
	bool MiterLimit( double value ) ;
	double MiterLimit( ) const ;

	void SetValue( const Name& name, const Object& val ) ;
	
	Color StrokeColor( ) const ;
	bool StrokeColor( const Color& color ) ;
	
	Color FillColor( ) const ;
	bool FillColor( const Color& color ) ;

	std::vector<int> DashPattern() const ;
	int DashPhrase( ) const ;

private :
	struct HandlerMap ;

	// text state command handlers
	bool OnTf( ContentOp& op, const ResourcesDict *res ) ;
	bool OnTL( ContentOp& op, const ResourcesDict *res ) ;
	
	// color space command handlers
	bool OnCS( ContentOp& op, const ResourcesDict *res ) ;
	bool Oncs( ContentOp& op, const ResourcesDict *res ) ;
	bool OnG( ContentOp& op, const ResourcesDict *res ) ;
	bool Ong( ContentOp& op, const ResourcesDict *res ) ;
	bool OnRG( ContentOp& op, const ResourcesDict *res ) ;
	bool Onrg( ContentOp& op, const ResourcesDict *res ) ;
	bool OnK( ContentOp& op, const ResourcesDict *res ) ;
	bool Onk( ContentOp& op, const ResourcesDict *res ) ;

	// graphics states command handlers
	bool Onw( ContentOp& op, const ResourcesDict *res ) ;
	bool OnJ( ContentOp& op, const ResourcesDict *res ) ;
	bool Onj( ContentOp& op, const ResourcesDict *res ) ;
	bool OnM( ContentOp& op, const ResourcesDict *res ) ;
	bool Ond( ContentOp& op, const ResourcesDict *res ) ;

	void CopyOnWrite( ) ;

	void PrintColors(
		std::ostream&			os,
		ResourcesDict			*res,
		const GraphicsState&	prev ) const ;

	enum ColorType { strk_color, fill_color } ;
	bool ChangeColor( ColorType type, const ColorValue& color ) ;
	bool SetColorSpace( ColorType type, const Name& cs,
		const ResourcesDict *res ) ;

private :
	struct Impl ;
	boost::shared_ptr<Impl>	m_impl ;
} ;

} // end of namespace

#endif // GRAPHICSSTATE_HH_
