#include <QtGui>
#include "supply.h"
#include <QtCore/QFile>
#include "supplyscenariotreemodel.h"
#include "supplytreeitem.h"

class SupplyScenarioTreeModel;
class SupplyTreeItem;

using namespace std;

Supply::Supply(QWidget *parent)
    : QWidget(parent)//, QAbstractItemView( parent )
{
//initial number of supply scenario
	m_SupScnNo = 0;
	processSupplyList.clear();
	m_iStartDay = 0;
	m_iEndDay = 0;
	m_iHourSteps = 0;
	int itemNumber = 0;

	gridLayout = new QGridLayout(this);
	verticalLayout = new QVBoxLayout();

	horizontalLayout1 = new QHBoxLayout();
	horizontalLayout2 = new QHBoxLayout();

	splitterHrt = new QSplitter(Qt::Horizontal);


	widget = new QWidget();
    widget->setObjectName(QString::fromUtf8("widget"));
	
	verticalLayout1 = new QVBoxLayout(widget);

	tree = new QTreeView();
	
	verticalLayout1->addWidget(tree);	


	addButton = new QPushButton("Add");
	addButton->setObjectName(QString::fromUtf8("addDS"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(addButton->sizePolicy().hasHeightForWidth());
    addButton->setSizePolicy(sizePolicy);

	delButton = new QPushButton("Delete");
	delButton->setObjectName(QString::fromUtf8("delDS"));
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(delButton->sizePolicy().hasHeightForWidth());
    delButton->setSizePolicy(sizePolicy);

    horizontalSpacer_1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	horizontalLayout1->addItem(horizontalSpacer_1);
	horizontalLayout1->addWidget(addButton);
	horizontalLayout1->addItem(horizontalSpacer_2);
	horizontalLayout1->addWidget(delButton);
	horizontalLayout1->addItem(horizontalSpacer_3);


	verticalLayout1->addLayout(horizontalLayout1);

//load the customised supply widget
	ssView = new SupplyProfile;
	ssView->setObjectName(QString::fromUtf8("ssView"));

	itemNumber = ssView->stdProfilesTable->rowCount();
	initialiseTemporalList(itemNumber);

//	qwtPlot = new QwtPlot();
	supplyPlot = new PlotSupply();
	QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy2.setHorizontalStretch(0);
	sizePolicy2.setVerticalStretch(0);
	sizePolicy2.setHeightForWidth(supplyPlot->sizePolicy().hasHeightForWidth());
	supplyPlot->setSizePolicy(sizePolicy2);


	splitterVrt = new QSplitter(Qt::Vertical);

	splitterHrt->addWidget(widget);
	splitterHrt->addWidget(ssView);

	splitterHrt->setStretchFactor(0,10);
	splitterHrt->setStretchFactor(1,13);

	splitterVrt->addWidget(splitterHrt);
	splitterVrt->addWidget(supplyPlot);

	splitterVrt->setStretchFactor(0,5);
	splitterVrt->setStretchFactor(1,2);


	gridLayout->addWidget(splitterVrt);
//	 QStringList data;
	 m_SupplyScenarioData.clear();
	 m_SupplyScenarioData << "No scenario has been specified yet.";
     model = new SupplyScenarioTreeModel(m_SupplyScenarioData);

	tree->setModel(model);
	tree->show();
    m_SupplyScenarioData.clear();

//double click an item in the table widget to draw the graph
	connect(ssView->stdProfilesTable, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(drawSupplyGraph(QTableWidgetItem *)));

//double click an item in the middle list widget to draw the graph
//	connect(ssView->listProcessSupplyWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(drawSupplyGraph(QListWidgetItem *)));

//single click an item in the right list widget to draw the supply graph
//	connect(ssView->listSelectedSupplyWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(drawRightSupplyGraph()));//QListWidgetItem *)));

//select multiple items in the right list widget to draw aggregate supplys
//    connect(ssView->listSelectedSupplyWidget, SIGNAL(itemSelectionChanged()), this, SLOT(drawMultipleSupplys()));

//	connect(ssView->saveScenarioButton, SIGNAL(clicked()), this, SLOT(saveCurrentScenario()));
//	connect(ssView->saveasScenarioButton, SIGNAL(clicked()), this, SLOT(saveCurrentScenarioAs()));

//	connect(tree, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(ItemName()));
//	connect(tree, SIGNAL(clicked(const QModelIndex &)), this, SLOT(populateProfiles()));
//	connect(addButton, SIGNAL(clicked()), this, SLOT(addSupplyScenario()));
	connect(tree, SIGNAL(clicked(const QModelIndex &)), this, SLOT(deployProfiles()));

	connect(addButton, SIGNAL(clicked()), this, SLOT(addSupplyScenario()));
	connect(delButton, SIGNAL(clicked()), this, SLOT(deleteCurrentScenario()));
	connect(ssView->stdProfilesTable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(LocateItem(const QPoint &)));
}

Supply::~Supply()
{

}

void Supply::initialiseTemporalList(int n)
{
	//allocate 8760 to the vectors for storing supply data
	supplyStartDayList.clear();
	supplyEndDayList.clear();
	supplyHourStepsList.clear();

    for (int i=0; i<n; i++)
	{
		supplyStartDayList.append(qMakePair(ssView->stdProfilesTable->item(i,0)->text(), 0));
		supplyEndDayList.append(qMakePair(ssView->stdProfilesTable->item(i,0)->text(), 0));
		supplyHourStepsList.append(qMakePair(ssView->stdProfilesTable->item(i,0)->text(), 0));
	}
}


void Supply::addSupplyScenario()
{
	if (m_SupplyScenarioData.count()!=0)
	{
		m_SupScnNo = supplyScenarioPortfolio.count()+1;
//		m_SupplyScenarioData.append(QString("ds %0").arg(m_SupScnNo));;
//		m_SupplyScenarioData.append("   no profile has been selected yet.");
//clear the list of supply prfoile;
		supplyProfileList.clear();

//initialise the data required for a specific supply profile
		SupplyData supplyProfileData; //represent one specific supply profile
//push the supply profile to a list
		supplyProfileList.append(supplyProfileData); //
//make pair with the supply scenario name and push back to a supply scenario list
		supplyScenarioPortfolio.append(qMakePair(QString("ss %0").arg(m_SupScnNo), supplyProfileList));
//put the reference id for the supply scenario into a list
		supplyScenarioName.append(QString("ss %0").arg(m_SupScnNo));
	}
	else
	{
		m_SupplyScenarioData << QString("ss %0").arg(0) << "   no profile has been selected yet.";
		m_SupScnNo++;
//establish the new interface for the supply profile
//load the customised supply widget
		ssProcess = new SupplyProcess;
		ssProcess->setObjectName(QString::fromUtf8("ssProcess"));

		ssSelected = new SupplySelection;
		ssSelected->setObjectName(QString::fromUtf8("ssSelected"));

		splitterHrt->addWidget(ssProcess);
		splitterHrt->addWidget(ssSelected);

		connect(ssProcess->accelerateButton, SIGNAL(clicked()), this, SLOT(moveMiddle()));
		connect(ssSelected->accelerateButton, SIGNAL(clicked()), this, SLOT(moveRight()));
		connect(ssProcess->listProcessSupplyWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(locateProcessListItem(const QPoint &)));
		connect(ssSelected->listSelectedSupplyWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(locateSelectedListItem(const QPoint &)));

		connect(ssView->stdProfilesTable, SIGNAL(itemClicked  (QTableWidgetItem *)), this, SLOT(setTableItemName(QTableWidgetItem *)));
		connect(ssProcess->listProcessSupplyWidget, SIGNAL(itemClicked  (QListWidgetItem *)), this, SLOT(setListItemName(QListWidgetItem *)));
		connect(ssProcess->scaleButton, SIGNAL(clicked()), this, SLOT(scaleSupplyProfile()));
		connect(ssProcess->shiftButton, SIGNAL(clicked()), this, SLOT(shiftSupplyProfile()));
		connect(ssProcess->combineButton, SIGNAL(clicked()), this, SLOT(combineSupplyProfile()));
		connect(ssProcess->clearButton, SIGNAL(clicked()), this, SLOT(clearSupplyProfile()));

		connect(ssSelected->saveButton, SIGNAL(clicked()), this, SLOT(saveCurrentScenario()));
		connect(ssSelected->saveasButton, SIGNAL(clicked()), this, SLOT(saveCurrentScenarioAs()));

		initialiseProcessSupplyTemporalList();
		initialiseSelectedSupplyTemporalList();

//clear the list of supply prfoile;
		supplyProfileList.clear();
//clear the list for the supply scenario name
		supplyScenarioName.clear();
//initialise the data required for a specific supply profile
		SupplyData supplyProfileData;
//push the supply profile to a list
		supplyProfileList.append(supplyProfileData);
//make pair with the supply scenario name and push back to a supply scenario list
		supplyScenarioPortfolio.append(qMakePair(QString("ss %0").arg(m_SupScnNo), supplyProfileList));
//put the reference id for the supply scenario into a list
		supplyScenarioName.append(QString("ss %0").arg(m_SupScnNo));
	}

	//prepare the list for the model
	m_SupplyScenarioData.clear();
	QString tempString = "";
	for (int k= 0; k < supplyScenarioPortfolio.count(); k++)
	{
		m_SupplyScenarioData.append(supplyScenarioPortfolio[k].first);
		for (int m = 0; m < supplyScenarioPortfolio[k].second.count(); m++)
		{
		   tempString = (supplyScenarioPortfolio[k].second[m].supplyName());
		   if (tempString == "")
			   tempString = "no profile has been selected yet.";
		   tempString.prepend(" ");
		   m_SupplyScenarioData.append(tempString);
		}
		mapSS[supplyScenarioPortfolio[k].first] = supplyScenarioName[k]; //make a map for later when user change the name of supply scenario
	}
//emit a signal for supply scenario for the match to invoke the slot
	emit valueChanged(mapSS);
	emit valueChangedSSP(supplyScenarioPortfolio);
    model = new SupplyScenarioTreeModel(m_SupplyScenarioData);

//set the view settings for TreeView
	tree->setSelectionMode(QAbstractItemView::SingleSelection);
	tree->header()->setClickable(true);

//set up the model to the tree view
	tree->setModel(model);
//expand the tree after the model has been set up
	for (int k=0; k<m_SupplyScenarioData.count(); k++)
		tree->setExpanded(model->index(k,0), true);
//show the tree view
	tree->show();
}

void Supply::initialiseProcessSupplyTemporalList()
{
	prcSupplyStartDayList.clear();
	prcSupplyEndDayList.clear();
	prcSupplyHourStepsList.clear();
}

void Supply::initialiseSelectedSupplyTemporalList()
{
	selSupplyStartDayList.clear();
	selSupplyEndDayList.clear();
	selSupplyHourStepsList.clear();
}
//draw the graph of the selected supply in standard profile table 
void Supply::drawSupplyGraph()
{
	QTableWidgetItem * itemTemp;
	itemTemp = getItemSelected();

	supplyPlot->detachItems(QwtPlotItem::Rtti_PlotCurve, true);

	retrieveSupplyData(itemTemp->text());
 	supplyPlot->drawSupplyGraph(itemTemp->text(),supplyDataVector);

}

void Supply::drawSingleMiddleSupplyGraph()
{
	QListWidgetItem * itemTemp;
	itemTemp = getListItemSelected();
//clear the previous plotted items
	supplyPlot->detachItems(QwtPlotItem::Rtti_PlotCurve, true);

	for (int i=0; i<processSupplyList.count(); i++)
	{
		if(itemTemp->text() == processSupplyList[i].first)
		{
	 		supplyPlot->drawSupplyGraph(itemTemp->text(), processSupplyList[i].second);
		}
	}
}

/*
void Supply::drawRightSupplyGraph()//QListWidgetItem * items)
{
//	retrieveWeatherData(d->m_sWeatherSite);
//	QList <float> supplyData;
	QList<QListWidgetItem *> ItemSelected;

	QVector <QVector <float> > SupplySelectedVector;

	ItemSelected = ssView->listSelectedSupplyWidget->selectedItems();
//Allocate memory for selected supply vector (2 dimensions)	
	SupplySelectedVector.clear();
	SupplySelectedVector.reserve(ItemSelected.count()+1);
	SupplySelectedVector.resize(ItemSelected.count()+1);

	ItemNameVector.clear();
	ItemNameVector.reserve(ItemSelected.count()+1);
	ItemNameVector.resize(ItemSelected.count()+1);

	for (int j=0; j < SupplySelectedVector.size(); j++)
	{
		SupplySelectedVector[j].clear();
		SupplySelectedVector[j].reserve(8760);
		SupplySelectedVector[j].resize(8760);
	}
	int count = 0;
	count = ssView->selectedSupplyList.count();

	for (int i = 0; i< ItemSelected.count()+1; i++)
	{
		if(i < ItemSelected.count())
		{
			ItemNameVector[i] = ItemSelected[i]->text();
//check if the name selected is matched with which item in the selectedsupplylist
			for (int k=0; k<count; k++)
			{
				if (ItemSelected[i]->text() == ssView->selectedSupplyList[k].first)
	 				SupplySelectedVector[i] = ssView->selectedSupplyList[k].second;
			}
//
//			retrieveSupplyData(ItemSelected[i]->text());
//			SupplySelectedVector[i] = supplyDataVector;
			for (int j(SupplySelectedVector[i].size() - 1); j >= 0; --j)
				SupplySelectedVector[ItemSelected.count()][j] += SupplySelectedVector[i][j];
		}
		else
		{
			ItemNameVector[i] = "Total";
		}
	}
//	retrieveSupplyData(item->text());
//	if (ItemNameVector.size() == 1)
	supplyPlot->drawMultipleSupplysGraph(ItemNameVector,SupplySelectedVector);


//	else if (ItemNameVector.size() == 2)
//		supplyPlot2->drawMultipleSupplysGraph(ItemNameVector,SupplySelectedVector);
//	supplyPlot->replot();
}
/*
void Supply::drawMultipleSupplys(QListWidgetItem * items)
{
//	retrieveWeatherData(d->m_sWeatherSite);
//	QList <float> supplyData;
	retrieveSupplyData(item->text());
 	supplyPlot->drawSupplyGraph(item->text(),supplyDataVector);

}
*/
//QList<QListWidgetItem *> QListWidget::selectedItems () const
//Returns a list of all selected items in the list widget.


void Supply::retrieveSupplyData(const QString& name)
{
	int i=0;
	AllocateMemory(8760);

	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list for the weather location
	QString strSQL, strName;
	strName = "'"  + name + "'";
	strSQL = "SELECT tot, to_datetime FROM energy WHERE ref = ";
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
		setSupplyDateData(i, query.value(1).toString());
		i++;
    }
	setTemporalParameterList();
}
float Supply::getSupplyData(int index)
{
	return supplyDataVector[index];
}

void Supply::setSupplyData(int index, float supply)
{
	supplyDataVector[index] = supply;
}

QString Supply::getSupplyDateData(int index)
{
	return supplyDateDataVector[index];
}

void Supply::setSupplyDateData(int index, QString supply_date)
{
	supplyDateDataVector[index] = supply_date;
}

void Supply::AllocateMemory(int n)
{
	//allocate 8760 to the vectors for storing supply data
	supplyDataVector.clear();
    supplyDataVector.reserve(n);
    supplyDataVector.resize(n);

	supplyTotalVector.clear();
    supplyTotalVector.reserve(n);
    supplyTotalVector.resize(n);

	supplyDateDataVector.clear();
    supplyDateDataVector.reserve(n);
    supplyDateDataVector.resize(n);
}

void Supply::saveCurrentScenario()
{
	QList <QString> listSupplys;
	QStringList temp1;
	QStringList temp2;
	QPair<QString, QVector<float> > supplyProfileData;
	listSupplys.clear();
	int rowCount = 0;
	QString supplys = "";
//rowCount: the number of profiles in the selected list widget
	rowCount = ssSelected->listSelectedSupplyWidget->count();


	if (tree->selectionModel()->hasSelection())
	{
		if (model->hasChildren(tree->currentIndex()))
		{
			supplys = model->data(tree->currentIndex(), Qt::DisplayRole).toString();
		}
		else
		{
			supplys = model->data(model->parent(tree->currentIndex()), Qt::DisplayRole).toString();
		}

		//navigate the elements within the map of supply scenario using iternator
		QString tempSS = "";
		QMap<QString, QString>::const_iterator i = mapSS.constBegin();
		while (i != mapSS.constEnd()) 
		{
			if (i.value() == supplys)
			{
				tempSS = i.key();
				break;
			}
			++i;
		}
		//set up the supply scenario using the data in the selected list
		for (int i=0; i < supplyScenarioPortfolio.count(); i++)
		{
			if (tempSS == supplyScenarioPortfolio[i].first)
			{
				//clear the list of supply prfoile;
				supplyProfileList.clear();
				//initialise the data required for a specific supply profile
				SupplyData supplyProfileData; //represent one specific supply profile

				for (int j = 0; j < selectedSupplyList.count(); j++)
				{
					supplyProfileData.setSupplyName(selectedSupplyList[j].first);
					supplyProfileData.setStartDay(selSupplyStartDayList[j].second);
					supplyProfileData.setEndDay(selSupplyStartDayList[j].second);
					supplyProfileData.setHourSteps(selSupplyHourStepsList[j].second);
					supplyProfileData.m_pSupplyDataVector = selectedSupplyList[j].second;

					//push the supply profile to a list
					supplyProfileList.append(supplyProfileData);
				}
				//copy the supply profile list to the supply scenario 
				supplyScenarioPortfolio[i].second = supplyProfileList;

				//terminate the loop
				break;
			}			
		}
	}
	else
	{
		if (m_SupScnNo == 1)
		{
			//clear the list of supply prfoile;
			supplyProfileList.clear();
			//initialise the data required for a specific supply profile
			SupplyData supplyProfileData; //represent one specific supply profile

			for (int j = 0; j < selectedSupplyList.count(); j++)
			{
				supplyProfileData.setSupplyName(selectedSupplyList[j].first);
				supplyProfileData.setStartDay(selSupplyStartDayList[j].second);
				supplyProfileData.setEndDay(selSupplyStartDayList[j].second);
				supplyProfileData.setHourSteps(selSupplyHourStepsList[j].second);
				supplyProfileData.m_pSupplyDataVector = selectedSupplyList[j].second;

				//push the supply profile to a list
				supplyProfileList.append(supplyProfileData);
			}
			//copy the supply profile list to the supply scenario 
			supplyScenarioPortfolio[0].second = supplyProfileList;
		}
	}
	
//prpeare the list for the updated profiles 	
	m_SupplyScenarioData.clear();
	QString tempString = "";
	for (int k= 0; k < supplyScenarioPortfolio.count(); k++)
	{
		m_SupplyScenarioData.append(supplyScenarioPortfolio[k].first);
		for (int m = 0; m < supplyScenarioPortfolio[k].second.count(); m++)
		{
		   tempString = (supplyScenarioPortfolio[k].second[m].supplyName());
		   if (tempString == "")
			   tempString = "no profile has been selected yet.";
		   tempString.prepend(" ");
		   m_SupplyScenarioData.append(tempString);
		}
		mapSS[supplyScenarioPortfolio[k].first] = supplyScenarioName[k]; //make a map for later when user change the name of supply scenario
	}

//emit a signal for supply scenario for the match to invoke the slot
//	emit valueChanged(m_DmdScnNo);
//	emit valueChanged(mapDS);
	emit valueChangedSSP(supplyScenarioPortfolio);

    model = new SupplyScenarioTreeModel(m_SupplyScenarioData);
	tree->setSelectionMode(QAbstractItemView::SingleSelection);
	tree->header()->setClickable(true);
	tree->setModel(model);

	for (int k=0; k<m_SupplyScenarioData.count(); k++)
		tree->setExpanded(model->index(k,0), true);

	tree->show();
}

void Supply::saveCurrentScenarioAs()
{
//	QList <QString> listSupplys;
//	QPair<QString, QVector<float> > supplyProfileData;

//	listSupplys.clear();
//	int rowCount = 0;
	QString supplys = "";
//	rowCount = dsSelected->listSelectedSupplyWidget->count();

/////////////////////////////////////
	m_SupScnNo++;
	//clear the list of supply prfoile;
	supplyProfileList.clear();
	//initialise the data required for a specific supply profile
	SupplyData supplyProfileData; //represent one specific supply profile

	for (int j = 0; j < selectedSupplyList.count(); j++)
	{
		supplyProfileData.setSupplyName(selectedSupplyList[j].first);
		supplyProfileData.setStartDay(selSupplyStartDayList[j].second);
		supplyProfileData.setEndDay(selSupplyStartDayList[j].second);
		supplyProfileData.setHourSteps(selSupplyHourStepsList[j].second);
		supplyProfileData.m_pSupplyDataVector = selectedSupplyList[j].second;

		//push the supply profile to a list
		supplyProfileList.append(supplyProfileData);
	}
//make pair with the supply scenario name and push back to a supply scenario list
	supplyScenarioPortfolio.append(qMakePair(QString("ss %0").arg(m_SupScnNo), supplyProfileList));
//put the reference id for the supply scenario into a list
	supplyScenarioName.append(QString("ss %0").arg(m_SupScnNo));
///////////////////////////////////
//prpeare the list for the updated profiles 	
	m_SupplyScenarioData.clear();
	QString tempString = "";
	for (int k= 0; k < supplyScenarioPortfolio.count(); k++)
	{
		m_SupplyScenarioData.append(supplyScenarioPortfolio[k].first);
		for (int m = 0; m < supplyScenarioPortfolio[k].second.count(); m++)
		{
		   tempString = (supplyScenarioPortfolio[k].second[m].supplyName());
		   if (tempString == "")
			   tempString = "no profile has been selected yet.";
		   tempString.prepend(" ");
		   m_SupplyScenarioData.append(tempString);
		}
		mapSS[supplyScenarioPortfolio[k].first] = supplyScenarioName[k]; //make a map for later when user change the name of supply scenario
	}
//////////////////////////////////
//emit a signal for supply scenario for the match to invoke the slot
	emit valueChanged(mapSS);
	emit valueChangedSSP(supplyScenarioPortfolio);


    model = new SupplyScenarioTreeModel(m_SupplyScenarioData);
	tree->setSelectionMode(QAbstractItemView::SingleSelection);
	tree->header()->setClickable(true);
	tree->setModel(model);
	for (int k=0; k<m_SupplyScenarioData.count(); k++)
		tree->setExpanded(model->index(k,0), true);

//	tree->setEditable(model->index(0,0));
	tree->show();
}

/*
QModelIndex Supply::ItemName()
{
	QStringList strings;

	m_ItemName = tree->currentIndex();
	//check the current item contains children
	//if yes, set the role editable

	//otherwise, set the item read-only
	if (model->hasChildren(m_ItemName))
		strings = model->data(m_ItemName, Qt::DisplayRole).toStringList();

	return m_ItemName;
}
*/
/*
void Supply::populateProfiles()
{
	QStringList temp;
	QString supplys = "";

	int position = 0;
	int next_position = 0;

	if (tree->selectionModel()->hasSelection())
	{
//if the current index is the parent item
		if (model->hasChildren(tree->currentIndex()))
		{
			supplys = model->data(tree->currentIndex(), Qt::DisplayRole).toString();

			for (int j=0; j <m_SupplyScenarioData.count(); j++)
			{
				if (supplys == m_SupplyScenarioData[j])
				{
					position = j;
					if ((position+1) <= m_SupplyScenarioData.count())
					{
						for (int k = position+1; k < m_SupplyScenarioData.count();k++)
						{
							if(m_SupplyScenarioData[k].startsWith(" "))
								next_position = k+1;
							else
								break;
						}
					}
					temp = m_SupplyScenarioData.mid(position, next_position);
					break;
				}
			}

		}
	}

	//add the list the RightListWidget for the selected supplys
	ssView->listSelectedSupplyWidget->clear();

	for (int i=1; i < temp.size(); i++)
	{
		if (!temp[i].contains("profile has"))
		{
			temp[i].remove(0,1);
			ssView->listSelectedSupplyWidget-> insertItem(i, temp[i]);


		}
	}

}
*/

void Supply::LocateItem(const QPoint &pos)
{

	if(ssView->stdProfilesTable->isItemSelected(ssView->stdProfilesTable->itemAt(pos)))
	{
		setItemSelected(ssView->stdProfilesTable->itemAt(pos));
		QMenu* contextMenu = new QMenu(this);
		Q_CHECK_PTR (contextMenu);
		if (m_SupScnNo == 0)
		{
			contextMenu->addAction("Graph", this, SLOT(drawSupplyGraph()));
			contextMenu->addAction("Info", this, SLOT(showSupplyInfo()));
			contextMenu->addAction("Properties", this, SLOT(supplyProperty())); //include the start time, end time and number of timesteps between
		}
		else
		{
			contextMenu->addAction("Graph", this, SLOT(drawSupplyGraph()));
			contextMenu->addAction("Info", this, SLOT(supplyInfo(stdProfilesTable->itemAt(pos))));
			contextMenu->addAction("Properties", this, SLOT(supplyProperty())); //include the start time, end time and number of timesteps between
			contextMenu->addSeparator();
			contextMenu->addAction("Select final", this, SLOT(moveRight()));
			contextMenu->addAction("Select manipulation", this, SLOT(moveMiddle()));
		}

		contextMenu->popup(QCursor::pos());
		contextMenu->exec();
		delete contextMenu;
		contextMenu = 0;
	}

}
void Supply::showSupplyInfo()
{
//	 QMessageBox msgBox;
	 QTableWidgetItem *selectedTableItem;
	 QString textSupply = " ";
//	 QModelIndex indexSupply;

	 selectedTableItem = getItemSelected();

	 ssView->stdProfilesTable->setCurrentItem(selectedTableItem);
	 textSupply = selectedTableItem->text();

//generate an information message box to show the supply info
	 QMessageBox::information( this, "Supply information", 
		QString("The selected supply is %1.").arg(textSupply) );

}

void Supply::setItemSelected(QTableWidgetItem * temp)
{
	m_ItemSelected = temp;
}

QTableWidgetItem * Supply::getItemSelected()
{
	return m_ItemSelected;
}
/*
void Supply::supplyInfo(QTableWidgetItem* item)
{

}
*/
void Supply::moveMiddle()
{
  QStringListModel modelSupply;
  QStringList strings;
  QTableWidgetItem *itemName = ssView->stdProfilesTable->item(ssView->stdProfilesTable->currentRow(),0);
  QPair<QString, QVector <float> > tempData;
  tempData.first = "";
  tempData.second.clear();
  tempData.second.reserve(8760);
  tempData.second.resize(8760);

  retrieveSupplyData(itemName->text());
//to check if the item to be added is contained within the list process supply widget
  if (ssProcess->listProcessSupplyWidget->findItems(itemName->text(), Qt::MatchExactly).count()!= 0)
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
	
	  processSupplyList.append(qMakePair(text, supplyDataVector));

	  prcSupplyStartDayList.append(qMakePair(text, getStartDay(itemName->text())));
	  prcSupplyEndDayList.append(qMakePair(text, getEndDay(itemName->text())));
	  prcSupplyHourStepsList.append(qMakePair(text, getHourSteps(itemName->text())));
	  
//	  modelSupply.setStringList(strings);
//	  for (int i =0; i< strings.count(); i++)
	  ssProcess->listProcessSupplyWidget-> insertItem(0, text);
	}
  }
//if it is not in the list, append it to the end of the list
  else
  {
//	  strings.append(itemName->text());
	  processSupplyList.append(qMakePair(itemName->text(), supplyDataVector));
	  prcSupplyStartDayList.append(qMakePair(itemName->text(), getStartDay(itemName->text())));
	  prcSupplyEndDayList.append(qMakePair(itemName->text(), getEndDay(itemName->text())));
	  prcSupplyHourStepsList.append(qMakePair(itemName->text(), getHourSteps(itemName->text())));

//	  modelSupply.setStringList( strings);
//	  for (int i =0; i< strings.count(); i++)
	  ssProcess->listProcessSupplyWidget-> insertItem(0, itemName->text());
  }

}


void Supply::moveRight()
{
  QStringListModel selectedSupply;
  QStringList strings;
 
  QPair<QString, QVector <float> > tempData;
  tempData.first = "";
  tempData.second.clear();
  tempData.second.reserve(8760);
  tempData.second.resize(8760);
	

//to check if the item to be added is contained within the list process supply widget
//  if (listSelectedSupplyWidget->findItems(itemName->text(), Qt::MatchExactly).count()!= 0)
  if (ssSelected->listSelectedSupplyWidget->findItems(m_ItemName, Qt::MatchExactly).count()!= 0)
  { 
    bool ok;
	//introduce a new InputDialog to ask user to input the name for the new item
	QString text = QInputDialog::getText(this,tr("Name already exist"),tr("Please enter a new name:"),QLineEdit::Normal,tr(" "),&ok);
	//if user press OK and text field is not empty, take the new name in the line edit field and insert the item to the end of the list
	if( ok && !text.isEmpty() )
	{
		//check if the item in the selected supply list is from the standard table widget 
		// or the item is from the middle list widget as the same item inside the standard table widget 
		  if (ssView->stdProfilesTable->findItems(m_ItemName, Qt::MatchExactly).count()!= 0)
		  {
			  retrieveSupplyData(m_ItemName);
			  selectedSupplyList.append(qMakePair(text, supplyDataVector));

			  selSupplyStartDayList.append(qMakePair(text, getStartDay(m_ItemName)));
			  selSupplyEndDayList.append(qMakePair(text, getEndDay(m_ItemName)));
			  selSupplyHourStepsList.append(qMakePair(text, getHourSteps(m_ItemName)));
		  }
		//if the item is from the middle list widget and not in the standard table widget
		  else
		  {
			  for (int i = 0; i < processSupplyList.count(); i++)
			  {
				  if (m_ItemName == processSupplyList[i].first)
				  {
					  selectedSupplyList.append(qMakePair(text, processSupplyList[i].second));

			          selSupplyStartDayList.append(qMakePair(text, getStartDay(m_ItemName)));
			          selSupplyEndDayList.append(qMakePair(text, getEndDay(m_ItemName)));
					  selSupplyHourStepsList.append(qMakePair(text, getHourSteps(m_ItemName)));
				  }
			  }
		  }
	//	  strings.append(itemName->text());
		  strings.append(m_ItemName);
		  selectedSupply.setStringList(strings);
		  for (int i =0; i< strings.count(); i++)
			  ssSelected->listSelectedSupplyWidget-> insertItem(i, text);
	}
  }
//if it is not in the list, append it to the end of the list
  else
  {
//	  strings.append(itemName->text());
		//check if the item in the selected supply list is from the standard table widget 
		// or the item is from the middle list widget as the same item inside the standard table widget 
		  if (ssView->stdProfilesTable->findItems(m_ItemName, Qt::MatchExactly).count()!= 0)
		  {
			  retrieveSupplyData(m_ItemName);
			  selectedSupplyList.append(qMakePair(m_ItemName, supplyDataVector));

	          selSupplyStartDayList.append(qMakePair(m_ItemName, getStartDay(m_ItemName)));
	          selSupplyEndDayList.append(qMakePair(m_ItemName, getEndDay(m_ItemName)));
			  selSupplyHourStepsList.append(qMakePair(m_ItemName, getHourSteps(m_ItemName)));
		  }
		//if the item is from the middle list widget and not in the standard table widget
		  else
		  {
			  for (int i = 0; i < processSupplyList.count(); i++)
			  {
				  if (m_ItemName == processSupplyList[i].first)
				  {
					  selectedSupplyList.append(processSupplyList[i]);

					  selSupplyStartDayList.append(qMakePair(m_ItemName, getStartDay(m_ItemName)));
					  selSupplyEndDayList.append(qMakePair(m_ItemName, getEndDay(m_ItemName)));
					  selSupplyHourStepsList.append(qMakePair(m_ItemName, getHourSteps(m_ItemName)));
				  }
			  }
		  }
		  strings.append(m_ItemName);
		  selectedSupply.setStringList(strings);
		  for (int i =0; i< strings.count(); i++)
			  ssSelected->listSelectedSupplyWidget-> insertItem(i, strings[i]);
  }
}
//Slot for the process list
void Supply::locateProcessListItem(const QPoint &pos)
{
//to create pop-up menu for the process supply list
	if(ssProcess->listProcessSupplyWidget->isItemSelected(ssProcess->listProcessSupplyWidget->itemAt(pos)))
	{
		setListItemSelected(ssProcess->listProcessSupplyWidget->itemAt(pos));
		QMenu* contextMenu = new QMenu(this);
		Q_CHECK_PTR (contextMenu);

		contextMenu->addAction("Graph", this, SLOT(drawSingleMiddleSupplyGraph()));
		contextMenu->addAction("Info", this, SLOT(showMiddleSupplyInfo()));
		contextMenu->addAction("Properties", this, SLOT(procSupplyProperty())); //include the start time, end time and number of timesteps between
		contextMenu->addSeparator();
		contextMenu->addAction("Select final", this, SLOT(moveRight()));
		contextMenu->addAction("Delete", this, SLOT(removeMiddle()));

		contextMenu->popup(QCursor::pos());
		contextMenu->exec();
		delete contextMenu;
		contextMenu = 0;
	}
//to create pop-up menu for the selected supply list

}

void Supply::showMiddleSupplyInfo()
{
//	 QMessageBox msgBox;
	 QListWidgetItem *selectedListItem;
	 QString textSupply = " ";
//	 QModelIndex indexSupply;

	 selectedListItem = getListItemSelected();

	 ssProcess->listProcessSupplyWidget->setCurrentItem(selectedListItem);
	 textSupply = selectedListItem->text();

//generate an information message box to show the supply info
	 QMessageBox::information( this, "Supply information", 
		QString("The selected supply is %1.").arg(textSupply) );

}
//for selected supplys
void Supply::setListItemSelected(QListWidgetItem * temp)
{
	m_ListItemSelected = temp;
}

QListWidgetItem * Supply::getListItemSelected()
{
	return m_ListItemSelected;
}

//for process supplys
void Supply::setProcessListItems(QListWidgetItem * temp)
{
	m_ProcessListItems = temp;
}

QListWidgetItem * Supply::getProcessListItems()
{
	return m_ProcessListItems;
}

//Slot for selected list
void Supply::locateSelectedListItem(const QPoint &pos)
{
//to create pop-up menu for the selected supply list
	if(ssSelected->listSelectedSupplyWidget->isItemSelected(ssSelected->listSelectedSupplyWidget->itemAt(pos)))
	{
		setListItemSelected(ssSelected->listSelectedSupplyWidget->itemAt(pos));

		QMenu* contextMenu = new QMenu(this);
		Q_CHECK_PTR (contextMenu);

		contextMenu->addAction("Graph", this, SLOT(drawSingleRightSupplyGraph()));
		contextMenu->addAction("Info", this, SLOT(showRightSupplyInfo()));
		contextMenu->addAction("Properties", this, SLOT(selSupplyProperty())); //include the start time, end time and number of timesteps between
		contextMenu->addSeparator();
		contextMenu->addAction("Delete", this, SLOT(removeRight()));

		contextMenu->popup(QCursor::pos());
		contextMenu->exec();
		delete contextMenu;
		contextMenu = 0;
	}
}

void Supply::showRightSupplyInfo()
{
//	 QMessageBox msgBox;
	 QListWidgetItem *selectedListItem;
	 QString textSupply = " ";
//	 QModelIndex indexSupply;

	 selectedListItem = getListItemSelected();

	 ssSelected->listSelectedSupplyWidget->setCurrentItem(selectedListItem);
	 textSupply = selectedListItem->text();

//generate an information message box to show the supply info
	 QMessageBox::information( this, "Supply information", 
		QString("The selected supply is %1.").arg(textSupply) );

}

void Supply::drawSingleRightSupplyGraph()
{
	QListWidgetItem * itemTemp;
	itemTemp = getListItemSelected();

//clear the previous plotted items
	supplyPlot->detachItems(QwtPlotItem::Rtti_PlotCurve, true);

	for (int i=0; i<selectedSupplyList.count(); i++)
	{
		if(itemTemp->text() == selectedSupplyList[i].first)
		{
	 		supplyPlot->drawSupplyGraph(itemTemp->text(), selectedSupplyList[i].second);//item->text(),supplyDataVector);
		}
	}
}


void Supply::drawRightSupplyGraph()
{
	QListWidgetItem * itemTemp;
	itemTemp = getListItemSelected();

//	retrieveSupplyData(itemTemp->text());

	QVector <QVector <float> > SupplySelectedVector;

	ItemSelected = ssSelected->listSelectedSupplyWidget->selectedItems();
//Allocate memory for selected supply vector (2 dimensions)	
	SupplySelectedVector.clear();
	SupplySelectedVector.reserve(ItemSelected.count());
	SupplySelectedVector.resize(ItemSelected.count());

	ItemNameVector.clear();
	ItemNameVector.reserve(ItemSelected.count());
	ItemNameVector.resize(ItemSelected.count());

	for (int j=0; j < SupplySelectedVector.size(); j++)
	{
		SupplySelectedVector[j].clear();
		SupplySelectedVector[j].reserve(8760);
		SupplySelectedVector[j].resize(8760);
	}
	int count = 0;
	count = ssSelected->selectedSupplyList.count();

	for (int i = 0; i< ItemSelected.count(); i++)
	{
		ItemNameVector[i] = ItemSelected[i]->text();
//check if the name selected is matched with which item in the selectedsupplylist
		SupplySelectedVector[i] = ssSelected->selectedSupplyList[i].second;
	}

	supplyPlot->drawMultipleSupplysGraph(ItemNameVector,SupplySelectedVector);

}

void Supply::setTableItemName(QTableWidgetItem *Item)
{
	m_ItemName = Item->text();
}

void Supply::setListItemName(QListWidgetItem *Item)
{
	m_ItemName = Item->text();
}


void Supply::removeMiddle()
{
	QString temp = "";
//	listProcessSupplyWidget->takeItem(listProcessSupplyWidget->currentRow());
	switch( QMessageBox::question(this, tr("Remove item"), tr("Are you sure to delete the item?"),
		    tr("&Yes"), tr("&No"), QString(), 0, 1))
	{
		case 0:
			//identify the item in the process list and delete the relevant info 
			//(i.e. start day, end day and hour steps) associated with the item 
				temp = ssProcess->listProcessSupplyWidget->currentItem()->text();
				for(int i=0; i<prcSupplyStartDayList.count(); i++)
				{
					if(temp == prcSupplyStartDayList[i].first)
					{
						prcSupplyStartDayList.takeAt(i);
						prcSupplyEndDayList.takeAt(i);
						prcSupplyHourStepsList.takeAt(i);
					}
				}
//delete the item in the process list widget
				ssProcess->listProcessSupplyWidget->takeItem(ssProcess->listProcessSupplyWidget->currentRow());
		break;

		case 1:
			return;
		break;

		default:
			return;
		break;
	}
}

void Supply::removeRight()
{
	QString temp = "";
//  listSelectedSupplyWidget->takeItem(listSelectedSupplyWidget->currentRow());
	switch( QMessageBox::question(this, tr("Remove item"), tr("Are you sure to delete the item?"),
		    tr("&Yes"), tr("&No"), QString(), 0, 1))
	{
		case 0:
			//identify the item in the selected list and delete the relevant info 
			//(i.e. start day, end day and hour steps) associated with the item 
				temp = ssSelected->listSelectedSupplyWidget->currentItem()->text();
				for(int i=0; i<selSupplyStartDayList.count(); i++)
				{
					if(temp == selSupplyStartDayList[i].first)
					{
						selSupplyStartDayList.takeAt(i);
						selSupplyEndDayList.takeAt(i);
						selSupplyHourStepsList.takeAt(i);

						selectedSupplyList.takeAt(i);
					}
				}
//delete the item in the selected list widget
				ssSelected->listSelectedSupplyWidget->takeItem(ssSelected->listSelectedSupplyWidget->currentRow());
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


//scale the profile
void Supply::scaleSupplyProfile()
{
  float sfactor = 1.0;
  QString  nameProfile = "";
  QString text = "";
  QVector<float> tempProfileVector;
  
  if( !ssProcess->listProcessSupplyWidget->currentItem() )
    return;
  
   nameProfile = ssProcess->listProcessSupplyWidget->currentItem()->text();
  
  ScaleDialog dlg( this );
//  sfactor = dlg.scaleFactor().toFloat();
  //found out which profile has been selected to scale down/up

  if( dlg.exec() == QDialog::Accepted )
  {
	  sfactor = dlg.scaleFactor().toFloat();

//apply the scaling factor to the supply   
	  for (int i = 0; i < processSupplyList.count(); i++)
	  {
		  if (nameProfile == processSupplyList[i].first)
		  {

			  for (int j=0; j < processSupplyList[i].second.count(); j++)
			  {
				  if (j == 0)
				  {
					tempProfileVector.clear();
					tempProfileVector.reserve(processSupplyList[i].second.count());
					tempProfileVector.resize(processSupplyList[i].second.count());
				  }

				  tempProfileVector[j] = sfactor * processSupplyList[i].second[j];
			  }
			  text = nameProfile+ "_" + dlg.scaleFactor() + "scaled";
			  processSupplyList[i].first = text;
			  processSupplyList[i].second = tempProfileVector;
           
//update the item name in the start day list, end day list and hour step list
			  prcSupplyStartDayList[i].first = text;
			  prcSupplyEndDayList[i].first = text;
              prcSupplyHourStepsList[i].first = text;

			  ssProcess->listProcessSupplyWidget->currentItem()->setText(text);
		  }
	  }
  }
}

//shift supply profile
void Supply::shiftSupplyProfile()
{
  int shift_steps = 0;
  int p = 0;
  int timesteps = 0;

  QString  nameProfile = "";
  QString text = "";
  QVector<float> tempProfileVector;
  
  if( !ssProcess->listProcessSupplyWidget->currentItem() )
    return;
  
   nameProfile = ssProcess->listProcessSupplyWidget->currentItem()->text();
  
  ShiftDialog dlg( this );

  if( dlg.exec() == QDialog::Accepted )
  {
	  shift_steps = dlg.numberOfSteps().toInt();
	  for (int i = 0; i < processSupplyList.count(); i++)
	  {
		  if (nameProfile == processSupplyList[i].first)
		  {
			  if (dlg.forwardDirection() && !dlg.backwardDirection()) //shift forward
			  {	
				p = 0;
				timesteps = processSupplyList[i].second.count();
				for (int j=0; j < processSupplyList[i].second.count(); j++)
				{
					if (j == 0)
					{
						tempProfileVector.clear();
						tempProfileVector.reserve(processSupplyList[i].second.count());
						tempProfileVector.resize(processSupplyList[i].second.count());
					}

					if (p < shift_steps)
					{
						tempProfileVector[j] = processSupplyList[i].second[timesteps-shift_steps+p];//assign the part of the supply outside boundary to supply the begining
						p++;
					}
					else
					{
						tempProfileVector[j] = processSupplyList[i].second[j-shift_steps];
					}
				}
			  }
			  else //shift backward
			  {
				p = 0;
				timesteps = processSupplyList[i].second.count();
				for (int j=0; j < processSupplyList[i].second.count(); j++)
				{
					if (j == 0)
					{
						tempProfileVector.clear();
						tempProfileVector.reserve(processSupplyList[i].second.count());
						tempProfileVector.resize(processSupplyList[i].second.count());
					}

					if (p < (timesteps-shift_steps))
					{
						tempProfileVector[j] = processSupplyList[i].second[j+shift_steps];
						p++;
					}
					else
					{	
						tempProfileVector[j] = processSupplyList[i].second[j-p];//assign the part of the supply outside boundary to supply the begining
					}
				}
			  }
			  text = nameProfile+ "_"  + "shifted";
			  processSupplyList[i].first = text;
			  processSupplyList[i].second = tempProfileVector;
//update the item name in the start day list, end day list and hour step list
			  prcSupplyStartDayList[i].first = text;
			  prcSupplyEndDayList[i].first = text;
              prcSupplyHourStepsList[i].first = text;

			  ssProcess->listProcessSupplyWidget->currentItem()->setText(text);

		  }
		}
	}
}

void Supply::combineSupplyProfile()
{
	QVector <QVector <float> > processSupplyVector;
	processItems = ssProcess->listProcessSupplyWidget->selectedItems();
//Allocate memory for selected supply vector (2 dimensions)	
	processSupplyVector.clear();
	processSupplyVector.reserve(processItems.count()+1);
	processSupplyVector.resize(processItems.count()+1);

	processItemNameVector.clear();
	processItemNameVector.reserve(processItems.count()+1);
	processItemNameVector.resize(processItems.count()+1);

	for (int j=0; j < processSupplyVector.size(); j++)
	{
		processSupplyVector[j].clear();
		processSupplyVector[j].reserve(8760);
		processSupplyVector[j].resize(8760);
	}

	int count = 0;
	int ccomb = 0;
	count = processSupplyList.count();
	ccomb = processItems.count()+1;

	for (int i = 0; i< processItems.count()+1; i++)
	{
		if (i < processItems.count())
		{
			processItemNameVector[i] = processItems[i]->text();
//check if the name selected is matched with which item in the selectedsupplylist
			for (int k=0; k<count; k++)
			{
				if (processItems[i]->text() == processSupplyList[k].first)
	 				processSupplyVector[i] = processSupplyList[k].second;
			}
//			processSupplyVector[i] = processSupplyList[i].second;
			for (int j(processSupplyVector[i].size() - 1); j >= 0; --j)
				processSupplyVector[processItems.count()][j] += processSupplyVector[i][j];
		}
		else
		{
			processItemNameVector[i] = "combined supply";
		}
	}

//add to the list
	  QStringList strings;
  QListWidgetItem *itemName = 
	  ssProcess->listProcessSupplyWidget->item(ssProcess->listProcessSupplyWidget->currentRow());

//to check if the item to be added is contained within the list process supply widget
  if (ssProcess->listProcessSupplyWidget->findItems(processItemNameVector[ccomb-1], Qt::MatchExactly).count()!= 0)
  { 
    bool ok;
	//introduce a new InputDialog to ask user to input the name for the new item
	QString text = QInputDialog::getText(this,tr("Name already exist"),tr("Please enter a new name:"),QLineEdit::Normal,tr(" "),&ok);
	//if user press OK and text field is not empty, take the new name in the line edit field and insert the item to the end of the list
	if( ok && !text.isEmpty() )
	{
	  strings.append(text);
	  processSupplyList.append(qMakePair(text, processSupplyVector[ccomb-1]));
	  
	  prcSupplyStartDayList.append(qMakePair(text, getStartDay(itemName->text())));
	  prcSupplyEndDayList.append(qMakePair(text, getEndDay(itemName->text())));
	  prcSupplyHourStepsList.append(qMakePair(text, getHourSteps(itemName->text())));

	  for (int i =0; i< strings.count(); i++)
		  ssProcess->listProcessSupplyWidget-> insertItem(i, text);
	}
  }
//if it is not in the list, append it to the end of the list
  else
  {
	  strings.append(processItemNameVector[ccomb-1]);
	  processSupplyList.append(qMakePair(processItemNameVector[ccomb-1], processSupplyVector[ccomb-1]));

	  prcSupplyStartDayList.append(qMakePair(processItemNameVector[ccomb-1], getStartDay(itemName->text())));
	  prcSupplyEndDayList.append(qMakePair(processItemNameVector[ccomb-1], getEndDay(itemName->text())));
	  prcSupplyHourStepsList.append(qMakePair(processItemNameVector[ccomb-1], getHourSteps(itemName->text())));

	  for (int i =0; i< strings.count(); i++)
		  ssProcess->listProcessSupplyWidget-> insertItem(i, strings[i]);
  }
}


void Supply::clearSupplyProfile()
{
	//	listProcessSupplyWidget->takeItem(listProcessSupplyWidget->currentRow());
	switch( QMessageBox::question(this, tr("Clear all"), tr("Are you sure to clear all the items?"),
		    tr("&Yes"), tr("&No"), QString(), 0, 1))
	{
		case 0:
			processSupplyList.clear();
			ssProcess->listProcessSupplyWidget->clear();
			prcSupplyStartDayList.clear();
			prcSupplyEndDayList.clear();
			prcSupplyHourStepsList.clear();
		break;

		case 1:
			return;
		break;

		default:
			return;
		break;
	}
}

//delete the current scenario in the tree view
void Supply::deleteCurrentScenario()
{
//check if the header or item in the tree view has been selected
	if (tree->selectionModel()->hasSelection())
	{

		switch( QMessageBox::question(this, tr("Delete supply scenario"), tr("Are you sure to delete the current supply scenario?"),
				tr("&Yes"), tr("&No"), QString(), 0, 1))
		{
			case 0:
					removeCurrentScenario();
			break;

			case 1:
				return;
			break;

			default:
				return;
			break;
		}
	}
}

void Supply::removeCurrentScenario()
{
	QList <QString> listSupplys;
	QStringList temp1;
	QStringList temp2;
	QPair<QString, QVector<float> > supplyProfileData;
	listSupplys.clear();
	int rowCount = 0; 
	int noScenario = 0;

	QString supplys = "";

	if (model->hasChildren(tree->currentIndex()))
	{
		supplys = model->data(tree->currentIndex(), Qt::DisplayRole).toString();
	}
	else
	{
		supplys = model->data(model->parent(tree->currentIndex()), Qt::DisplayRole).toString();
	}

	//navigate the elements within the map of supply scenario using iternator
	QString tempSS = "";
	QMap<QString, QString>::const_iterator i = mapSS.constBegin();
	while (i != mapSS.constEnd()) 
	{
		if (i.value() == supplys)
		{
			tempSS = i.key();
			break;
		}
		++i;
	}
	//set up the supply scenario using the data in the selected list
	for (int i=0; i < supplyScenarioPortfolio.count(); i++)
	{
		if (tempSS == supplyScenarioPortfolio[i].first)
		{
			supplyScenarioPortfolio.takeAt(i);
			break;
		}
	}

//prpeare the list for the updated profiles 	
	m_SupplyScenarioData.clear();
	mapSS.clear();
	QString tempString = "";
	for (int k= 0; k < supplyScenarioPortfolio.count(); k++)
	{
		m_SupplyScenarioData.append(supplyScenarioPortfolio[k].first);
		for (int m = 0; m < supplyScenarioPortfolio[k].second.count(); m++)
		{
		   tempString = (supplyScenarioPortfolio[k].second[m].supplyName());
		   if (tempString == "")
			   tempString = "no profile has been selected yet.";
		   tempString.prepend(" ");
		   m_SupplyScenarioData.append(tempString);
		}
		mapSS[supplyScenarioPortfolio[k].first] = supplyScenarioName[k]; //make a map for later when user change the name of supply scenario
	}


//emit a signal for supply scenario for the match to invoke the slot
	emit valueChanged(mapSS);
	emit valueChangedSSP(supplyScenarioPortfolio);

//assign the new data to the model
	model = new SupplyScenarioTreeModel(m_SupplyScenarioData);
//set the view settings for TreeView
	tree->setSelectionMode(QAbstractItemView::SingleSelection);
	tree->header()->setClickable(true);
//deploy the profile to the selected list widget
	connect(tree, SIGNAL(clicked()), this, SLOT(deployProfiles()));

//set up the model with the tree view
	tree->setModel(model);

//expand the tree after the model has been set up
	for (int k=0; k<m_SupplyScenarioData.count(); k++)
		tree->setExpanded(model->index(k,0), true);
//show the view
	tree->show();

//reduce the number of scenario
	m_SupScnNo--;	
}

void Supply::deployProfiles()
{
	QStringList temp;
	QString supplys = "";
	QString tempString = "";

	int position = 0;
	int next_position = 0;

	if (tree->selectionModel()->hasSelection())
	{
//if the current index is the parent item
		if (model->hasChildren(tree->currentIndex()))
		{
			supplys = model->data(tree->currentIndex(), Qt::DisplayRole).toString();

			//navigate the elements within the map of supply scenario using iternator
			QString tempSS = "";
			QMap<QString, QString>::const_iterator i = mapSS.constBegin();
			while (i != mapSS.constEnd()) 
			{
				if (i.value() == supplys)
				{
					tempSS = i.key();
					break;
				}
				++i;
			}

			//set up the supply scenario using the data in the selected list
			for (int i=0; i < supplyScenarioPortfolio.count(); i++)
			{
				if (tempSS == supplyScenarioPortfolio[i].first)
				{
					//add the list the RightListWidget for the selected supplys
					ssSelected->listSelectedSupplyWidget->clear();
					//insert the item to the selected list widget
					for (int j = 0; j < supplyScenarioPortfolio[i].second.count(); j++)
					{
					   tempString = (supplyScenarioPortfolio[i].second[j].supplyName());
					   ssSelected->listSelectedSupplyWidget-> insertItem(j, (supplyScenarioPortfolio[i].second[j].supplyName()));
					}
					break;
				}
			}
		}
	}
}

//slot to set the propeties for the available profiles
void Supply::supplyProperty()
{
 int flag = 0;
//check if there is any item has been selected in the standard supply profile table widget
  if( !ssView->stdProfilesTable->currentItem() )
    return;

  retrieveSupplyData(ssView->stdProfilesTable->currentItem()->text());

  QString temp = ssView->stdProfilesTable->currentItem()->text();

  SupplyPropertyDialog dlg( this );

  for (int i=0;i<supplyStartDayList.count(); i++)
  {
	  if (temp == supplyStartDayList[i].first)
	  {
		dlg.setStartDay(start_day_list, supplyStartDayList[i].second);
		dlg.setEndDay(end_day_list, supplyEndDayList[i].second);
		dlg.setHourSteps(hour_steps_list, supplyHourStepsList[i].second);
		flag = i;
		break;
	  }
  }

  if( dlg.exec() == QDialog::Accepted )
  {
	supplyStartDayList[flag].second = dlg.startDay();
	supplyEndDayList[flag].second = dlg.endDay();
	supplyHourStepsList[flag].second = dlg.hourSteps();
  }
}

//slot to set the propeties for the process profiles
void Supply::procSupplyProperty()
{
 int flag = 0;
//check if there is any item has been selected in the standard supply profile table widget
  if( !ssProcess->listProcessSupplyWidget->currentItem() )
    return;

  QString temp = ssProcess->listProcessSupplyWidget->currentItem()->text();

  SupplyPropertyDialog dlg( this );

  for (int i=0;i<prcSupplyStartDayList.count(); i++)
  {
	  if (temp == prcSupplyStartDayList[i].first)
	  {
		dlg.setStartDay(start_day_list, prcSupplyStartDayList[i].second);
		dlg.setEndDay(end_day_list, prcSupplyEndDayList[i].second);
		dlg.setHourSteps(hour_steps_list, prcSupplyHourStepsList[i].second);
		flag = i;
		break;
	  }
  }

  if( dlg.exec() == QDialog::Accepted )
  {
	prcSupplyStartDayList[flag].second = dlg.startDay();
	prcSupplyEndDayList[flag].second = dlg.endDay();
	prcSupplyHourStepsList[flag].second = dlg.hourSteps();
  }
}

//slot to set the propeties for the selected profiles
void Supply::selSupplyProperty()
{
 int flag = 0;
//check if there is any item has been selected in the standard supply profile table widget
  if( !ssSelected->listSelectedSupplyWidget->currentItem() )
    return;

  QString temp = ssSelected->listSelectedSupplyWidget->currentItem()->text();

//show the property dialog
  SupplyPropertyDialog dlg( this );

  for (int i=0;i<selSupplyStartDayList.count(); i++)
  {
	  if (temp == selSupplyStartDayList[i].first)
	  {
		dlg.setStartDay(start_day_list, selSupplyStartDayList[i].second);
		dlg.setEndDay(end_day_list, selSupplyEndDayList[i].second);
		dlg.setHourSteps(hour_steps_list, selSupplyHourStepsList[i].second);
		flag = i;
		break;
	  }
  }

  if( dlg.exec() == QDialog::Accepted )
  {
	selSupplyStartDayList[flag].second = dlg.startDay();
	selSupplyEndDayList[flag].second = dlg.endDay();
	selSupplyHourStepsList[flag].second = dlg.hourSteps();
  }
}

//convert to julian day from day and month
/* Private Member Function: Converts calender date to julian day.
   Returns julian day for any given day of month, and accounts for leap years(?).*/   
int Supply::convertToJulian(int Day,int Month)
{
	int J=0;
	switch (Month)
	{
		case 1:
			J=Day;
			break;
		case 2:
			J=31+Day;
			break;
		case 3:
			J=59+Day;
			break;
		case 4:
			J=90+Day;
			break;
		case 5:
			J=120+Day;
			break;
		case 6:
			J=151+Day;
			break;
		case 7:
			J=181+Day;
			break;
		case 8:
			J=212+Day;
			break;
		case 9:
			J=243+Day;
			break;
		case 10:
			J=273+Day;
			break;
		case 11:
			J=304+Day;
			break;
		case 12:
			J=334+Day;
			break;
	}//end case

	return J;
}

//supply the data for the three comboboxes in the supply property dialog
void Supply::setTemporalParameterList()
{
  int numberOfDataPoints = 0;
  int start_day = 0;
  int end_day = 0;
  int start_month = 0;
  int end_month = 0;
  int timeStepsPerHour = 0;	
  int julian_start_day = 0;
  int julian_end_day = 0;

  start_day_list.clear();
  end_day_list.clear();
  hour_steps_list.clear();

  QStringList start_date = supplyDateDataVector[0].split(" ");
  QStringList end_date = supplyDateDataVector[8759].split(" ");

  QStringList start_date_parts = start_date[0].split("-");
  QStringList end_date_parts = end_date[0].split("-");

  start_day = (start_date_parts[2].trimmed()).toInt();
  end_day = (end_date_parts[2].trimmed()).toInt();

  start_month = (start_date_parts[1].trimmed()).toInt();
  end_month = (end_date_parts[1].trimmed()).toInt();

  julian_start_day = convertToJulian(start_day, start_month);
  julian_end_day = convertToJulian(end_day, end_month);

  numberOfDataPoints = supplyDateDataVector.count();

  if (julian_start_day <= julian_end_day)
  {
	for (int i = julian_start_day; i < (julian_end_day+1); i++)
	{
		start_day_list.append(QString("%1").arg(i));
	}
	for (int j = julian_end_day; j > (julian_start_day-1); j--)
	{
		end_day_list.append(QString("%1").arg(j));
	}
	timeStepsPerHour = (int)numberOfDataPoints/((julian_end_day - julian_start_day + 1)*24);
	if (timeStepsPerHour > 1) //for the moment, assume the minimum steps is second-based
	{
		for (int i = timeStepsPerHour; i > 0; i--)
		{
		  hour_steps_list.append(QString("%1").arg(i));
		}
	}
	else
	{
	  hour_steps_list.append(QString("%1").arg(timeStepsPerHour));
	}
  }
  else
  {
	  julian_start_day = julian_end_day;
	  timeStepsPerHour = (int)numberOfDataPoints/24;

	  start_day_list.append(QString("%1").arg(julian_end_day));
      end_day_list.append(QString("%1").arg(julian_end_day));
	  hour_steps_list.append(QString("%1").arg(timeStepsPerHour));
  }
}


////
int Supply::getStartDay(const QString& itemName)
{
	for(int i=0; i<supplyStartDayList.count(); i++)
	{
		if(itemName == supplyStartDayList[i].first)
		{
			return supplyStartDayList[i].second;
		}
	}
	return 0;
}

int Supply::getEndDay(const QString& itemName)
{
	for(int i=0; i<supplyEndDayList.count(); i++)
	{
		if(itemName == supplyEndDayList[i].first)
		{
			return supplyEndDayList[i].second;
		}
	}
	return 0;
}

int Supply::getHourSteps(const QString& itemName)
{
	for(int i=0; i<supplyHourStepsList.count(); i++)
	{
		if(itemName == supplyHourStepsList[i].first)
		{
			return supplyHourStepsList[i].second;
		}
	}
	return 0;
}
