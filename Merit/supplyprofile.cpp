#include <QtGui>
#include <QMouseEvent>
#include <QStandardItemModel>
#include "supplyprofile.h"


class QStandardItemModel;
class QTableView;

using namespace std;

SupplyProfile::SupplyProfile(QWidget *parent)
    : QWidget(parent)
{
//    meritTab = new MeritTab;
    setupUi(this);
	pRefNameVector.clear();
	pDescriptionVector.clear();
	processSupplyList.clear();
	supplyData.clear();
	m_ItemName = "";
	/*
	supplySource->addItem("Supply database");
	supplySource->addItem("Import Supply");	
	supplySource->addItem("Supply generator");

	profileType->addItem("Electricity");
	profileType->addItem("Thermal");
    */
	//connect(dsNameEdit, SIGNAL(textChanged(QString)), this, SLOT(setText(QString)));
//	connect(dsNameEdit, SIGNAL(textChanged(QString)), this, SLOT(setdsName(QString)));
	//connect(dsDescriptionEdit, SIGNAL(textChanged(QString)), this, SLOT(setdsDescription(QString)));
	//connect(dsNameEdit, SIGNAL(textChanged(QString)), this, SLOT(setTitle(QString)));
	LoadStandardSupplyList();

//	connect(stdProfilesTable, SIGNAL(itemEntered (QTableWidgetItem *)), this, SLOT(setTableItemName(QTableWidgetItem *)));
//	connect(listProcessSupplyWidget, SIGNAL(itemEntered (QListWidgetItem *)), this, SLOT(setListItemName(QListWidgetItem *)));

	connect(stdProfilesTable, SIGNAL(itemClicked  (QTableWidgetItem *)), this, SLOT(setTableItemName(QTableWidgetItem *)));
//	connect(listProcessSupplyWidget, SIGNAL(itemClicked  (QListWidgetItem *)), this, SLOT(setListItemName(QListWidgetItem *)));

//	connect(addMiddleButton, SIGNAL(clicked()), this, SLOT(moveMiddle()) );
//	connect(addRightButton, SIGNAL(clicked()), this, SLOT(moveRight()) );

//	connect(removeMiddleButton, SIGNAL(clicked()), this, SLOT(removeMiddle()) );
//	connect(removeRightButton, SIGNAL(clicked()), this, SLOT(removeRight()) );



//	connect( right, SIGNAL(clicked()), this, SLOT(moveRight()) );
}



SupplyProfile::~SupplyProfile()
{

// delete stdProfileWidget;
// delete stdItem1;
// delete stdItem2;
// delete pRefNameVector;
// delete pDescriptionVector;
}


void SupplyProfile::updateView()
{

}



/*
//
void SupplyProfile::setText(const QString &text)
{
//check the text is actually being changed;
	if(m_text == text)
		return;
    
//actually emit the signal;
	m_text = text;
	emit textChanged(m_text);
}

void SupplyProfile::setTitle(const QString &title)
{
	//check the text is actually being changed;
	setWindowTitle(title);
    
//actually emit the signal;
	m_title = title;
	emit textChanged(m_title);
}
*/

/*
	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list from the supply entity
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

	QStringList headerList;
	headerList<<"Reference name"<<"Description";
	stdProfile=new QTableWidget(stdProfilesTable);
	stdProfile->setRowCount(pRefNameVector.size());
	stdProfile->setColumnCount(2);
	stdProfile->setHorizontalHeaderLabels(headerList);
	stdProfile->show();
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
//}

/*
void SupplyProfile::retrieveSupplyData(const QString& site)
{
	int i=0;
	d->AllocateMemory(8760);

	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list for the weather location
	QString strSQL, strSite;
	strSite = "'"  + site + "'";
	strSQL = "SELECT ambient, dir_solar, diff_solar, wind_speed, wind_dir, rh FROM climate_espr WHERE climate_site = ";
	strSQL = strSQL + strSite;

//	strSQL.Format(_T("SELECT ambient, dir_solar, diff_solar, wind_speed, wind_dir, rh FROM climate_espr WHERE climate_site = '%s'"),site);
	query.exec(strSQL);
    if (!query.isActive())
		QMessageBox::warning(this, tr("Database Error"),
                         query.lastError().text());

	//clear the data in the weather location list

	//navigate through the fected query result set

	while (query.next()) 
	{
		setDbTemperature(i, query.value(0).toFloat());
		setDirectSolar(i, query.value(1).toFloat());
		setDiffuseSolar(i, query.value(2).toFloat());
		setWindSpeed(i, query.value(3).toFloat());
		setWindDirection(i, query.value(4).toFloat());
		setRelativeHumidity(i, query.value(5).toFloat());
		i++;
    }
}
*/
/*
void SupplyProfile::moveMiddle()
{
  QStringListModel modelSupply;
  QStringList strings;
  QTableWidgetItem *itemName = stdProfilesTable->item( stdProfilesTable->currentRow(),0);
  QPair<QString, QVector <float> > tempData;
  tempData.first = "";
  tempData.second.clear();
  tempData.second.reserve(8760);
  tempData.second.resize(8760);

  retrieveSupplyData(itemName->text());
//to check if the item to be added is contained within the list process supply widget
  if (listProcessSupplyWidget->findItems(itemName->text(), Qt::MatchExactly).count()!= 0)
  { 
    bool ok;
	//introduce a new InputDialog to ask user to input the name for the new item
	QString text = QInputDialog::getText(this,tr("Name already exist"),tr("Please enter a new name:"),QLineEdit::Normal,tr(" "),&ok);
	//if user press OK and text field is not empty, take the new name in the line edit field and insert the item to the end of the list
	if( ok && !text.isEmpty() )
	{
	  strings.append(itemName->text());
//	  retrieve data from the database
//	  tempData = 
	
	  processSupplyList.append(qMakePair(text, supplyData));
	  
	  modelSupply.setStringList(strings);
	  for (int i =0; i< strings.count(); i++)
		  listProcessSupplyWidget-> insertItem(i, text);
	}
  }
//if it is not in the list, append it to the end of the list
  else
  {
	  strings.append(itemName->text());
	  processSupplyList.append(qMakePair(itemName->text(), supplyData));
	  modelSupply.setStringList( strings);
	  for (int i =0; i< strings.count(); i++)
		  listProcessSupplyWidget-> insertItem(i, strings[i]);
  }

}


void SupplyProfile::moveRight()
{
  QStringListModel selectedSupply;
  QStringList strings;
 
  QPair<QString, QVector <float> > tempData;
  tempData.first = "";
  tempData.second.clear();
  tempData.second.reserve(8760);
  tempData.second.resize(8760);

 // connect(stdProfilesTable, SIGNAL(itemEntered (QTableWidgetItem *)), this, SLOT(setTableItemName(QTableWidgetItem *)));

//  void QTableWidget::itemEntered ( QTableWidgetItem * item )

//  QListWidgetItem *itemName = listProcessSupplyWidget->item(listProcessSupplyWidget->currentRow());

//  strings.append(itemName->text());
//  selectedSupply.setStringList( strings);

//  for (int i =0; i< strings.count(); i++)
//	  listSelectedSupplyWidget-> insertItem(i, strings[i]);

//to check if the item to be added is contained within the list process supply widget
//  if (listSelectedSupplyWidget->findItems(itemName->text(), Qt::MatchExactly).count()!= 0)
  if (listSelectedSupplyWidget->findItems(m_ItemName, Qt::MatchExactly).count()!= 0)
  { 
    bool ok;
	//introduce a new InputDialog to ask user to input the name for the new item
	QString text = QInputDialog::getText(this,tr("Name already exist"),tr("Please enter a new name:"),QLineEdit::Normal,tr(" "),&ok);
	//if user press OK and text field is not empty, take the new name in the line edit field and insert the item to the end of the list
	if( ok && !text.isEmpty() )
	{
		//check if the item in the selected supply list is from the standard table widget 
		// or the item is from the middle list widget as the same item inside the standard table widget 
		  if (stdProfilesTable->findItems(m_ItemName, Qt::MatchExactly).count()!= 0)
		  {
			  retrieveSupplyData(m_ItemName);
			  selectedSupplyList.append(qMakePair(text, supplyData));
		  }
		//if the item is from the middle list widget and not in the standard table widget
		  else
		  {
			  for (int i = 0; i < processSupplyList.count(); i++)
			  {
				  if (m_ItemName == processSupplyList[i].first)
				  {
					  selectedSupplyList.append(qMakePair(text, processSupplyList[i].second));
				  }
			  }
		  }
	//	  strings.append(itemName->text());
		  strings.append(m_ItemName);
		  selectedSupply.setStringList(strings);
		  for (int i =0; i< strings.count(); i++)
			  listSelectedSupplyWidget-> insertItem(i, text);
	}
  }
//if it is not in the list, append it to the end of the list
  else
  {
//	  strings.append(itemName->text());
		//check if the item in the selected supply list is from the standard table widget 
		// or the item is from the middle list widget as the same item inside the standard table widget 
		  if (stdProfilesTable->findItems(m_ItemName, Qt::MatchExactly).count()!= 0)
		  {
			  retrieveSupplyData(m_ItemName);
			  selectedSupplyList.append(qMakePair(m_ItemName, supplyData));
		  }
		//if the item is from the middle list widget and not in the standard table widget
		  else
		  {
			  for (int i = 0; i < processSupplyList.count(); i++)
			  {
				  if (m_ItemName == processSupplyList[i].first)
				  {
					  selectedSupplyList.append(processSupplyList[i]);
				  }
			  }
		  }
		  strings.append(m_ItemName);
		  selectedSupply.setStringList(strings);
		  for (int i =0; i< strings.count(); i++)
			  listSelectedSupplyWidget-> insertItem(i, strings[i]);
  }
}


void SupplyProfile::removeMiddle()
{
//	listProcessSupplyWidget->takeItem(listProcessSupplyWidget->currentRow());
	switch( QMessageBox::question(this, tr("Remove item"), tr("Are you sure to delete the item?"),
		    tr("&Yes"), tr("&No"), QString(), 0, 1))
	{
		case 0:
				listProcessSupplyWidget->takeItem(listProcessSupplyWidget->currentRow());
		break;

		case 1:
			return;
		break;

		default:
			return;
		break;
	}
}

void SupplyProfile::removeRight()
{
//  listSelectedSupplyWidget->takeItem(listSelectedSupplyWidget->currentRow());
	switch( QMessageBox::question(this, tr("Remove item"), tr("Are you sure to delete the item?"),
		    tr("&Yes"), tr("&No"), QString(), 0, 1))
	{
		case 0:
				listSelectedSupplyWidget->takeItem(listSelectedSupplyWidget->currentRow());
		break;

		case 1:
			return;
		break;

		default:
			return;
		break;
	}
 // emit datachanged;
}
*/
void SupplyProfile::LoadStandardSupplyList()
{
	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list from the supply entity
	query.exec("SELECT ref, name FROM entity where data_type like \'S%\'");

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

  	QStringList headerList;
	headerList<<"Reference name"<<"Description";
	stdProfilesTable->setRowCount(pRefNameVector.size());
//	stdProfilesTable->setColumnCount(2);
	stdProfilesTable->setColumnCount(1);

	stdProfilesTable->setHorizontalHeaderLabels(headerList);
	stdProfilesTable->show();

	for (int i= 0; i<pRefNameVector.size(); i++)
	{
		stdItem1 = new QTableWidgetItem(pRefNameVector[i]);	
		stdItem2 = new QTableWidgetItem(pDescriptionVector[i]);

        stdProfilesTable->setItem(i, 0, stdItem1);
		stdItem1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        stdProfilesTable->setItem(i, 1, stdItem2);
		stdItem2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	}
//	dp->stdProfilesTable->setEditable( false );
	stdProfilesTable->setSelectionBehavior( QAbstractItemView::SelectRows );
//	dp->show();
}


void SupplyProfile::setTableItemName(QTableWidgetItem *Item)
{
	m_ItemName = Item->text();
}


void SupplyProfile::retrieveSupplyData(const QString& name)
{
	int i=0;
	AllocateMemory(8760);

	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list for the weather location
	QString strSQL, strName;
	strName = "'"  + name + "'";
	strSQL = "SELECT tot FROM energy WHERE ref = ";
	strSQL = strSQL + strName;

//	strSQL.Format(_T("SELECT ambient, dir_solar, diff_solar, wind_speed, wind_dir, rh FROM climate_espr WHERE climate_site = '%s'"),site);
	query.exec(strSQL);
    if (!query.isActive())
		QMessageBox::warning(this, tr("Database Error"),
                         query.lastError().text());

	//clear the data in the weather location list

	//navigate through the fected query result set
	int temp = 0;

	while (query.next()) 
	{
		setSupplyData(i, query.value(0).toFloat());
		i++;
    }
}

float SupplyProfile::getSupplyData(int index)
{
	return supplyData[index];
}

void SupplyProfile::setSupplyData(int index, float supply)
{
	supplyData[index] = supply;
}

void SupplyProfile::AllocateMemory(int n)
{
	//allocate 8760 to the vectors for storing supply data
	supplyData.clear();
    supplyData.reserve(n);
    supplyData.resize(n);
}
void SupplyProfile::itemClicked(QTableWidgetItem* item )
{
	QMenu* contextMenu = new QMenu(this);
	Q_CHECK_PTR (contextMenu);

	contextMenu->addAction("Graph", this->stdProfilesTable, SLOT(drawSupplyGraph()));
	contextMenu->addAction("Info", this->stdProfilesTable, SLOT(supplyInfo(item)));
	contextMenu->addSeparator();
	contextMenu->addAction("Select final", this->stdProfilesTable, SLOT(MoveRigth()));
	contextMenu->addAction("Select manipulation", this->stdProfilesTable, SLOT(MoveMiddle()));

	contextMenu->popup(QCursor::pos());
	contextMenu->exec();
	delete contextMenu;
	contextMenu = 0;
}
/*
void SupplyProfile::LocateItem(const QPoint &pos)
{

	if(stdProfilesTable->isItemSelected(stdProfilesTable->itemAt(pos)))
	{
		QMenu* contextMenu = new QMenu(this);
		Q_CHECK_PTR (contextMenu);

		contextMenu->addAction("Graph", this->stdProfilesTable, SLOT(drawDemandGraph()));
		contextMenu->addAction("Info", this->stdProfilesTable, SLOT(demandInfo(stdProfilesTable->itemAt(pos))));
		contextMenu->addAction("Select final", this->stdProfilesTable, SLOT(MoveRigth()));
		contextMenu->addAction("Select manipulation", this->stdProfilesTable, SLOT(MoveMiddle()));

		contextMenu->popup(QCursor::pos());
		contextMenu->exec();
		delete contextMenu;
		contextMenu = 0;
	}

}
*/
/*
void SupplyProfile::mousePressEvent( QMouseEvent * event )
{
	  int mx, my;
	  mx = event->x();
	  my = event->y();

//	emit gotEvent( tr("mousePressEvent( x:%1, y:%2, button:%3 )")
    .arg( event->x() )
    .arg( event->y() )
    .arg( event->button()==Qt::LeftButton? "LeftButton":
          event->button()==Qt::RightButton?"RightButton":
          event->button()==Qt::MidButton?  "MidButton":
          event->button()==Qt::XButton1?   "XButton1":
                                           "XButton2" ) );

}
*/

//void SupplyProfile::supplyInfo(QTableWidgetItem* item)
//{

//}