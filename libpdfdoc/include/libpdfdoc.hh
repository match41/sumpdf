/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
	\file	libpdfdoc.hh
	\brief	definition the libpdfdoc class
	\date	Fri Apr 11 2008
	\author	Nestal Wan
*/

#ifndef __LIB_PDF_DOC_HEADER_INCLUDED__
#define __LIB_PDF_DOC_HEADER_INCLUDED__

#include <string>

#define LIBPDFDOC_API 

/*!	\mainpage	libpdfdoc Project Home Page
	\section	intro	Introduction
	libpdfdoc is a library to access
	<a href="http://www.adobe.com/devnet/pdf/pdf_reference.html">PDF (Portable
	Document Format)</a>. It provides classes
	and functions for reading and writing to PDF files.
	
	\section	features	Features
	- read/write PDF files with the same API
	- not much right now
	
	\section	license		License
	libpdfdoc is release under
	<a href="http://www.gnu.org/licenses/lgpl-2.1.html">LGPL</a>.
	In short, it is <a href="http://www.gnu.org/philosophy/free-sw.html">Free
	Software.</a> I am not responsible for any damage it may make. You can
	change it, sell it,	distribute it. You can even use it in comercial
	projects. However, every changes you make to libpdfdoc MUST be made
	public if you distribute it.
	
	\section	get		Getting libpdfdoc
	libpdfdoc can be downloaded in from
	<a href="http://sourceforge.net/project/showfiles.php?group_id=14057">
	sourceforge</a>. The latest snapshot is also available in the
	<a href="http://libpdfxx.git.sourceforge.net/">git</a> repository hosted
	in sourceforge. To access the repository, configure you git clients to
	clone from git://libpdfxx.git.sourceforge.net/gitroot/libpdfxx.
	
	\section	depend	Dependencies
	libpdfdoc depends on the following libraries:
	- <a href="http://www.boost.org">The Boost Library</a>
	- <a href="http://zlib.net">Zlib</a>
	- <a href="http://www.freetype.org">Freetype</a> (Optional)
	- <a href="http://libpng.sourceforge.net/index.html">libpng</a> (Optional)
	- <a href="http://www.ijg.org/">libjpeg</a> (Optional)
	- <a href= 
	"http://apps.sourceforge.net/mediawiki/cppunit/index.php?title=Main_Page">
	cppunit</a> (Optional)
	
	\section	build	Building libpdfdoc
	libpdfdoc uses the <a href="http://www.cmake.org/">cmake</a> build system
	to produce makefiles for different development platforms. The cmake build
	system will automatically detect the location of the dependency libraries
	and adjust the include and library path of the project. In most unix-like
	environment, the command:

\verbatim
cmake -G 'Unix Makefiles' /path/to/libpdfdoc/
\endverbatim

	should be enough to generate the makefiles to build the project.
	Afterwards, just type \c make to build. cmake by default will not show the
	actual command line of the compiler. To see it, type this command:

\verbatim
VERBOSE=1 make
\endverbatim

	This will show the full command line of the compiler. It is useful for
	troubleshooting compiler issues.

	cmake also supports Microsoft Visual Studio. Just use a different makefile
	generator for the version of VS. For example, for VS 2005:
	
\verbatim
cmake -G 'Visual Studio 8 2005' c:\path\to\libpdfdoc\
\endverbatim

	You may have to add <tt>-DCMAKE_PREFIX_PATH=c:\path\to\libraries\</tt> to
	the cmake command line to make sure it can find the library dependencies.
	You can also use <tt>-DBOOST_ROOT=c:\path\to\boost\library</tt> to specify
	the path to the boost library.

\code
C:\VSS\make\libpdfdoc>cmake -G "Visual Studio 8 2005" -DBOOST_ROOT=c:\VSS\boost_
1_38_0 -DCMAKE_PREFIX_PATH=c:\VSS\zlib123-dll -DCPPUNIT_PREFIX=c:\VSS\cppunit-1.
12.0 ..\..\libpdfxx\libpdfdoc
\endcode

	<a href="http://sourceforge.net">
		<img src="http://sourceforge.net/sflogo.php?group_id=14057&amp;type=5"
	         width="210" height="62" border="0" alt="SourceForge Logo" />
	</a>
*/

/*
#ifdef DLL_EXPORTS
    #ifdef _MSC_VER
    	#define LIBPDFDOC_API __declspec( dllexport )
    #else
    	#define LIBPDFDOC_API __attribute__ ((visibility("default")))
    #endif
#else
    #ifdef _MSC_VER
	    #define LIBPDFDOC_API __declspec( dllimport )
	#else
		#define LIBPDFDOC_API 
	#endif
#endif
*/

/*!	\brief	main namespace for libpdfdoc

	This namespace contains almost everything in libpdfdoc.
*/
namespace pdf {

class Doc ;
class Page ;

LIBPDFDOC_API Doc*  CreateDoc( ) ;

} // end of namespace

#endif
