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

###############################################################################
# required packages
###############################################################################
set( Boost_USE_MULTITHREAD ON )
find_package( Qt4 REQUIRED )
find_package( Boost REQUIRED )
find_package( Freetype REQUIRED )

include(${QT_USE_FILE})

set( BASE_SRCS		src/main.cc
                    src/GlyphGraphicsItem.cc
                    src/GlyphGroup.cc
                    src/Util.cc
					src/MainWnd.cc
					src/PageView.cc
					src/TextEdit.cc
					src/PropertiesDlg.cc )

###############################################################################
# all sources
###############################################################################
set( SRCS	${BASE_SRCS} )

qt4_automoc( ${SRCS} )

set( MOC_SRCS	src/MainWnd.cc
				src/PropertiesDlg.cc
				src/PageView.cc
				src/TextEdit.cc )
set( MOC_HDRS	src/MainWnd.hh
				src/PropertiesDlg.hh
				src/PageView.hh
				src/TextEdit.hh )
set( UIS 		ui/MainWnd.ui
				ui/Properties.ui )

qt4_wrap_ui( UIH ${UIS})
qt4_wrap_cpp( MOC_SRCS ${MOC_HDRS})

add_executable( pdfedit WIN32 ${SRCS} ${UIH} ${MOC_SRCS} )
add_executable( qtest WIN32 src/test.cc )

###############################################################################
# unix specific libraries
###############################################################################
if ( UNIX )
	find_library( DL_LIBRARY 	NAMES dl	PATH /usr/lib /usr/lib64 )
	find_library( BFD_LIBRARY	NAMES bfd	PATH /usr/lib /usr/lib64 )
    set( OPT_LIBS   ${OPT_LIBS}	${DL_LIBRARY} ${BFD_LIBRARY} )
endif ( UNIX )

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
                                       ${Boost_INCLUDE_DIRS}
                                       ${FREETYPE_INCLUDE_DIRS}
                                       ${OPT_INCS} )

target_link_libraries( pdfedit	${PDFDOC_LIBRARY} ${OPT_LIBS} ${QT_LIBRARIES}
								${FREETYPE_LIBRARIES}
								boost_program_options )
target_link_libraries( qtest	${QT_LIBRARIES}
								${FREETYPE_LIBRARIES} )
								