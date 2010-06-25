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

#include "Doc.hh"
#include "DocInfo.hh"
#include "libpdfdoc.hh"
#include "font/Font.hh"
#include "graphics/Text.hh"
#include "graphics/TextLine.hh"
#include "graphics/TextState.hh"
#include "page/Page.hh"

#include "util/Backtrace.hh"
#include "util/Matrix.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <cmath>

int main( int argc, char **argv )
{
	pdf::Doc *doc = pdf::CreateDoc( ) ;
	if ( argc >= 2 )
	{
		try
		{
			doc->Read( argv[1] ) ;
		}
		catch ( std::exception& e )
		{
			std::cout << e.what() ;
			return -1 ;
		}
	}
	return 0 ;
	
//	pdf::Page *p = doc->AppendPage( ) ;
	pdf::Page *p = doc->GetPage( 0 ) ;
	if ( argc >= 3 )
	{
		std::ifstream file( argv[2] ) ;
		std::vector<unsigned char> buf(
			(std::istreambuf_iterator<char>( file )),
			(std::istreambuf_iterator<char>()) ) ;
		
		p->SetRawContent( &buf[0], buf.size() ) ;
	}
	else
	{
		std::string test_font =
			TEST_DATA_DIR + std::string("FreeMonoBoldOblique.ttf") ;

		std::ifstream file(test_font.c_str(), std::ios::binary | std::ios::in) ;
		std::vector<unsigned char> buf(
			(std::istreambuf_iterator<char>( file )),
			(std::istreambuf_iterator<char>()) ) ;

		pdf::Font *f = doc->CreateSimpleFont( &buf[0], buf.size() ) ;
		
		pdf::TextState ts ;
		ts.ChangeFont( 12.0, f ) ;
		pdf::Text *t = pdf::CreateText( pdf::GraphicsState(ts) ) ;
		t->AddLine( 0, 0, L"Hello world!" ) ;
		
//		const double pi = 3.1415926 ;
//		pdf::Matrix mat(
//			std::cos( pi/4), -std::sin(pi/4),
//			std::sin( pi/4),  std::cos(pi/4),
//			0, 0 ) ;
//		t->Transform( mat ) ;
		pdf::Matrix mat(
//			24.20442, 20.931938, -20.931938, 24.20442, 141.968289, 428.170339 ) ;
			24.20442, 20.931938, -20.931938, 24.20442, 0, 0 ) ;
		t->Transform( mat ) ;
		
		std::vector<pdf::Graphics*> gfx( 1, t ) ;
		p->SetContent( gfx ) ;
	}
	
	doc->Write( "test.pdf" ) ;
	delete doc ;

	return 0 ;
}
