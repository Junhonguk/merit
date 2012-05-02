#include <QTableView>
#include <QStandardItemModel>
#include "standardprofileview.h"


StandardProfileView::StandardProfileView( QWidget *parent ) : QTableView( parent )
{
	stdTableView = new QTableView;
	QStandardItemModel model( 10, 2 );

	model.setHorizontalHeaderItem( 0, new QStandardItem( "Reference" ) );
	model.setHorizontalHeaderItem( 1, new QStandardItem( "Description" ) );
	for( int r=0; r<10; ++r )
	{
		QStandardItem *item = new QStandardItem( QString("Row %1").arg(r+1) );
		item->setEditable( false ); 
		model.setItem( r, 0, item );
		model.setItem( r, 1, new QStandardItem( QString::number((r*30)%100 )) );  
	}

	stdTableView->setModel(&model);
	stdTableView->show();
}

 StandardProfileView::~StandardProfileView()
{

}
