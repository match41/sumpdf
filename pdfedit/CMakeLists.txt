#***************************************************************************#
 #   Copyright (C) 2002 by Nestal Wan                                      *
 #   me@nestal.net                                                         *
 #                                                                         *
 #   This program is free software; you can redistribute it and/or modify  *
 #   it under the terms of the GNU General Public License as published by  *
 #   the Free Software Foundation; either version 2 of the License.        *
 #                                                                         *
 #   This program is distributed in the hope that it will be useful,       *
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 #   GNU General Public License for more details.                          *
 #                                                                         *
 #   You should have received a copy of the GNU General Public License     *
 #   along with this program; if not, write to the                         *
 #   Free Software Foundation, Inc.,                                       *
 #   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
#***************************************************************************#

PROJECT(pdfedit)

cmake_minimum_required(VERSION 2.6)

set( VERSION	0.1.2 )
add_definitions( -DVERSION="${VERSION}" )

###############################################################################
# required packages
###############################################################################
set( Boost_USE_MULTITHREAD ON )
find_package( Qt4 REQUIRED )
find_package( Boost COMPONENTS program_options )
find_package( ZLIB REQUIRED )
find_package( Freetype REQUIRED )

# we don't want to let the boost header to choose the libraries for us
add_definitions( -DBOOST_ALL_NO_LIB )

include(${QT_USE_FILE})

set( BASE_SRCS		src/main.cc
                    src/GlyphGraphicsItem.cc
                    src/Util.cc
					src/MainWnd.cc
					src/PageLoader.cc
					src/PageView.cc
					src/TextEdit.cc
					src/PropertiesDlg.cc
					src/ExceptionDlg.cc
					src/TextDlg.cc
					src/InsertTextDlg.cc
					src/ToolBox.cc
					src/DocModel.cc
					src/PathObject.cc
                    src/TextObject.cc
					src/GraphicsObject.cc
					src/QtGraphicsState.cc )

###############################################################################
# all sources
###############################################################################
set( SRCS	${BASE_SRCS} )

qt4_automoc( ${SRCS} )

set( MOC_HDRS	src/MainWnd.hh
				src/PropertiesDlg.hh
				src/PageView.hh
				src/TextEdit.hh
				src/ExceptionDlg.hh
				src/TextDlg.hh 
				src/InsertTextDlg.hh
				src/ToolBox.hh
				src/DocModel.hh )
set( UIS 		ui/MainWnd.ui
				ui/Properties.ui
				ui/ExceptionDlg.ui
				ui/TextDlg.ui 
				ui/InsertTextDlg.ui )

qt4_wrap_ui( UIH ${UIS})
qt4_wrap_cpp( MOC_SRCS ${MOC_HDRS})

qt4_add_resources( RCC_SRCS ui/pdfedit.qrc )

add_executable( pdfedit WIN32 ${SRCS} ${UIH} ${MOC_SRCS} ${RCC_SRCS} )

###############################################################################
# finding optional fontconfig library
###############################################################################
find_path( FONTCONFIG_INCLUDE_DIR
	fontconfig/fontconfig.h
	/usr/include
	/usr/local/include
	${FONTCONFIG_PREFIX}/include )

find_library( FONTCONFIG_LIBRARY_DEBUG NAMES fontconfig
	PATHS
		/usr/lib
    	/usr/lib64
        /usr/local/lib
        /usr/local/lib64
        ${FONTCONFIG_PREFIX}/lib
	PATH_SUFFIXES debug )

find_library( FONTCONFIG_LIBRARY_RELEASE NAMES fontconfig
	PATHS
		/usr/lib
    	/usr/lib64
        /usr/local/lib
        /usr/local/lib64
        ${FONTCONFIG_PREFIX}/lib
	PATH_SUFFIXES release )

set( FONTCONFIG_LIBRARY debug	${FONTCONFIG_LIBRARY_DEBUG}
                     optimized	${FONTCONFIG_LIBRARY_RELEASE} )

if ( FONTCONFIG_INCLUDE_DIR AND FONTCONFIG_LIBRARY )
	message( STATUS "found fontconfig" )
	set( FONTCONFIG_FOUND TRUE )
	set( OPT_INCS	${OPT_INCS}
	                ${FONTCONFIG_INCLUDE_DIR} )
	add_definitions( -DHAVE_FONTCONFIG )
	set( OPT_LIBS   ${OPT_LIBS} ${FONTCONFIG_LIBRARY} )	
endif ( FONTCONFIG_INCLUDE_DIR AND FONTCONFIG_LIBRARY )

###############################################################################
# unix specific libraries
###############################################################################
if ( UNIX )
	find_library( DL_LIBRARY 	NAMES dl	PATH /usr/lib /usr/lib64 )
	find_library( BFD_LIBRARY	NAMES bfd	PATH /usr/lib /usr/lib64 )
    set( OPT_LIBS   ${OPT_LIBS}	${DL_LIBRARY} ${BFD_LIBRARY} )
endif ( UNIX )

###############################################################################
# win32 specific libraries
###############################################################################
if ( WIN32 )
	find_library( DBHELP_LIBRARY NAMES DbgHelp
	PATHS
		/usr/lib
    	/usr/lib64
        /usr/local/lib
        /usr/local/lib64 )
	
	if ( DBHELP_LIBRARY )
		message( "found dbghelp: ${DBHELP_LIBRARY}" )
		set( OPT_LIBS   ${OPT_LIBS}	Dbghelp )
	else ( DBHELP_LIBRARY )
		message( "dbghelp not found" )
	endif ( DBHELP_LIBRARY )
endif ( WIN32 )

# MSVC has stupid warnings on STD
if ( MSVC )
	add_definitions( -D_SCL_SECURE_NO_WARNINGS )
	set_target_properties( pdfedit
		PROPERTIES COMPILE_FLAGS "/wd4345 /wd4819 /Zc:wchar_t-" )
endif ( MSVC )

###############################################################################
# optional boost library
###############################################################################
if ( Boost_PROGRAM_OPTIONS_FOUND )
	add_definitions( -DHAVE_BOOST_PROGRAM_OPTIONS )
    set( OPT_LIBS   ${OPT_LIBS} ${Boost_PROGRAM_OPTIONS_LIBRARY} )
endif ( Boost_PROGRAM_OPTIONS_FOUND )

###############################################################################
# pdfdoc library
###############################################################################
set( PDFDOC_PREFIX    ../libpdfdoc )

find_path( PDFDOC_INCLUDE_DIR Doc.hh /usr/include
                                     /usr/local/include
                                     ${PDFDOC_PREFIX}/include )

find_library( PDFDOC_LIBRARY_DEBUG NAMES pdfdoc pdfdoc_static
              PATHS /usr/lib
                    /usr/lib64
                    /usr/local/lib
                    /usr/local/lib64
                    ${PDFDOC_PREFIX}/lib
              PATH_SUFFIXES debug )

find_library( PDFDOC_LIBRARY_RELEASE NAMES pdfdoc pdfdoc_static
              PATHS /usr/lib
                    /usr/lib64
                    /usr/local/lib
                    /usr/local/lib64
                    ${PDFDOC_PREFIX}/lib
              PATH_SUFFIXES release )

set( PDFDOC_LIBRARY debug     ${PDFDOC_LIBRARY_DEBUG}
                    optimized ${PDFDOC_LIBRARY_RELEASE} )

if ( PDFDOC_INCLUDE_DIR AND PDFDOC_LIBRARY )
	message( STATUS "found pdfdoc" )
	set( PDFDOC_FOUND TRUE )
	set( OPT_INCS	${OPT_INCS}
	                ${PDFDOC_INCLUDE_DIR} )
endif ( PDFDOC_INCLUDE_DIR AND PDFDOC_LIBRARY )

# in order to include the generated UI header files
include_directories( ${QT_INCLUDE_DIR} ${pdfedit_SOURCE_DIR}/src
                                       ${pdfedit_SOURCE_DIR}
                                       ${pdfedit_BINARY_DIR}
                                       ${Boost_INCLUDE_DIRS}
                                       ${FREETYPE_INCLUDE_DIRS}
                                       ${OPT_INCS} )

target_link_libraries( pdfedit	${PDFDOC_LIBRARY} ${OPT_LIBS} ${QT_LIBRARIES}
								${QT_QTMAIN_LIBRARY}
								${ZLIB_LIBRARIES}
								${FREETYPE_LIBRARIES} )
								