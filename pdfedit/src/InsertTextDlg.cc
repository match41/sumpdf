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

namespace pdf {

InsertTextDlg::InsertTextDlg( QWidget *parent )
	: QDialog( parent )
{
	setupUi( this ) ;

	// set up the font size combo box
    for (int i = 8; i < 30; i = i + 2)
        m_fontsize->addItem(QString().setNum(i));
	QIntValidator *validator = new QIntValidator(2, 64, this);
    m_fontsize->setValidator(validator);

	// font color
 //   m_textcolor = new QToolButton;
    m_textcolor->setPopupMode(QToolButton::MenuButtonPopup);
    m_textcolor->setMenu(OnCreateColorMenu(SLOT(OnTextColorChanged()),
                                                 Qt::black));
    m_text_action = m_textcolor->menu()->defaultAction();
    m_textcolor->setIcon(OnCreateColorToolButtonIcon(
    ":/images/textpointer.png", Qt::black));
    m_textcolor->setAutoFillBackground(true);

	connect(
		m_textcolor, 
		SIGNAL( clicked( ) ),
		this, 
		SLOT( OnFontChanged( ) ) );



	connect(
		m_font,
		SIGNAL( currentFontChanged( QFont ) ),
		this ,
		SLOT( EmitFontChanged( QFont ) ) );
	connect(
		m_fontsize,
		SIGNAL( currentIndexChanged( int ) ),
		this ,
		SLOT( EmitFontChanged( int ) ) );

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
	connect(	// Insert text into current scene
		m_btn_insert, 
		SIGNAL( clicked( ) ),	
		this, 
		SLOT( OnInsertTextNow( ) ) );
}

InsertTextDlg::~InsertTextDlg( )
{
}

void InsertTextDlg::OnTextColorChanged()
{
    m_text_action = qobject_cast<QAction *>(sender());
    m_textcolor->setIcon(OnCreateColorToolButtonIcon(
                ":/images/textpointer.png",
                qVariantValue<QColor>(m_text_action->data())));
    OnFontChanged();
}

QMenu *InsertTextDlg::OnCreateColorMenu(const char *slot, QColor default_color)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::green << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("green") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *color_menu = new QMenu;
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(OnCreateColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()),
                this, slot);
        color_menu->addAction(action);
        if (colors.at(i) == default_color) {
            color_menu->setDefaultAction(action);
        }
    }
    return color_menu;
}

QIcon InsertTextDlg::OnCreateColorToolButtonIcon(const QString &image_file,
                        QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(image_file);
    QRect target(0, 0, 50, 60);
    QRect source(0, 0, 42, 42);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}

QIcon InsertTextDlg::OnCreateColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}

void InsertTextDlg::OnMousePositionSet( QPointF new_pos )
{
	pos = new_pos;
}

QString	InsertTextDlg::GetFontSize( )
{
	return m_fontsize->currentText( );
}

QTextEdit* InsertTextDlg::GetText( )
{
	return m_text;
}

QPointF	InsertTextDlg::GetPosition( )
{
	return pos;
}

void InsertTextDlg::OnFontChanged( )
{
    QFont font = m_font->currentFont();
    font.setPointSize(m_fontsize->currentText().toInt());
//    font.setWeight(m_action_bold->isChecked() ? QFont::Bold : QFont::Normal);
//    font.setItalic(m_action_italic->isChecked());
//    font.setUnderline(m_action_underline->isChecked());
    m_text->setTextColor(qVariantValue<QColor>(m_text_action->data()));

    m_text->setFont(font);

}

void InsertTextDlg::OnInsertTextNow( )
{
	emit OnInsertClicked( );
}

void InsertTextDlg::EmitFontChanged( QFont f )
{
	emit FontPropertiesChanged( f );
}

void InsertTextDlg::EmitFontChanged( int i )
{
	emit FontPropertiesChanged( i );
	focusNextChild();
}

void InsertTextDlg::SetFontChanged( QFont f)
{
	m_font->setCurrentFont( f );
}

void InsertTextDlg::SetFontChanged( int i )
{
	m_fontsize->setCurrentIndex( i );
}

void InsertTextDlg::closeEvent( QCloseEvent *e )
{
	QDialog::closeEvent( e );
	emit OnDlgClosed( );
}

} // end of namespace
