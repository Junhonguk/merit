
#include <QScrollBar>
#include <QGridLayout>
#include <QLabel>

#include "demandscenarioview.h"
#include "demandprofile.h"

DemandScenarioView::DemandScenarioView( QWidget *parent ) : QAbstractItemView( parent )
{

	gridLayout = new QGridLayout( this->viewport() );
//	horizontalLayout = new QHBoxLayout;
    
//	label = new QLabel;
	widget = new DemandProfile;

//	label->setAlignment( Qt::AlignCenter );
//	label->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
//	label->setText( tr("<i>No data.</i>") );

//	mdiArea->setAlignment( Qt::AlignCenter );
	widget->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
	

//	horizontalLayout->addWidget(mdiArea);
//	horizontalLayout->addWidget(label);
	gridLayout->addWidget(widget);

//	dp = new DemandProfile;
//	mdiArea->addSubWindow(dp);
//	mdiArea->setViewMode(QMdiArea::TabbedView);

//	dp->LoadStandardDemandList();
//	dp->show();
	
//	connect(dp->addMiddleButton, SIGNAL(clicked()), dp->listProcessProfilesView, SLOT(updateView()));

}
 DemandScenarioView::~DemandScenarioView()
{
//	delete mdiArea;
//	delete gridLayout;
}
QRect DemandScenarioView::visualRect( const QModelIndex &index ) const
{
  if( selectionModel()->selection().indexes().count() != 1 )
    return QRect();
    
  if( currentIndex() != index )
    return QRect();
    
  return rect();  
}

QRegion DemandScenarioView::visualRegionForSelection( const QItemSelection &selection ) const
{
  if( selection.indexes().count() != 1 )
    return QRect();
    
  if( currentIndex() != selection.indexes()[0] )
    return QRect();
    
  return rect();
}

bool DemandScenarioView::isIndexHidden( const QModelIndex &index ) const
{
  if( selectionModel()->selection().indexes().count() != 1 )
    return true;
    
  if( currentIndex() != index )
    return true;
    
  return false;
}

QModelIndex DemandScenarioView::indexAt( const QPoint &point ) const
{
  if( selectionModel()->selection().indexes().count() != 1 )
    return QModelIndex();
    
  return currentIndex();  
}

int DemandScenarioView::horizontalOffset() const
{
  return horizontalScrollBar()->value();
}

int DemandScenarioView::verticalOffset() const
{
  return verticalScrollBar()->value();
}

QModelIndex DemandScenarioView::moveCursor( CursorAction cursorAction, Qt::KeyboardModifiers modifiers )
{
  return currentIndex();
}

void DemandScenarioView::setSelection( const QRect &rect, QItemSelectionModel::SelectionFlags flags )
{
  // do nothing
}

void DemandScenarioView::scrollTo( const QModelIndex &index, ScrollHint hint )
{
  // cannot scroll
}

void DemandScenarioView::dataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight )
{
  updateText();
  updateView();
}

void DemandScenarioView::selectionChanged( const QItemSelection &selected, const QItemSelection &deselected )
{
  updateText();
}

void DemandScenarioView::updateText()
{
	QString lable = "";

  int i = 0;
  i = 1;
  QStringListModel modelDemand;
  QStringList strings;
//  lable = model()->data( currentIndex() ).toStringList();

  strings=model()->data( currentIndex() ).toStringList();//append(lable);
  modelDemand.setStringList( strings);
  for (i =0; i< strings.count(); i++)
  {
	  dp->listProcessDemandWidget->insertItem(i, strings[i]);

  }
//  dp->listProcessProfilesView->setSelectionModel(selectionModel());
//  dp->listProcessProfilesView->show();
/*
  switch( selectionModel()->selection().indexes())
  {
    case 0:
//      label->setText( tr("<i>No data.</i>") );
      break;
      
    case 1:
		i = currentIndex().row();
//      label->setText( model()->data( currentIndex() ).toString() );
      break;
      
    default:
//      label->setText( tr("<i>Too many items selected.<br>Can only show one item at a time.</i>") );
      break;
  }
  */
}

void DemandScenarioView::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
//  model->setData( index, static_cast<QLineEdit*>( dp->Ui->dsNameEdit)->text() );
}
/*
void DemandScenarioView::addSubWin() 
{
	dp = new DemandProfile;

	mdiArea->addSubWindow(dp);
	mdiArea->setViewMode(QMdiArea::TabbedView);
	m_DmdScnNo++;	

	LoadStandardDemandList();
  	QStringList headerList;
	headerList<<"Reference name"<<"Description";
	dp->stdProfilesTable->setRowCount(pRefNameVector.size());
	dp->stdProfilesTable->setColumnCount(2);

	dp->stdProfilesTable->setHorizontalHeaderLabels(headerList);
	dp->stdProfilesTable->show();

	for (int i= 0; i<pRefNameVector.size(); i++)
	{
		stdItem1 = new QTableWidgetItem(pRefNameVector[i]);	
		stdItem2 = new QTableWidgetItem(pDescriptionVector[i]);

        dp->stdProfilesTable->setItem(i, 0, stdItem1);
		stdItem1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        dp->stdProfilesTable->setItem(i, 1, stdItem2);
		stdItem2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	}
//	dp->stdProfilesTable->setEditable( false );
	dp->stdProfilesTable->setSelectionBehavior( QAbstractItemView::SelectRows );
	dp->show();
}
*/
void DemandScenarioView::updateView()
{

  QStringListModel modelDemand;
  QStringList strings;
  QTableWidgetItem *itemName = dp->stdProfilesTable->item( dp->stdProfilesTable->currentRow(),0);

  strings.append(itemName->text());
  modelDemand.setStringList( strings);
//  processDemandList.append(itemName);
//  SingleItemView *selectionView = new SingleItemView();

//  dp->listProcessProfilesView->setModel(&modelDemand);
 // dp->listProcessProfilesView->show();
}

//to detect if there is anything value changed in the field
/*
bool DemandScenarioView::isSafeToClose()
{
  if( document()->isModified() ) 
  {
    switch( QMessageBox::warning( this, tr("MDI"), 
      tr("The document has unsaved changes.\n"
         "Do you want to save it before it is closed?"), 
         QMessageBox::Discard | QMessageBox::Cancel ) )
    {
    case QMessageBox::Cancel:
      return false;
    default:
      return true;
    }
  }

  return true;
}

//when the user close the demand scenario tab
void DemandScenarioView::closeEvent( QCloseEvent *event )
{
  if( isSafeToClose() )
    event->accept();
  else
    event->ignore();
}
*/
void DemandScenarioView::LoadStandardDemandList()
{
	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list from the demand entity
	query.exec("SELECT ref, name FROM entity where data_type like \'D%\'");

    if (!query.isActive())
		QMessageBox::warning(this, tr("Database Error"),
                         query.lastError().text());


	//clear the data in the weather location list
	//stdProfiles->clear();
	//navigate through the fected query result set
    pRefNameVector.clear();
    pDescriptionVector.clear();
	while (query.next()) 
	{
		QString RefName = query.value(0).toString();
		QString DmdDescription = query.value(1).toString();
        pRefNameVector.push_back(RefName);
		pDescriptionVector.push_back(DmdDescription);
    }
}

/*
	stdProfile = new QTableWidget(this->stdProfilesTable);
    stdProfile->setHorizontalHeaderLabels(QStringList() << tr("Reference name") << tr("Description"));

	for (int i= 0; i<pRefNameVector.size(); i++)
	{
		stdItem1 = new QTableWidgetItem(pRefNameVector[i]);	
		stdItem2 = new QTableWidgetItem(pDescriptionVector[i]);

        stdProfile->setItem(i, 0, stdItem1);
        stdProfile->setItem(i, 1, stdItem2);
	}	

	stdProfile->show();
*/


