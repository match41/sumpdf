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

/**	\file	DocModel.hh
    \brief	definition the DocModel class
    \date	Mar 27, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_DOCMODEL_HEADER_INCLUDED__
#define __PDF_DOCMODEL_HEADER_INCLUDED__

#include <QObject>

#include <memory>
#include <vector>

class QString ;
class QGraphicsScene ;

namespace pdf {

class Doc ;
class Page ;

///	brief description
/**	\internal
	The DocModel class represents
*/
class DocModel : public QObject
{
	Q_OBJECT

public :
	explicit DocModel( QObject *parent = 0 ) ;
	
	void OpenFile( const QString& filename ) ;
	void SaveFile( const QString& filename ) ;
	void New( ) ;

	QGraphicsScene* CurrentScene() ;
	QGraphicsScene* GoToPage( std::size_t idx ) ;

	Doc* Document( ) ;
	std::size_t CurrentPage( ) const ;

signals :
	void SelectionChanged() ;

public slots :
	void OnSelectionChanged() ;

private :
	void ReplaceDocument( Doc *doc ) ;
	void StorePage( QGraphicsScene *scene, Page *page ) ;

private :
	std::auto_ptr<Doc>				m_doc ;
	std::vector<QGraphicsScene*>	m_pages ;
	std::size_t						m_current_page ;
} ;

} // end of namespace

#endif // DOCMODEL_HH_
