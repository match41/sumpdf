/***************************************************************************
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

/**
	\file	ColorComboBox.hh
	\brief	definition the ColorComboBox class
	\date	May 6, 2010
*/

#include "ColorComboBox.hh"

#include <QColor>
#include <QPainter>
#include <QMenu>
#include <QColorDialog>
#include <QPaintEngine>
#include <QPaintDevice>


namespace pdf {

ColorComboBox::ColorComboBox( QWidget *parent )
	: QToolButton( parent )
{
}

void ColorComboBox::Initialize(QToolButton* box)
{
	box->setPopupMode( QToolButton::MenuButtonPopup );
	box->setMenu(
		OnCreateColorMenu( SLOT( OnTextColorChanged() ), Qt::black ) );
    m_text_action = box->menu()->defaultAction();
	box->setIcon(
		OnCreateColorButtonIcon( ":/images/textpointer.png", Qt::black) );
	box->setAutoFillBackground( true );
}

void ColorComboBox::OnTextColorChanged( )
{
	m_text_action = qobject_cast<QAction *>( sender() );
	this->setIcon(
		OnCreateColorButtonIcon(
		":/images/textpointer.png",
		qVariantValue<QColor>( m_text_action->data() ) ) );
	emit clicked( qVariantValue<QColor>( m_text_action->data() ) );
}

void ColorComboBox::OnSetColor( )
{
	QColor color = QColorDialog::getColor(Qt::green, this);
	this->setIcon(
		OnCreateColorButtonIcon( ":/images/textpointer.png", color ) );
	emit clicked( color );
}

// drop-down text color selection menu
QMenu *ColorComboBox::OnCreateColorMenu( const char *slot, QColor default_color )
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

QIcon ColorComboBox::OnCreateColorButtonIcon( 
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

QIcon ColorComboBox::OnCreateColorIcon( QColor color )
{
	QPixmap pixmap( 20, 20 );
	QPainter painter( &pixmap );
	painter.setPen( Qt::NoPen );
	painter.fillRect( QRect(0, 0, 20, 20), color);

	return QIcon( pixmap );
}

} // end of namespace