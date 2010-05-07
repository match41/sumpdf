/***************************************************************************\
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
	\file	InsertTextDlg.cc
	\brief	definition the InsertTextDlg class
	\date	March 21, 2010
*/

#include "InsertTextDlg.hh"

#include <QPushButton>
#include <QMenu>
#include <QPainter>
#include <QColorDialog>
#include <QTextEdit>
#include <QTextBlock>
#include <QString>
#include <QDebug>
#include <QTextLayout>

#include <QPaintEngine>
#include <QPaintDevice>
#include <QPainter>
#include <QTextItem>

#include <limits>

namespace pdf {

InsertTextDlg::InsertTextDlg( QWidget *parent )
	: QDialog( parent )
{
	setupUi( this ) ;
	
	// rename OK button to insert
	m_btn_insert->button(QDialogButtonBox::Ok)->setText("Insert");

	// set up the font size combo box
	for (int i = 8; i < 30; i = i + 2)
		m_fontsize->addItem( QString().setNum(i) );
	QIntValidator *validator = new QIntValidator( 2, 64, this );
	m_fontsize->setValidator( validator );

	// font color
	m_textcolor->setPopupMode( QToolButton::MenuButtonPopup );
	m_textcolor->setMenu(
		OnCreateColorMenu( SLOT( OnTextColorChanged() ), Qt::black ) );
    m_text_action = m_textcolor->menu()->defaultAction();
	m_textcolor->setIcon(
		OnCreateColorButtonIcon( ":/images/textpointer.png", Qt::black) );
	m_textcolor->setAutoFillBackground( true );
	// text color changed
	connect( m_textcolor,	SIGNAL( clicked() ), this, SLOT( OnFontChanged() ) );
	// text bold
	connect( m_bold,		SIGNAL( clicked() ), this, SLOT( OnFontChanged() ) );	
	// text italic
	connect( m_italic,		SIGNAL( clicked() ), this, SLOT( OnFontChanged() ) );

	connect(
		m_font,
		SIGNAL( currentFontChanged( QFont ) ),
		this ,
		SLOT( OnFontChanged( ) ) );
	connect(
		m_fontsize,
		SIGNAL( currentIndexChanged( int ) ),
		this,
		SLOT( OnFontChanged( ) ) );
}

InsertTextDlg::~InsertTextDlg( )
{
}

void InsertTextDlg::OnTextColorChanged( )
{
	m_text_action = qobject_cast<QAction *>( sender() );
	m_textcolor->setIcon(
		OnCreateColorButtonIcon(
		":/images/textpointer.png",
		qVariantValue<QColor>( m_text_action->data() ) ) );
	OnFontChanged();
}

void InsertTextDlg::OnSetColor( )
{
	QColor color = QColorDialog::getColor(Qt::green, this);
	m_textcolor->setIcon(
		OnCreateColorButtonIcon( ":/images/textpointer.png", color ) );
	m_text->setTextColor( color );
}
// drop-down text color selection menu
QMenu *InsertTextDlg::OnCreateColorMenu( const char *slot, QColor default_color )
{
	QList<QColor> colors;
	colors << Qt::black << Qt::green << Qt::red << Qt::blue << Qt::yellow;
	QStringList names;
	names << tr("black") << tr("green") << tr("red") 
		<< tr("blue") << tr("yellow");

	QMenu *color_menu = new QMenu;
	for ( int i = 0; i < colors.count(); ++i ) 
	{
		QAction *action = new QAction( names.at(i), this );
		action->setData( colors.at(i) );
		action->setIcon( OnCreateColorIcon( colors.at(i) ) );
		connect( action, SIGNAL( triggered() ), this, slot );
		color_menu->addAction( action );
		if (colors.at(i) == default_color) 
		{
			color_menu->setDefaultAction( action );
		}
	}        
	// chose color from QColorDialog
	QAction *action = new QAction( "... more", this );
	color_menu->addAction( action );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnSetColor() ) );

	return color_menu;
}

QIcon InsertTextDlg::OnCreateColorButtonIcon( 
	const QString &image_file,
	QColor color )
{
	QPixmap pixmap( 50, 80 );
	pixmap.fill( Qt::transparent );
	QPainter painter( &pixmap );
	QPixmap image( image_file );
	QRect target( 0, 0, 50, 60 );
	QRect source( 0, 0, 61, 70 ); // 42, 43);
	painter.fillRect( QRect( 0, 60, 50, 80 ), color );
	painter.drawPixmap( target, image, source );

	return QIcon( pixmap );
}

QIcon InsertTextDlg::OnCreateColorIcon( QColor color )
{
	QPixmap pixmap( 20, 20 );
	QPainter painter( &pixmap );
	painter.setPen( Qt::NoPen );
	painter.fillRect( QRect(0, 0, 20, 20), color);

	return QIcon( pixmap );
}

QString	InsertTextDlg::GetFontSize( )
{
	return m_fontsize->currentText( );
}

QTextEdit* InsertTextDlg::GetText( )
{
	return m_text;
}

class PdfPaintEngine : public QPaintEngine
{
public :
	bool begin( QPaintDevice * pdev )
	{
		return true ;
	}
	
	void drawPixmap( const QRectF & r, const QPixmap & pm, const QRectF & sr )
	{
	}
	
	bool end()
	{
		return true ;
	}
	
	void drawTextItem( const QPointF & p, const QTextItem& textItem )
	{
		qDebug() << "drawing " << textItem.text() << " at: " << p ;
	}
	
	Type type() const
	{
		return QPaintEngine::User ;
	}
	
	void updateState( const QPaintEngineState & state )
	{
	}
} ;

class PdfPaintDevice : public QPaintDevice
{
public :
	PdfPaintDevice( )
	{
	}

	QPaintEngine* paintEngine( ) const
	{
		return &m_engine ;
	}

protected :
	int metric( PaintDeviceMetric metric ) const
	{
		switch (metric)
		{
		case PdmWidth:		return 500 ;
		case PdmHeight:		return 500 ;
		case PdmWidthMM:	return static_cast<int>(500/72.0 * 2.54) ;
		case PdmHeightMM:	return static_cast<int>(500/72.0 * 2.54) ;
		case PdmNumColors:	return std::numeric_limits<int>::max() ;
		case PdmDepth:		return 24 ;
		case PdmDpiX:
		case PdmDpiY:
		case PdmPhysicalDpiX:
		case PdmPhysicalDpiY:	return 72 ;
		
		default :		return 0 ;
		}
	}

private :
	mutable PdfPaintEngine	m_engine ;
} ;

void InsertTextDlg::OnFontChanged( )
{
	PdfPaintDevice dev ;
	QPainter painter( &dev ) ;
	m_text->document()->drawContents( &painter ) ;
	
	QTextCursor cur=m_text->textCursor();	// get the current cursor

	QFont font = m_font->currentFont();
    font.setPointSize( m_fontsize->currentText().toInt() );
    font.setWeight(m_bold->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(m_italic->isChecked());
    // font.setUnderline(m_underlined->isChecked());
    m_text->setTextColor( qVariantValue<QColor>( m_text_action->data() ) );
	m_text->setCurrentFont ( font );

//	m_text->setText( m_text->toPlainText() );	// update text to apply new color
//	m_text->setTextCursor(cur);					// set cursor to intial state
}

} // end of namespace
