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

/**	\file	test.cc
	\brief	implementation of the test class
	\date	Jan 17, 2010
	\author	Nestal Wan
*/

#include <QApplication>
#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPathItem>
#include <QDebug>
#include <iostream>

#include <ft2build.h>
#include FT_IMAGE_H
#include FT_OUTLINE_H
#include FT_CACHE_H
#include FT_ERRORS_H

class GlyphWidget : public QMainWindow
{
public :
	GlyphWidget( int ch )
		: m_scene( new QGraphicsScene( QRectF( 0, 0, 1000, 700 ), this ) ),
		  m_view( new QGraphicsView( m_scene, this ) ),
		  m_glyph( new QGraphicsPathItem )
	{
		setCentralWidget( m_view ) ;
		
		FT_Init_FreeType( &m_ft ) ;
		char file[] = "PLYBKC+NimbusRomNo9L-Regu.ttf" ;
		FTC_Manager_New( m_ft, 0, 0, 0, &GlyphWidget::LoadFace, file, &m_cache ) ;
		FT_Error e = FTC_Manager_LookupFace( m_cache, 0, &m_face ) ;
std::cout << "em = " << m_face->units_per_EM << std::endl ;
		FTC_CMapCache cmap ;
		FTC_CMapCache_New( m_cache, &cmap);
		int idx = FTC_CMapCache_Lookup(cmap, 0, 0, 'e') ;
std::cout << "index = " << FTC_CMapCache_Lookup(cmap, 0, 0, 'e') << std::endl ;
		
		FTC_ImageCache img ;
		FTC_ImageCache_New( m_cache, &img ) ;
		
	
std::cout << "error = " << e << " " << m_face << std::endl ;
//		FT_New_Face(
//			m_ft,
//			"PLYBKC+NimbusRomNo9L-Regu.ttf",
//			0,
//			&m_face ) ; 
		
//		FT_Set_Char_Size( m_face, 0, 16*64, 90, 90 ) ;
		FTC_ImageTypeRec imgr = {0, 0, 0, FT_LOAD_NO_SCALE} ;

		FT_Glyph g ;
		e = FTC_ImageCache_Lookup( img, &imgr, idx, &g, 0 ) ; 

//		e = FT_Load_Glyph( m_face, FT_Get_Char_Index(m_face, ch), FT_LOAD_NO_SCALE );
std::cout << "error = " << std::hex << e << std::endl;
		
		m_scene->setSceneRect( 0, 0,
			m_face->glyph->metrics.horiBearingX+m_face->glyph->metrics.width,
			m_face->glyph->metrics.height ) ; 
//	}
//
//	void paintEvent( QPaintEvent *e )
//	{
//		QPainter p( this ) ;
		QPainterPath path( QPointF(0, 0) ) ;

		FT_Outline_Funcs f =
		{
			&GlyphWidget::MoveTo,
			&GlyphWidget::LineTo,
			&GlyphWidget::QuadTo,
			&GlyphWidget::CubicTo,
			0, 0
		} ;
		
		Render r = { this, &path } ;
		
		FT_OutlineGlyph og = (FT_OutlineGlyph)g ;
		
		FT_Outline_Decompose( &og->outline, &f, &r ) ;
		m_glyph->setPath( path ) ;
//qDebug() << m_face->units_per_EM ;
//		double scalefactor = 16.0 / m_face->units_per_EM ;
//		m_glyph->scale ( scalefactor, -scalefactor );
//		m_glyph->scale ( 1, -1 );
//		
//		m_glyph->translate( 0, -100 ) ;
		m_scene->addItem( m_glyph ) ;
//		m_view->scale( 2, 2 ) ;
		m_view->setRenderHint( QPainter::Antialiasing ) ;
	}
	
private :
	static FT_Error LoadFace(
		FTC_FaceID  face_id,
	    FT_Library  library,
	    FT_Pointer  request_data,
	    FT_Face*    aface )
	{
		return FT_New_Face(
			library,
			(char*)request_data,
			0,
			aface ) ; 
	}

	struct Render
	{
		GlyphWidget		*pthis ;
		QPainterPath	*painter ;
	} ;

	QPointF Transform( const FT_Vector *p ) const
	{
		return QPointF( p->x, p->y ) ;
	}

	static int MoveTo( const FT_Vector* to, void *user )
	{
		Render *r = reinterpret_cast<Render*>( user ) ;
		return r->pthis->MoveTo( to, r->painter ) ;
	}

	int MoveTo( const FT_Vector* to, QPainterPath *p )
	{
		p->moveTo( Transform( to ) ) ;
		return 0 ;
	}

	static int LineTo( const FT_Vector* to, void *user )
	{
		Render *r = reinterpret_cast<Render*>( user ) ;
		return r->pthis->LineTo( to, r->painter ) ;
	}

	int LineTo( const FT_Vector* to, QPainterPath *p )
	{
		p->lineTo( Transform( to ) ) ;
		return 0 ;
	}

	static int QuadTo(
		const FT_Vector	*control,
		const FT_Vector	*to,
		void 			*user )
	{
		Render *r = reinterpret_cast<Render*>( user ) ;
		return r->pthis->QuadTo( control, to, r->painter ) ;
	}

	int QuadTo(
		const FT_Vector	*control,
		const FT_Vector	*to,
		QPainterPath 	*p)
	{
		p->quadTo(
			Transform( control ),
			Transform( to ) ) ;
		return 0 ;
	}

	static int CubicTo(
		const FT_Vector	*control1,
		const FT_Vector	*control2,
		const FT_Vector	*to,
		void 			*user )
	{
		Render *r = reinterpret_cast<Render*>( user ) ;
		return r->pthis->CubicTo( control1, control2, to, r->painter ) ;
	}

	int CubicTo(
		const FT_Vector	*control1,
		const FT_Vector	*control2,
		const FT_Vector	*to,
		QPainterPath	*p )
	{
		p->cubicTo(
			Transform( control1 ),
			Transform( control2 ),
			Transform( to ) ) ;
		return 0 ;
	}

private :
	FT_Library	m_ft ;
	FTC_Manager	m_cache ;
	FT_Face		m_face ;
	
	QGraphicsScene	*m_scene ;
	QGraphicsView	*m_view ;
	QGraphicsPathItem	*m_glyph ;
} ;

int main( int argc, char **argv )
{
	QApplication app( argc, argv ) ;
	GlyphWidget g( 'e' ) ;
	g.show( ) ;
	return app.exec( ) ;
}

