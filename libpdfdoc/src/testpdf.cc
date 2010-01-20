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
#include "graphics/TextBlock.hh"
#include "page/Page.hh"
#include "page/PageContent.hh"

#include "util/Exception.hh"

#include <fontconfig/fontconfig.h>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>

int main( int argc, char **argv )
{
	FcPattern *sans = FcPatternBuild( NULL,
		FC_FAMILY,	FcTypeString, "Helvetica Lt Std",
//	    FC_SIZE,	FcTypeDouble, 12.0,
	    NULL ) ;

	FcResult result ;
	FcPattern *matched = FcFontMatch( 0, sans, &result);

	FcChar8 *filename2 ;
	if (FcPatternGetString (matched, FC_FILE, 0, &filename2) != FcResultMatch)
		std::cout << "oops" << std::endl ;

	int id ;
	if (FcPatternGetInteger (matched, FC_INDEX, 0, &id) != FcResultMatch)
		std::cout << "oops2" << std::endl ;
	  
	std::cout << "file is " << filename2 << " " << id << std::endl ; 

	return 0 ;
	pdf::Doc *doc = pdf::CreateDoc( ) ;
	if ( argc >= 2 )
		doc->Read( argv[1] ) ;
	
	pdf::Page *p = doc->AppendPage( ) ;
	pdf::Font *f = doc->CreateSimpleFont( "Helvetica" ) ;
	p->DrawText( 100, 100, f, "Hello world!" ) ;
	p->DrawText( 100, 200, f, "This is the second line!" ) ;
	p->Finish( ) ;
	
	pdf::PageContent *c = p->GetContent( ) ;
	pdf::Text *t = c->AddText( ) ;
	pdf::TextBlock& b = *t->begin()->begin() ;
	b.SetText( L"wahaha" ) ;
	
	pdf::DocInfo *info = doc->Info() ;
	info->SetCreator( "Haha" ) ;
	
	doc->Write( "test.pdf" ) ;
	delete doc ;

	return 0 ;
}
