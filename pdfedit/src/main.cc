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
	\file	main.cc
	\brief	entry point of pdfedit
	\date	Dec 27, 2009
	\author	Nestal Wan
*/

#include "MainWnd.hh"

#ifdef HAVE_BOOST_PROGRAM_OPTIONS
	#include <boost/program_options/options_description.hpp>
	#include <boost/program_options/variables_map.hpp>
	#include <boost/program_options/parsers.hpp>
#endif

// Qt headers
#include <QApplication>

#include <util/Exception.hh>
#include <iostream>

#ifdef HAVE_BOOST_PROGRAM_OPTIONS
namespace po = boost::program_options ;
#endif

int main( int argc, char **argv )
{
    QApplication app(argc, argv);

#ifdef HAVE_BOOST_PROGRAM_OPTIONS
    std::string path ;
    std::size_t	page = 0 ;
    
	// Declare the supported options.
	po::options_description desc( "Allowed options" ) ;
	desc.add_options()
    	(	"help",		"produce help message")
    	(	"input,i",	po::value<std::string>(&path),
    		"open input file" )
    	(	"page,p",	po::value<std::size_t>(&page),
    		"page number to open" )
	;

	po::variables_map vm;
	po::store(po::parse_command_line( argc, argv, desc ), vm ) ;
	po::notify(vm);
#endif

    pdf::MainWnd *w = new pdf::MainWnd ;
    w->resize( 800, 600 ) ;
    w->show( ) ;
    
#ifdef HAVE_BOOST_PROGRAM_OPTIONS
    if ( vm.count( "input" ) )
    	w->OpenFile( path.c_str() ) ;
    
    // humans use one-base index; C uses zero-base
    if ( vm.count( "page" ) )
    	w->GoToPage( page - 1 ) ;
#else
    if ( argc > 1 )
        w->OpenFile( argv[1] ) ;
#endif

    return app.exec( ) ;
}
