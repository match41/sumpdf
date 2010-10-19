/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	RealPage.cc
	\brief	implementation the RealPage class
	\date	Sat Mar 15 2008
	\author	Nestal Wan
*/

#include "RealPage.hh"

#include "ContentStream.hh"
#include "PageTree.hh"
#include "RealResources.hh"

// core object headers
#include "core/Array.hh"
#include "core/Object.hh"
#include "core/String.hh"

// other libpdfdoc headers
#include "file/File.hh"
#include "file/DictReader.hh"
#include "file/ElementPool.hh"
#include "graphics/Graphics.hh"
#include "graphics/GraphicsImpl.hh"
#include "util/Debug.hh"
#include "util/Rect.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace pdf {

RealPage::RealPage( PageTree *parent )
	: m_pinfo( parent )
{
	PDF_ASSERT( parent != 0 ) ;
	parent->AppendLeaf( this ) ;
}

void RealPage::Read( DictReader& dict )
{
	PDF_ASSERT( dict.GetFile() != 0 ) ;

	// read content
	Object contents ;
	if ( dict.Detach( "Contents", contents ) )
		ReadContent( contents, dict.GetFile() ) ;

	m_pinfo.Read( dict ) ;
}

RealPage::~RealPage( )
{
}

Rect RealPage::MediaBox( ) const
{
	return m_pinfo.MediaBox() ;
}

Rect RealPage::CropBox( ) const
{
	return m_pinfo.CropBox() ;
}

void RealPage::ReadContent( const Object& str_obj, File *src )
{
	PDF_ASSERT( src != 0 ) ;
	
	// for indirect objects, dereference it
	if ( str_obj.Is<Ref>( ) )
		ReadContent( src->ReadObj( str_obj ), src ) ;

	// append individual stream objects
	else if ( str_obj.Is<Stream>( ) )
		m_cstrs.push_back( str_obj.As<Stream>( ) ) ;

	// catenate individual objects in array
	else if ( str_obj.Is<Array>( ) )
	{
		const Array& a = str_obj.As<Array>( ) ;
		std::for_each(
			a.begin( ), a.end( ),
		    boost::bind( &RealPage::ReadContent, this, _1, src ) ) ;
	}

	else if ( !str_obj.Is<void>( ) )
		throw std::runtime_error( "invalid page content" ) ;
}

void RealPage::Write(
	const Ref& 				link,
	File 					*file,
	const Ref& 				parent,
	const FontSubsetInfo	*ss ) const
{
	PDF_ASSERT( file != 0 ) ;
	
	Dictionary self ;
	self.insert( "Type",		Name( "Page" ) ) ;
 	self.insert( "Contents",	WriteContent( file ) ) ;
	self.insert( "Parent",		parent ) ;
	
	m_pinfo.Write( self, file, ss ) ;

	file->WriteObj( self, link ) ;
}

Object RealPage::WriteContent( File *file ) const
{
	assert( file != 0 ) ;

	if ( m_cstrs.size() == 1 )
		return file->WriteObj( m_cstrs.front() ) ;
	
	else
	{
		Array strs( m_cstrs.size() ) ;
		std::transform(
			m_cstrs.begin(),
			m_cstrs.end(),
		    strs.begin(),
		    boost::bind( &File::WriteObj, file, _1 ) ) ;
		return strs ;
	}
}

void RealPage::Clear( )
{
	m_cstrs.clear( ) ;
}

std::size_t RealPage::Count( ) const
{
	return 1 ;
}

PageTree* RealPage::Parent( )
{
	return m_pinfo.Parent( ) ;
}

RealPage* RealPage::GetLeaf( std::size_t index )
{
	// we are a leaf node. we have no children but ourselves.
	return index == 0 ? this : 0 ;
}

RealResources* RealPage::Resource( )
{
	return m_pinfo.Resource() ;
}

const RealResources* RealPage::Resource( ) const
{
	return m_pinfo.Resource() ;
}

int RealPage::Rotation( ) const
{
	return m_pinfo.Rotation( ) ;
}

void RealPage::VisitGraphics( GraphicsVisitor *visitor ) const
{
	ContentStream cs(
		m_cstrs.begin(),
		m_cstrs.end(),
		m_pinfo.Resource(),
		visitor ) ;
	
	cs.Decode( ) ;
}

void RealPage::WriteGraphic(
	const Graphics	*gfx,
	std::ostream& 	os,
	GraphicsState& 	gs,
	RealResources	*res )
{
	PDF_ASSERT( gfx != 0 ) ;

	// no need to write one more space before "cm" because there will be a
	// space after printing a matrix
	if ( gfx->Transform() != Matrix() )
		os << "q " << gfx->Transform() << "cm\n" ;

	const GraphicsImpl *impl = gfx->QueryImplementation() ;
	if ( impl != 0 )
		impl->Print( os, res, gs ) ;
	
	if ( gfx->Transform() != Matrix() )
		os << "Q\n" ;
}

void RealPage::SetContent( const std::vector<Graphics*>& gfx )
{
	Stream str ;
	std::ostream os( str.OutStreamBuf() ) ;

	GraphicsState	gs ;

	// clear the resource dictionary first. the fonts/images will be
	// added to the resource dictionary when calling Graphics::Print().
	RealResources *res = m_pinfo.CreateNewResource() ;

	using namespace boost ;
	std::for_each(
		gfx.begin(),
		gfx.end(),
		bind( &RealPage::WriteGraphic, this, _1, ref(os), ref(gs), res ) ) ;

	os.flush() ;
	m_pinfo.ReplaceResource( res ) ;

	// throw away the existing resources and start over
	Clear( ) ;
	m_cstrs.push_back( str ) ;
}

void RealPage::GetRawContent( std::vector<unsigned char>& out ) const
{
	BOOST_FOREACH( const Stream& s, m_cstrs )
	{
		std::vector<unsigned char> buf ;
		s.CopyData( buf ) ;
		out.insert( out.end(), buf.begin(), buf.end() ) ;
	}
}

void RealPage::SetRawContent( const unsigned char *data, std::size_t size )
{
	Stream str ;
	str.Append( data, size ) ;
	str.Flush( ) ;
	
	// throw away the existing resources and start over
	Clear( ) ;
	m_cstrs.push_back( str ) ;
}

void RealPage::AddInlineImage( std::size_t width, std::size_t height,
		const std::string& file )
{
	std::ostringstream bi ;
	bi	<< width/2 << " 0 0 " << height/2 << " 0 0 cm\n"
		<< "BI\n"
		<< "/W "	<< width	<< "\n"
		<< "/H "	<< height	<< "\n"
		<< "/CS "	<< "/RGB"	<< "\n"
		<< "/BPC "	<< "8"		<< "\n"
		<< "/F "	<< "/DCT"	<< "\n"
		<< "ID\n" ;

	std::string bis = bi.str() ;

	Stream str ;
	str.Append( reinterpret_cast<unsigned char*>(&bis[0]), bis.size() ) ;
	
	// read the whole file and put it in the stream
	std::ifstream fstr( file.c_str(), std::ios::binary | std::ios::in ) ;
	std::vector<char> fdata(
		(std::istreambuf_iterator<char>( fstr ) ),
		(std::istreambuf_iterator<char>() ) ) ;
	
	str.Append( reinterpret_cast<unsigned char*>(&fdata[0]), fdata.size() ) ;
	
	// end of image marker
	str.Append( reinterpret_cast<const unsigned char*>("\nEI\n"), 4 ) ;
	
	str.Flush( ) ;
	
	// append the stream to the existing one
	m_cstrs.push_back( str ) ;
}

} // end of namespace
