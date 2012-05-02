#include <QtGui>
#include "demand.h"
#include <QtCore/QFile>
#include "demandscenariotreemodel.h"
#include "demandtreeitem.h"
//#include "scaledialog.h"

class DemandScenarioTreeModel;
class DemandTreeItem;


using namespace std;

Demand::Demand(QWidget *parent)
    : QWidget(parent)//, QAbstractItemView( parent )
{
//initial number of demand scenario
	m_ItemName = "";
	m_DmdScnNo = 0;
	m_iStartDay = 0;
	m_iEndDay = 0;
	m_iHourSteps = 0;
	int itemNumber = 0;

	processDemandList.clear();

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

//load the customised demand widget
	dsView = new DemandProfile;
	dsView->setObjectName(QString::fromUtf8("dsView"));

	itemNumber = dsView->stdProfilesTable->rowCount();
	initialiseTemporalList(itemNumber);

//	qwtPlot = new QwtPlot();
	demandPlot = new PlotDemand();
	QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy2.setHorizontalStretch(0);
	sizePolicy2.setVerticalStretch(0);
	sizePolicy2.setHeightForWidth(demandPlot->sizePolicy().hasHeightForWidth());
	demandPlot->setSizePolicy(sizePolicy2);


	splitterVrt = new QSplitter(Qt::Vertical);

	splitterHrt->addWidget(widget);
	splitterHrt->addWidget(dsView);

	splitterHrt->setStretchFactor(0,10);
	splitterHrt->setStretchFactor(1,13);

	splitterVrt->addWidget(splitterHrt);
	splitterVrt->addWidget(demandPlot);

	splitterVrt->setStretchFactor(0,5);
	splitterVrt->setStretchFactor(1,2);


	gridLayout->addWidget(splitterVrt);
//	 QStringList data;
	 m_DemandScenarioData.clear();
	 m_DemandScenarioData << "No scenario has been specified yet."; //<< "   no profile has been selected yet.";
     model = new DemandScenarioTreeModel(m_DemandScenarioData);

	tree->setModel(model);
	tree->show();
	 m_DemandScenarioData.clear();

//double click an item in the table widget to draw the graph
	connect(dsView->stdProfilesTable, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(drawDemandGraph(QTableWidgetItem *)));

//double click an item in the middle list widget to draw the graph
//	connect(dsView->listProcessDemandWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(drawDemandGraph(QListWidgetItem *)));

//single click an item in the right list widget to draw the demand graph
//	connect(dsView->listSelectedDemandWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(drawRightDemandGraph()));//QListWidgetItem *)));

//select multiple items in the right list widget to draw aggregate demands
//    connect(dsView->listSelectedDemandWidget, SIGNAL(itemSelectionChanged()), this, SLOT(drawMultipleDemands()));

//	connect(dsView->saveScenarioButton, SIGNAL(clicked()), this, SLOT(saveCurrentScenario()));
//	connect(dsView->saveasScenarioButton, SIGNAL(clicked()), this, SLOT(saveCurrentScenarioAs()));

//	connect(tree, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(ItemName()));
//	connect(tree, SIGNAL(clicked(const QModelIndex &)), this, SLOT(populateProfiles()));
	connect(tree, SIGNAL(clicked(const QModelIndex &)), this, SLOT(deployProfiles()));

	connect(addButton, SIGNAL(clicked()), this, SLOT(addDemandScenario()));
	connect(delButton, SIGNAL(clicked()), this, SLOT(deleteCurrentScenario()));
	connect(dsView->stdProfilesTable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(LocateItem(const QPoint &)));
//	connect(&a, SIGNAL(valueChanged(int)), &b, SLOT(setValue(int)));
}

Demand::~Demand()
{

}

void Demand::initialiseTemporalList(int n)
{
	//allocate 8760 to the vectors for storing demand data
	demandStartDayList.clear();
	demandEndDayList.clear();
	demandHourStepsList.clear();

    for (int i=0; i<n; i++)
	{
		demandStartDayList.append(qMakePair(dsView->stdProfilesTable->item(i,0)->text(), 0));
		demandEndDayList.append(qMakePair(dsView->stdProfilesTable->item(i,0)->text(), 0));
		demandHourStepsList.append(qMakePair(dsView->stdProfilesTable->item(i,0)->text(), 0));
	}
}

void Demand::addDemandScenario()
{
	if (demandScenarioPortfolio.count()!=0)
	{
		m_DmdScnNo = demandScenarioPortfolio.count()+1;

//		m_DemandScenarioData.append(QString("ds %0").arg(m_DmdScnNo));
//		m_DemandScenarioData.append("   no profile has been selected yet.");

//clear the list of demand prfoile;
		demandProfileList.clear();

//initialise the data required for a specific demand profile
		DemandData demandProfileData; //represent one specific demand profile
//push the demand profile to a list
		demandProfileList.append(demandProfileData); //
//make pair with the demand scenario name and push back to a demand scenario list
		demandScenarioPortfolio.append(qMakePair(QString("ds %0").arg(m_DmdScnNo), demandProfileList));
//put the reference id for the demand scenario into a list
		demandScenarioName.append(QString("ds %0").arg(m_DmdScnNo));
	}
	else
	{
//		m_DemandScenarioData << QString("ds %0").arg(1) << "   no profile has been selected yet.";
		m_DmdScnNo++;
//establish the new interface for the demand profile
//load the customised demand widget
		dsProcess = new DemandProcess;
		dsProcess->setObjectName(QString::fromUtf8("dsProcess"));
//		dsProcess->list->setSelectionMode(QAbstractItemView::ExtendedSelection);

		dsSelected = new DemandSelection;
		dsSelected->setObjectName(QString::fromUtf8("dsSelected"));

		splitterHrt->addWidget(dsProcess);
		splitterHrt->addWidget(dsSelected);	
		
		connect(dsProcess->accelerateButton, SIGNAL(clicked()), this, SLOT(moveMiddle()));
		connect(dsSelected->accelerateButton, SIGNAL(clicked()), this, SLOT(moveRight()));
		connect(dsProcess->listProcessDemandWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(locateProcessListItem(const QPoint &)));
		connect(dsSelected->listSelectedDemandWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(locateSelectedListItem(const QPoint &)));

		connect(dsView->stdProfilesTable, SIGNAL(itemClicked  (QTableWidgetItem *)), this, SLOT(setTableItemName(QTableWidgetItem *)));
		connect(dsProcess->listProcessDemandWidget, SIGNAL(itemClicked  (QListWidgetItem *)), this, SLOT(setListItemName(QListWidgetItem *)));
		connect(dsProcess->scaleButton, SIGNAL(clicked()), this, SLOT(scaleDemandProfile()));
		connect(dsProcess->shiftButton, SIGNAL(clicked()), this, SLOT(shiftDemandProfile()));
		connect(dsProcess->combineButton, SIGNAL(clicked()), this, SLOT(combineDemandProfile()));
		connect(dsProcess->clearButton, SIGNAL(clicked()), this, SLOT(clearDemandProfile()));

		connect(dsSelected->saveButton, SIGNAL(clicked()), this, SLOT(saveCurrentScenario()));
		connect(dsSelected->saveasButton, SIGNAL(clicked()), this, SLOT(saveCurrentScenarioAs()));

		initialiseProcessDemandTemporalList();
		initialiseSelectedDemandTemporalList();

//clear the list of demand prfoile;
		demandProfileList.clear();
//clear the list for the demand scenario name
		demandScenarioName.clear();
//initialise the data required for a specific demand profile
		DemandData demandProfileData;
//push the demand profile to a list
		demandProfileList.append(demandProfileData);
//make pair with the demand scenario name and push back to a demand scenario list
		demandScenarioPortfolio.append(qMakePair(QString("ds %0").arg(m_DmdScnNo), demandProfileList));
//put the reference id for the demand scenario into a list
		demandScenarioName.append(QString("ds %0").arg(m_DmdScnNo));
	}


	//prepare the list for the model
	m_DemandScenarioData.clear();
	QString tempString = "";
	for (int k= 0; k < demandScenarioPortfolio.count(); k++)
	{
		m_DemandScenarioData.append(demandScenarioPortfolio[k].first);
		for (int m = 0; m < demandScenarioPortfolio[k].second.count(); m++)
		{
		   tempString = (demandScenarioPortfolio[k].second[m].demandName());
		   if (tempString == "")
			   tempString = "no profile has been selected yet.";
		   tempString.prepend(" ");
		   m_DemandScenarioData.append(tempString);
		}
		mapDS[demandScenarioPortfolio[k].first] = demandScenarioName[k]; //make a map for later when user change the name of demand scenario
	}
//emit a signal for demand scenario for the match to invoke the slot
//	emit valueChanged(m_DmdScnNo);
	emit valueChanged(mapDS);
	emit valueChangedDSP(demandScenarioPortfolio);

//assign the data to the model
    model = new DemandScenarioTreeModel(m_DemandScenarioData); //
//	model = new DemandScenarioTreeModel(demandScenarioPortfolio);

//set the view settings for TreeView
	tree->setSelectionMode(QAbstractItemView::SingleSelection);
	tree->header()->setClickable(true);

//set up the model to the tree view
	tree->setModel(model);

//expand the tree after the model has been set up
	for (int k=0; k<m_DemandScenarioData.count(); k++)
		tree->setExpanded(model->index(k,0), true);

//show the tree view
	tree->show();
}

void Demand::initialiseProcessDemandTemporalList()
{
	prcDemandStartDayList.clear();
	prcDemandEndDayList.clear();
	prcDemandHourStepsList.clear();
}

void Demand::initialiseSelectedDemandTemporalList()
{
	selDemandStartDayList.clear();
	selDemandEndDayList.clear();
	selDemandHourStepsList.clear();
}

//draw the graph of the selected demand in standard profile table 
void Demand::drawDemandGraph()
{
//	retrieveWeatherData(d->m_sWeatherSite);
//	QList <float> demandData;
	QTableWidgetItem * itemTemp;
	itemTemp = getItemSelected();
	demandPlot->detachItems(QwtPlotItem::Rtti_PlotCurve, true);

	retrieveDemandData(itemTemp->text());
 	demandPlot->drawDemandGraph(itemTemp->text(),demandDataVector);

}

void Demand::drawSingleMiddleDemandGraph()
{
	QListWidgetItem * itemTemp;
	itemTemp = getListItemSelected();
//clear the previous plotted items
	demandPlot->detachItems(QwtPlotItem::Rtti_PlotCurve, true);
	for (int i=0; i<processDemandList.count(); i++)
	{
		if(itemTemp->text() == processDemandList[i].first)
		{
	 		demandPlot->drawDemandGraph(itemTemp->text(), processDemandList[i].second);//item->text(),demandDataVector);
		}
	}

//	retrieveDemandData(itemTemp->text());
// 	demandPlot->drawDemandGraph(itemTemp->text(),demandDataVector);
/*
	int count = 0;
	count = dsView->processDemandList.count();
	for (int i=0; i<count; i++)
	{
		if (item->text() == dsView->processDemandList[i].first)
	 		demandPlot->drawDemandGraph(item->text(), dsView->processDemandList[i].second);//item->text(),demandDataVector);
	}
*/

}
/*
void Demand::drawRightDemandGraph()//QListWidgetItem * items)
{
//	retrieveWeatherData(d->m_sWeatherSite);
//	QList <float> demandData;
	QList<QListWidgetItem *> ItemSelected;

	QVector <QVector <float> > DemandSelectedVector;

	ItemSelected = dsView->listSelectedDemandWidget->selectedItems();
//Allocate memory for selected demand vector (2 dimensions)	
	DemandSelectedVector.clear();
	DemandSelectedVector.reserve(ItemSelected.count()+1);
	DemandSelectedVector.resize(ItemSelected.count()+1);

	ItemNameVector.clear();
	ItemNameVector.reserve(ItemSelected.count()+1);
	ItemNameVector.resize(ItemSelected.count()+1);

	for (int j=0; j < DemandSelectedVector.size(); j++)
	{
		DemandSelectedVector[j].clear();
		DemandSelectedVector[j].reserve(8760);
		DemandSelectedVector[j].resize(8760);
	}
	int count = 0;
	count = dsView->selectedDemandList.count();

	for (int i = 0; i< ItemSelected.count()+1; i++)
	{
		if(i < ItemSelected.count())
		{
			ItemNameVector[i] = ItemSelected[i]->text();
//check if the name selected is matched with which item in the selecteddemandlist
			for (int k=0; k<count; k++)
			{
				if (ItemSelected[i]->text() == dsView->selectedDemandList[k].first)
	 				DemandSelectedVector[i] = dsView->selectedDemandList[k].second;
			}
//
//			retrieveDemandData(ItemSelected[i]->text());
//			DemandSelectedVector[i] = demandDataVector;
			for (int j(DemandSelectedVector[i].size() - 1); j >= 0; --j)
				DemandSelectedVector[ItemSelected.count()][j] += DemandSelectedVector[i][j];
		}
		else
		{
			ItemNameVector[i] = "Total";
		}
	}
//	retrieveDemandData(item->text());
//	if (ItemNameVector.size() == 1)
	demandPlot->drawMultipleDemandsGraph(ItemNameVector,DemandSelectedVector);


//	else if (ItemNameVector.size() == 2)
//		demandPlot2->drawMultipleDemandsGraph(ItemNameVector,DemandSelectedVector);
//	demandPlot->replot();
}
/*
void Demand::drawMultipleDemands(QListWidgetItem * items)
{
//	retrieveWeatherData(d->m_sWeatherSite);
//	QList <float> demandData;
	retrieveDemandData(item->text());
 	demandPlot->drawDemandGraph(item->text(),demandDataVector);

}
*/
//QList<QListWidgetItem *> QListWidget::selectedItems () const
//Returns a list of all selected items in the list widget.


void Demand::retrieveDemandData(const QString& name)
{
	int i=0;
//hourly demand profile
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

	//navigate through the fected query result set
	int temp = 0;

	while (query.next()) 
	{
		setDemandData(i, query.value(0).toFloat());
		setDemandDateData(i, query.value(1).toString());
		i++;
    }
	setTemporalParameterList();
}
float Demand::getDemandData(int index)
{
	return demandDataVector[index];
}

void Demand::setDemandData(int index, float demand)
{
	demandDataVector[index] = demand;
}

QString Demand::getDemandDateData(int index)
{
	return demandDateDataVector[index];
}

void Demand::setDemandDateData(int index, QString demand_date)
{
	demandDateDataVector[index] = demand_date;
}

void Demand::AllocateMemory(int n)
{
	//allocate 8760 to the vectors for storing demand data
	demandDataVector.clear();
    demandDataVector.reserve(n);
    demandDataVector.resize(n);

	demandTotalVector.clear();
    demandTotalVector.reserve(n);
    demandTotalVector.resize(n);

	demandDateDataVector.clear();
    demandDateDataVector.reserve(n);
    demandDateDataVector.resize(n);
}

//save the current the scenario in the tree view
void Demand::saveCurrentScenario()
{
	QList <QString> listDemands;
	QStringList temp1;
	QStringList temp2;
	QPair<QString, QVector<float> > demandProfileData;
	listDemands.clear();
	int rowCount = 0; 
	QString demands = "";

//rowCount: the number of profiles in the selected list widget
	rowCount = dsSelected->listSelectedDemandWidget->count();

//check if the header or item in the tree view has been selected
	if (tree->selectionModel()->hasSelection())
	{
		if (model->hasChildren(tree->currentIndex()))
		{
			demands = model->data(tree->currentIndex(), Qt::DisplayRole).toString();
		}
		else
		{
			demands = model->data(model->parent(tree->currentIndex()), Qt::DisplayRole).toString();
		}

		//navigate the elements within the map of demand scenario using iternator
		QString tempDS = "";
		QMap<QString, QString>::const_iterator i = mapDS.constBegin();
		while (i != mapDS.constEnd()) 
		{
			if (i.value() == demands)
			{
				tempDS = i.key();
				break;
			}
			++i;
		}
		//set up the demand scenario using the data in the selected list
		for (int i=0; i < demandScenarioPortfolio.count(); i++)
		{
			if (tempDS == demandScenarioPortfolio[i].first)
			{
				//clear the list of demand prfoile;
				demandProfileList.clear();
				//initialise the data required for a specific demand profile
				DemandData demandProfileData; //represent one specific demand profile

				for (int j = 0; j < selectedDemandList.count(); j++)
				{
					demandProfileData.setDemandName(selectedDemandList[j].first);
					demandProfileData.setStartDay(selDemandStartDayList[j].second);
					demandProfileData.setEndDay(selDemandStartDayList[j].second);
					demandProfileData.setHourSteps(selDemandHourStepsList[j].second);
					demandProfileData.m_pDemandDataVector = selectedDemandList[j].second;

					//push the demand profile to a list
					demandProfileList.append(demandProfileData);
				}
				//copy the demand profile list to the demand scenario 
				demandScenarioPortfolio[i].second = demandProfileList;

				//terminate the loop
				break;
			}			
		}

	}
	else
	{
		if (m_DmdScnNo == 1)
		{
			//clear the list of demand prfoile;
			demandProfileList.clear();
			//initialise the data required for a specific demand profile
			DemandData demandProfileData; //represent one specific demand profile

			for (int j = 0; j < selectedDemandList.count(); j++)
			{
				demandProfileData.setDemandName(selectedDemandList[j].first);
				demandProfileData.setStartDay(selDemandStartDayList[j].second);
				demandProfileData.setEndDay(selDemandStartDayList[j].second);
				demandProfileData.setHourSteps(selDemandHourStepsList[j].second);
				demandProfileData.m_pDemandDataVector = selectedDemandList[j].second;

				//push the demand profile to a list
				demandProfileList.append(demandProfileData);
			}
			//copy the demand profile list to the demand scenario 
			demandScenarioPortfolio[0].second = demandProfileList;
		}
	}

//prpeare the list for the updated profiles 	
	m_DemandScenarioData.clear();
	QString tempString = "";
	for (int k= 0; k < demandScenarioPortfolio.count(); k++)
	{
		m_DemandScenarioData.append(demandScenarioPortfolio[k].first);
		for (int m = 0; m < demandScenarioPortfolio[k].second.count(); m++)
		{
		   tempString = (demandScenarioPortfolio[k].second[m].demandName());
		   if (tempString == "")
			   tempString = "no profile has been selected yet.";
		   tempString.prepend(" ");
		   m_DemandScenarioData.append(tempString);
		}
		mapDS[demandScenarioPortfolio[k].first] = demandScenarioName[k]; //make a map for later when user change the name of demand scenario
	}

//emit a signal for demand scenario for the match to invoke the slot
//	emit valueChanged(m_DmdScnNo);
//	emit valueChanged(mapDS);
	emit valueChangedDSP(demandScenarioPortfolio);
//assign the new data to the model
    model = new DemandScenarioTreeModel(m_DemandScenarioData);
//set the view settings for TreeView
	tree->setSelectionMode(QAbstractItemView::SingleSelection);
	tree->header()->setClickable(true);

//set up the model with the tree view
	tree->setModel(model);

//expand the tree after the model has been set up
	for (int k=0; k<m_DemandScenarioData.count(); k++)
		tree->setExpanded(model->index(k,0), true);
//show the view
	tree->show();
}

//save the current scenario as new one in the tree view
void Demand::saveCurrentScenarioAs()
{
//	QList <QString> listDemands;
//	QPair<QString, QVector<float> > demandProfileData;

//	listDemands.clear();
//	int rowCount = 0;
	QString demands = "";
//	rowCount = dsSelected->listSelectedDemandWidget->count();

/////////////////////////////////////
	m_DmdScnNo++;
	//clear the list of demand prfoile;
	demandProfileList.clear();
	//initialise the data required for a specific demand profile
	DemandData demandProfileData; //represent one specific demand profile

	for (int j = 0; j < selectedDemandList.count(); j++)
	{
		demandProfileData.setDemandName(selectedDemandList[j].first);
		demandProfileData.setStartDay(selDemandStartDayList[j].second);
		demandProfileData.setEndDay(selDemandStartDayList[j].second);
		demandProfileData.setHourSteps(selDemandHourStepsList[j].second);
		demandProfileData.m_pDemandDataVector = selectedDemandList[j].second;

		//push the demand profile to a list
		demandProfileList.append(demandProfileData);
	}
//make pair with the demand scenario name and push back to a demand scenario list
	demandScenarioPortfolio.append(qMakePair(QString("ds %0").arg(m_DmdScnNo), demandProfileList));
//put the reference id for the demand scenario into a list
	demandScenarioName.append(QString("ds %0").arg(m_DmdScnNo));
///////////////////////////////////
//prpeare the list for the updated profiles 	
	m_DemandScenarioData.clear();
	QString tempString = "";
	for (int k= 0; k < demandScenarioPortfolio.count(); k++)
	{
		m_DemandScenarioData.append(demandScenarioPortfolio[k].first);
		for (int m = 0; m < demandScenarioPortfolio[k].second.count(); m++)
		{
		   tempString = (demandScenarioPortfolio[k].second[m].demandName());
		   if (tempString == "")
			   tempString = "no profile has been selected yet.";
		   tempString.prepend(" ");
		   m_DemandScenarioData.append(tempString);
		}
		mapDS[demandScenarioPortfolio[k].first] = demandScenarioName[k]; //make a map for later when user change the name of demand scenario
	}
//////////////////////////////////
//emit a signal for demand scenario for the match to invoke the slot
	emit valueChanged(mapDS);
	emit valueChangedDSP(demandScenarioPortfolio);
//update the model with the new data
    model = new DemandScenarioTreeModel(m_DemandScenarioData);

//setup the show settings for the tree view
	tree->setSelectionMode(QAbstractItemView::SingleSelection);
	tree->header()->setClickable(true);

//set up the model
	tree->setModel(model);

//expand the tree after the model has been set up
	for (int k=0; k<m_DemandScenarioData.count(); k++)
		tree->setExpanded(model->index(k,0), true);

//show the tree view in the interface
	tree->show();
}

/*
QModelIndex Demand::ItemName()
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
void Demand::populateProfiles()
{
	QStringList temp;
	QString demands = "";

	int position = 0;
	int next_position = 0;

	if (tree->selectionModel()->hasSelection())
	{
//if the current index is the parent item
		if (model->hasChildren(tree->currentIndex()))
		{
			demands = model->data(tree->currentIndex(), Qt::DisplayRole).toString();

			for (int j=0; j <m_DemandScenarioData.count(); j++)
			{
				if (demands == m_DemandScenarioData[j])
				{
					position = j;
					if ((position+1) <= m_DemandScenarioData.count())
					{
						for (int k = position+1; k < m_DemandScenarioData.count();k++)
						{
							if(m_DemandScenarioData[k].startsWith(" "))
								next_position = k+1;
							else
								break;
						}
					}
					temp = m_DemandScenarioData.mid(position, next_position);
					break;
				}
			}

		}
	}

	//add the list the RightListWidget for the selected demands
	dsView->listSelectedDemandWidget->clear();

	for (int i=1; i < temp.size(); i++)
	{
		if (!temp[i].contains("profile has"))
		{
			temp[i].remove(0,1);
			dsView->listSelectedDemandWidget-> insertItem(i, temp[i]);
		}
	}

}
*/

void Demand::LocateItem(const QPoint &pos)
{

	if(dsView->stdProfilesTable->isItemSelected(dsView->stdProfilesTable->itemAt(pos)))
	{	
		setItemSelected(dsView->stdProfilesTable->itemAt(pos));
		QMenu* contextMenu = new QMenu(this);
		Q_CHECK_PTR (contextMenu);
		if (m_DmdScnNo == 0)
		{
			contextMenu->addAction("Graph", this, SLOT(drawDemandGraph()));
			contextMenu->addAction("Info", this, SLOT(showDemandInfo()));
			contextMenu->addAction("Properties", this, SLOT(demandProperty())); //include the start time, end time and number of timesteps between
		}
		else
		{
			contextMenu->addAction("Graph", this, SLOT(drawDemandGraph()));
			contextMenu->addAction("Info", this, SLOT(showDemandInfo()));
			contextMenu->addAction("Properties", this, SLOT(demandProperty())); //include the start time, end time and number of timesteps between
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

void Demand::showDemandInfo()
{
//	 QMessageBox msgBox;
	 QTableWidgetItem *selectedTableItem;
	 QString textDemand = " ";
//	 QModelIndex indexDemand;

	 selectedTableItem = getItemSelected();

	 dsView->stdProfilesTable->setCurrentItem(selectedTableItem);
	 textDemand = selectedTableItem->text();

//generate an information message box to show the demand info
	 QMessageBox::information( this, "Demand information", 
		QString("The selected demand is %1.").arg(textDemand) );

}

void Demand::setItemSelected(QTableWidgetItem * temp)
{
	m_ItemSelected = temp;
}

QTableWidgetItem * Demand::getItemSelected()
{
	return m_ItemSelected;
}

/*
void Demand::showDemandInfo(QTableWidgetItem* item)
{
	 QMessageBox msgBox;
	 msgBox.setText("The document has been modified.");
	 msgBox.exec();
}
*/

void Demand::moveMiddle()
{
  QStringListModel modelDemand;
  QStringList strings;
  QTableWidgetItem *itemName = dsView->stdProfilesTable->item(dsView->stdProfilesTable->currentRow(),0);
  QPair<QString, QVector <float> > tempData;
  tempData.first = "";
  tempData.second.clear();
  tempData.second.reserve(8760);
  tempData.second.resize(8760);

  retrieveDemandData(itemName->text());
//to check if the item to be added is contained within the list process demand widget
  if (dsProcess->listProcessDemandWidget->findItems(itemName->text(), Qt::MatchExactly).count()!= 0)
  { 
    bool ok;
	//introduce a new InputDialog to ask user to input the name for the new item
	QString text = QInputDialog::getText(this,tr("Name already exist"),tr("Please enter a new name:"),QLineEdit::Normal,tr(" "),&ok);
	//if user press OK and text field is not empty, take the new name in the line edit field and insert the item to the end of the list
	if( ok && !text.isEmpty() )
	{
//	  strings.append(itemName->text());
//	  retrieve data from the database

	  processDemandList.append(qMakePair(text, demandDataVector));

	  prcDemandStartDayList.append(qMakePair(text, getStartDay(itemName->text())));
	  prcDemandEndDayList.append(qMakePair(text, getEndDay(itemName->text())));
	  prcDemandHourStepsList.append(qMakePair(text, getHourSteps(itemName->text())));

//	  modelDemand.setStringList(strings);
//	  for (int i =0; i< strings.count(); i++)
	  dsProcess->listProcessDemandWidget-> insertItem(0, text);
	}
  }
//if it is not in the list, append it to the end of the list
  else
  {
//	  strings.append(itemName->text());
	  processDemandList.append(qMakePair(itemName->text(), demandDataVector));

	  prcDemandStartDayList.append(qMakePair(itemName->text(), getStartDay(itemName->text())));
	  prcDemandEndDayList.append(qMakePair(itemName->text(), getEndDay(itemName->text())));
	  prcDemandHourStepsList.append(qMakePair(itemName->text(), getHourSteps(itemName->text())));
//	  modelDemand.setStringList( strings);
//	  for (int i =0; i< strings.count(); i++)
	  dsProcess->listProcessDemandWidget-> insertItem(0, itemName->text());
  }

}

void Demand::moveRight()
{
  QStringListModel selectedDemand;
  QStringList strings;
 
  QPair<QString, QVector <float> > tempData;
  tempData.first = "";
  tempData.second.clear();
  tempData.second.reserve(8760);
  tempData.second.resize(8760);
	

//to check if the item to be added is contained within the list process demand widget
//  if (listSelectedDemandWidget->findItems(itemName->text(), Qt::MatchExactly).count()!= 0)
  if (dsSelected->listSelectedDemandWidget->findItems(m_ItemName, Qt::MatchExactly).count()!= 0)
  { 
    bool ok;
	//introduce a new InputDialog to ask user to input the name for the new item
	QString text = QInputDialog::getText(this,tr("Name already exist"),tr("Please enter a new name:"),QLineEdit::Normal,tr(" "),&ok);
	//if user press OK and text field is not empty, take the new name in the line edit field and insert the item to the end of the list
	if( ok && !text.isEmpty() )
	{
		//check if the item in the selected demand list is from the standard table widget 
		// or the item is from the middle list widget as the same item inside the standard table widget 
		  if (dsView->stdProfilesTable->findItems(m_ItemName, Qt::MatchExactly).count()!= 0)
		  {
			  retrieveDemandData(m_ItemName);
			  selectedDemandList.append(qMakePair(text, demandDataVector));

			  selDemandStartDayList.append(qMakePair(text, getStartDay(m_ItemName)));
			  selDemandEndDayList.append(qMakePair(text, getEndDay(m_ItemName)));
			  selDemandHourStepsList.append(qMakePair(text, getHourSteps(m_ItemName)));
		  }
		//if the item is from the middle list widget and not in the standard table widget
		  else
		  {
			  for (int i = 0; i < processDemandList.count(); i++)
			  {
				  if (m_ItemName == processDemandList[i].first)
				  {
					  selectedDemandList.append(qMakePair(text, processDemandList[i].second));

			          selDemandStartDayList.append(qMakePair(text, getStartDay(m_ItemName)));
			          selDemandEndDayList.append(qMakePair(text, getEndDay(m_ItemName)));
					  selDemandHourStepsList.append(qMakePair(text, getHourSteps(m_ItemName)));
				  }
			  }
		  }
	//	  strings.append(itemName->text());
		  strings.append(m_ItemName);
		  selectedDemand.setStringList(strings);
		  for (int i =0; i< strings.count(); i++)
			  dsSelected->listSelectedDemandWidget-> insertItem(i, text);
	}
  }
//if it is not in the list, append it to the end of the list
  else
  {
//	  strings.append(itemName->text());
		//check if the item in the selected demand list is from the standard table widget 
		// or the item is from the middle list widget as the same item inside the standard table widget 
		  if (dsView->stdProfilesTable->findItems(m_ItemName, Qt::MatchExactly).count()!= 0)
		  {
			  retrieveDemandData(m_ItemName);
			  selectedDemandList.append(qMakePair(m_ItemName, demandDataVector));

	          selDemandStartDayList.append(qMakePair(m_ItemName, getStartDay(m_ItemName)));
	          selDemandEndDayList.append(qMakePair(m_ItemName, getEndDay(m_ItemName)));
			  selDemandHourStepsList.append(qMakePair(m_ItemName, getHourSteps(m_ItemName)));
		  }
		//if the item is from the middle list widget and not in the standard table widget
		  else
		  {
			  for (int i = 0; i < processDemandList.count(); i++)
			  {
				  if (m_ItemName == processDemandList[i].first)
				  {
					  selectedDemandList.append(processDemandList[i]);

					  selDemandStartDayList.append(qMakePair(m_ItemName, getStartDay(m_ItemName)));
					  selDemandEndDayList.append(qMakePair(m_ItemName, getEndDay(m_ItemName)));
					  selDemandHourStepsList.append(qMakePair(m_ItemName, getHourSteps(m_ItemName)));
				  }
			  }
		  }
		  strings.append(m_ItemName);
		  selectedDemand.setStringList(strings);
		  for (int i =0; i< strings.count(); i++)
			  dsSelected->listSelectedDemandWidget-> insertItem(i, strings[i]);
  }
}

//Slot for the process list
void Demand::locateProcessListItem(const QPoint &pos)
{
//to create pop-up menu for the process demand list
	if(dsProcess->listProcessDemandWidget->isItemSelected(dsProcess->listProcessDemandWidget->itemAt(pos)))
	{
		setListItemSelected(dsProcess->listProcessDemandWidget->itemAt(pos));
		QMenu* contextMenu = new QMenu(this);
		Q_CHECK_PTR (contextMenu);

		contextMenu->addAction("Graph", this, SLOT(drawSingleMiddleDemandGraph()));
		contextMenu->addAction("Info", this, SLOT(showMiddleDemandInfo()));
		contextMenu->addAction("Properties", this, SLOT(procDemandProperty())); //include the start time, end time and number of timesteps between
		contextMenu->addSeparator();
		contextMenu->addAction("Select final", this, SLOT(moveRight()));
		contextMenu->addAction("Delete", this, SLOT(removeMiddle()));

		contextMenu->popup(QCursor::pos());
		contextMenu->exec();
		delete contextMenu;
		contextMenu = 0;
	}
//to create pop-up menu for the selected demand list

}

void Demand::showMiddleDemandInfo()
{
//	 QMessageBox msgBox;
	 QListWidgetItem *selectedListItem;
	 QString textDemand = " ";
//	 QModelIndex indexDemand;

	 selectedListItem = getListItemSelected();

	 dsProcess->listProcessDemandWidget->setCurrentItem(selectedListItem);
	 textDemand = selectedListItem->text();

//generate an information message box to show the demand info
	 QMessageBox::information( this, "Demand information", 
		QString("The selected demand is %1.").arg(textDemand) );

}
//for selected demands
void Demand::setListItemSelected(QListWidgetItem * temp)
{
	m_ListItemSelected = temp;
}

QListWidgetItem * Demand::getListItemSelected()
{
	return m_ListItemSelected;
}

//for process demands
void Demand::setProcessListItems(QListWidgetItem * temp)
{
	m_ProcessListItems = temp;
}

QListWidgetItem * Demand::getProcessListItems()
{
	return m_ProcessListItems;
}

//Slot for selected list
void Demand::locateSelectedListItem(const QPoint &pos)
{
//to create pop-up menu for the selected demand list
	if(dsSelected->listSelectedDemandWidget->isItemSelected(dsSelected->listSelectedDemandWidget->itemAt(pos)))
	{
		setListItemSelected(dsSelected->listSelectedDemandWidget->itemAt(pos));

		QMenu* contextMenu = new QMenu(this);
		Q_CHECK_PTR (contextMenu);

		contextMenu->addAction("Graph", this, SLOT(drawSingleRightDemandGraph()));
		contextMenu->addAction("Info", this, SLOT(showRightDemandInfo()));
		contextMenu->addAction("Properties", this, SLOT(selDemandProperty())); //include the start time, end time and number of timesteps between
		contextMenu->addSeparator();
		contextMenu->addAction("Delete", this, SLOT(removeRight()));

		contextMenu->popup(QCursor::pos());
		contextMenu->exec();
		delete contextMenu;
		contextMenu = 0;
	}
}

void Demand::showRightDemandInfo()
{
//	 QMessageBox msgBox;
	 QListWidgetItem *selectedListItem;
	 QString textDemand = " ";
//	 QModelIndex indexDemand;

	 selectedListItem = getListItemSelected();

	 dsSelected->listSelectedDemandWidget->setCurrentItem(selectedListItem);
	 textDemand = selectedListItem->text();

//generate an information message box to show the demand info
	 QMessageBox::information( this, "Demand information", 
		QString("The selected demand is %1.").arg(textDemand) );

}

void Demand::drawSingleRightDemandGraph()
{
	QListWidgetItem * itemTemp;
	itemTemp = getListItemSelected();

//clear the plotted item in the graph
	demandPlot->detachItems(QwtPlotItem::Rtti_PlotCurve, true);

	for (int i=0; i<selectedDemandList.count(); i++)
	{
		if(itemTemp->text() == selectedDemandList[i].first)
		{
	 		demandPlot->drawDemandGraph(itemTemp->text(), selectedDemandList[i].second);//item->text(),demandDataVector);
		}
	}
}


void Demand::drawRightDemandGraph()
{
	QListWidgetItem * itemTemp;
	itemTemp = getListItemSelected();

//	retrieveDemandData(itemTemp->text());

	QVector <QVector <float> > DemandSelectedVector;

	ItemSelected = dsSelected->listSelectedDemandWidget->selectedItems();
//Allocate memory for selected demand vector (2 dimensions)	
	DemandSelectedVector.clear();
	DemandSelectedVector.reserve(ItemSelected.count());
	DemandSelectedVector.resize(ItemSelected.count());

	ItemNameVector.clear();
	ItemNameVector.reserve(ItemSelected.count());
	ItemNameVector.resize(ItemSelected.count());

	for (int j=0; j < DemandSelectedVector.size(); j++)
	{
		DemandSelectedVector[j].clear();
		DemandSelectedVector[j].reserve(8760);
		DemandSelectedVector[j].resize(8760);
	}
	int count = 0;
	count = dsSelected->selectedDemandList.count();

	for (int i = 0; i< ItemSelected.count(); i++)
	{
		ItemNameVector[i] = ItemSelected[i]->text();
//check if the name selected is matched with which item in the selecteddemandlist
		DemandSelectedVector[i] = dsSelected->selectedDemandList[i].second;
	}

	demandPlot->drawMultipleDemandsGraph(ItemNameVector,DemandSelectedVector);

}

void Demand::setTableItemName(QTableWidgetItem *Item)
{
	m_ItemName = Item->text();
}

void Demand::setListItemName(QListWidgetItem *Item)
{
	m_ItemName = Item->text();
}


void Demand::removeMiddle()
{
	QString temp = "";
//	listProcessSupplyWidget->takeItem(listProcessSupplyWidget->currentRow());
	switch( QMessageBox::question(this, tr("Remove item"), tr("Are you sure to delete the item?"),
		    tr("&Yes"), tr("&No"), QString(), 0, 1))
	{
		case 0:
			//identify the item in the process list and delete the relevant info 
			//(i.e. start day, end day and hour steps) associated with the item 
				temp = dsProcess->listProcessDemandWidget->currentItem()->text();
				for(int i=0; i<prcDemandStartDayList.count(); i++)
				{
					if(temp == prcDemandStartDayList[i].first)
					{
						prcDemandStartDayList.takeAt(i);
						prcDemandEndDayList.takeAt(i);
						prcDemandHourStepsList.takeAt(i);
					}
				}
//delete the item in the process list widget
				dsProcess->listProcessDemandWidget->takeItem(dsProcess->listProcessDemandWidget->currentRow());
		break;

		case 1:
			return;
		break;

		default:
			return;
		break;
	}
}

void Demand::removeRight()
{
	QString temp = "";
//  listSelectedSupplyWidget->takeItem(listSelectedSupplyWidget->currentRow());
	switch( QMessageBox::question(this, tr("Remove item"), tr("Are you sure to delete the item?"),
		    tr("&Yes"), tr("&No"), QString(), 0, 1))
	{
		case 0:
			//identify the item in the selected list and delete the relevant info 
			//(i.e. start day, end day and hour steps) associated with the item 
				temp = dsSelected->listSelectedDemandWidget->currentItem()->text();
				for(int i=0; i<selDemandStartDayList.count(); i++)
				{
					if(temp == selDemandStartDayList[i].first)
					{
						selDemandStartDayList.takeAt(i);
						selDemandEndDayList.takeAt(i);
						selDemandHourStepsList.takeAt(i);

						selectedDemandList.takeAt(i);
					}
				}
//delete the item in the selected list widget
				dsSelected->listSelectedDemandWidget->takeItem(dsSelected->listSelectedDemandWidget->currentRow());
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
void Demand::scaleDemandProfile()
{
  float sfactor = 1.0;
  QString  nameProfile = "";
  QString text = "";
  QVector<float> tempProfileVector;
  
  if( !dsProcess->listProcessDemandWidget->currentItem() )
    return;
  
   nameProfile = dsProcess->listProcessDemandWidget->currentItem()->text();
  
  ScaleDialog dlg( this );
//  sfactor = dlg.scaleFactor().toFloat();
  //found out which profile has been selected to scale down/up

  if( dlg.exec() == QDialog::Accepted )
  {
	  sfactor = dlg.scaleFactor().toFloat();

//apply the scaling factor to the demand   
	  for (int i = 0; i < processDemandList.count(); i++)
	  {
		  if (nameProfile == processDemandList[i].first)
		  {

			  for (int j=0; j < processDemandList[i].second.count(); j++)
			  {
				  if (j == 0)
				  {
					tempProfileVector.clear();
					tempProfileVector.reserve(processDemandList[i].second.count());
					tempProfileVector.resize(processDemandList[i].second.count());
				  }

				  tempProfileVector[j] = sfactor * processDemandList[i].second[j];
			  }
			  text = nameProfile+ "_" + dlg.scaleFactor() + "scaled";
			  processDemandList[i].first = text;
			  processDemandList[i].second = tempProfileVector;
           
//update the item name in the start day list, end day list and hour step list
			  prcDemandStartDayList[i].first = text;
			  prcDemandEndDayList[i].first = text;
              prcDemandHourStepsList[i].first = text;

			  dsProcess->listProcessDemandWidget->currentItem()->setText(text);
		  }
	  }
  }
}

//shift demand profile
void Demand::shiftDemandProfile()
{
  int shift_steps = 0;
  int p = 0;
  int timesteps = 0;

  QString  nameProfile = "";
  QString text = "";
  QVector<float> tempProfileVector;
  
  if( !dsProcess->listProcessDemandWidget->currentItem() )
    return;
  
   nameProfile = dsProcess->listProcessDemandWidget->currentItem()->text();
  
  ShiftDialog dlg( this );

  if( dlg.exec() == QDialog::Accepted )
  {
	  shift_steps = dlg.numberOfSteps().toInt();
	  for (int i = 0; i < processDemandList.count(); i++)
	  {
		  if (nameProfile == processDemandList[i].first)
		  {
			  if (dlg.forwardDirection() && !dlg.backwardDirection()) //shift forward
			  {	
				p = 0;
				timesteps = processDemandList[i].second.count();
				for (int j=0; j < processDemandList[i].second.count(); j++)
				{
					if (j == 0)
					{
						tempProfileVector.clear();
						tempProfileVector.reserve(processDemandList[i].second.count());
						tempProfileVector.resize(processDemandList[i].second.count());
					}

					if (p < shift_steps)
					{
						tempProfileVector[j] = processDemandList[i].second[timesteps-shift_steps+p];//assign the part of the demand outside boundary to demand the begining
						p++;
					}
					else
					{
						tempProfileVector[j] = processDemandList[i].second[j-shift_steps];
					}
				}
			  }
			  else //shift backward
			  {
				p = 0;
				timesteps = processDemandList[i].second.count();
				for (int j=0; j < processDemandList[i].second.count(); j++)
				{
					if (j == 0)
					{
						tempProfileVector.clear();
						tempProfileVector.reserve(processDemandList[i].second.count());
						tempProfileVector.resize(processDemandList[i].second.count());
					}

					if (p < (timesteps-shift_steps))
					{
						tempProfileVector[j] = processDemandList[i].second[j+shift_steps];
						p++;
					}
					else
					{	
						tempProfileVector[j] = processDemandList[i].second[j-p];//assign the part of the demand outside boundary to demand the begining
					}
				}
			  }
			  text = nameProfile+ "_"  + "shifted";
			  processDemandList[i].first = text;
			  processDemandList[i].second = tempProfileVector;
//update the item name in the start day list, end day list and hour step list
			  prcDemandStartDayList[i].first = text;
			  prcDemandEndDayList[i].first = text;
              prcDemandHourStepsList[i].first = text;

			  dsProcess->listProcessDemandWidget->currentItem()->setText(text);

		  }
		}
	}
}

void Demand::combineDemandProfile()
{
	QVector <QVector <float> > processDemandVector;
	processItems = dsProcess->listProcessDemandWidget->selectedItems();
//Allocate memory for selected demand vector (2 dimensions)	
	processDemandVector.clear();
	processDemandVector.reserve(processItems.count()+1);
	processDemandVector.resize(processItems.count()+1);

	processItemNameVector.clear();
	processItemNameVector.reserve(processItems.count()+1);
	processItemNameVector.resize(processItems.count()+1);

	for (int j=0; j < processDemandVector.size(); j++)
	{
		processDemandVector[j].clear();
		processDemandVector[j].reserve(8760);
		processDemandVector[j].resize(8760);
	}

	int count = 0;
	int ccomb = 0;
	count = processDemandList.count();
	ccomb = processItems.count()+1;

	for (int i = 0; i< processItems.count()+1; i++)
	{
		if (i < processItems.count())
		{
			processItemNameVector[i] = processItems[i]->text();
//check if the name selected is matched with which item in the selecteddemandlist
			for (int k=0; k<count; k++)
			{
				if (processItems[i]->text() == processDemandList[k].first)
	 				processDemandVector[i] = processDemandList[k].second;
			}
//			processDemandVector[i] = processDemandList[i].second;
			for (int j(processDemandVector[i].size() - 1); j >= 0; --j)
				processDemandVector[processItems.count()][j] += processDemandVector[i][j];
		}
		else
		{
			processItemNameVector[i] = "combined demand";
		}
	}

//add to the list
	  QStringList strings;
  QListWidgetItem *itemName = 
	  dsProcess->listProcessDemandWidget->item(dsProcess->listProcessDemandWidget->currentRow());

//to check if the item to be added is contained within the list process demand widget
  if (dsProcess->listProcessDemandWidget->findItems(processItemNameVector[ccomb-1], Qt::MatchExactly).count()!= 0)
  { 
    bool ok;
	//introduce a new InputDialog to ask user to input the name for the new item
	QString text = QInputDialog::getText(this,tr("Name already exist"),tr("Please enter a new name:"),QLineEdit::Normal,tr(" "),&ok);
	//if user press OK and text field is not empty, take the new name in the line edit field and insert the item to the end of the list
	if( ok && !text.isEmpty() )
	{
	  strings.append(text);
	  processDemandList.append(qMakePair(text, processDemandVector[ccomb-1]));
	  
	  prcDemandStartDayList.append(qMakePair(text, getStartDay(itemName->text())));
	  prcDemandEndDayList.append(qMakePair(text, getEndDay(itemName->text())));
	  prcDemandHourStepsList.append(qMakePair(text, getHourSteps(itemName->text())));

	  for (int i =0; i< strings.count(); i++)
		  dsProcess->listProcessDemandWidget-> insertItem(i, text);
	}
  }
//if it is not in the list, append it to the end of the list
  else
  {
	  strings.append(processItemNameVector[ccomb-1]);
	  processDemandList.append(qMakePair(processItemNameVector[ccomb-1], processDemandVector[ccomb-1]));

	  prcDemandStartDayList.append(qMakePair(processItemNameVector[ccomb-1], getStartDay(itemName->text())));
	  prcDemandEndDayList.append(qMakePair(processItemNameVector[ccomb-1], getEndDay(itemName->text())));
	  prcDemandHourStepsList.append(qMakePair(processItemNameVector[ccomb-1], getHourSteps(itemName->text())));

	  for (int i =0; i< strings.count(); i++)
		  dsProcess->listProcessDemandWidget-> insertItem(i, strings[i]);
  }
}


void Demand::clearDemandProfile()
{
	//	listProcessSupplyWidget->takeItem(listProcessSupplyWidget->currentRow());
	switch( QMessageBox::question(this, tr("Clear all"), tr("Are you sure to clear all the items?"),
		    tr("&Yes"), tr("&No"), QString(), 0, 1))
	{
		case 0:
			processDemandList.clear();
			dsProcess->listProcessDemandWidget->clear();
			prcDemandStartDayList.clear();
			prcDemandEndDayList.clear();
			prcDemandHourStepsList.clear();
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
void Demand::deleteCurrentScenario()
{
//check if the header or item in the tree view has been selected
	if (tree->selectionModel()->hasSelection())
	{

		switch( QMessageBox::question(this, tr("Delete demand scenario"), tr("Are you sure to delete the current demand scenario?"),
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

void Demand::removeCurrentScenario()
{
	QList <QString> listDemands;
	QStringList temp1;
	QStringList temp2;
	QPair<QString, QVector<float> > demandProfileData;
	listDemands.clear();
	int rowCount = 0; 
	int noScenario = 0;

	QString demands = "";

	if (model->hasChildren(tree->currentIndex()))
	{
		demands = model->data(tree->currentIndex(), Qt::DisplayRole).toString();
	}
	else
	{
		demands = model->data(model->parent(tree->currentIndex()), Qt::DisplayRole).toString();
	}

	//navigate the elements within the map of demand scenario using iternator
	QString tempDS = "";
	QMap<QString, QString>::const_iterator i = mapDS.constBegin();
	while (i != mapDS.constEnd()) 
	{
		if (i.value() == demands)
		{
			tempDS = i.key();
			break;
		}
		++i;
	}
	//set up the demand scenario using the data in the selected list
	for (int i=0; i < demandScenarioPortfolio.count(); i++)
	{
		if (tempDS == demandScenarioPortfolio[i].first)
		{
			demandScenarioPortfolio.takeAt(i);
			break;
		}
	}

//prpeare the list for the updated profiles 	
	m_DemandScenarioData.clear();
	mapDS.clear();
	QString tempString = "";
	for (int k= 0; k < demandScenarioPortfolio.count(); k++)
	{
		m_DemandScenarioData.append(demandScenarioPortfolio[k].first);
		for (int m = 0; m < demandScenarioPortfolio[k].second.count(); m++)
		{
		   tempString = (demandScenarioPortfolio[k].second[m].demandName());
		   if (tempString == "")
			   tempString = "no profile has been selected yet.";
		   tempString.prepend(" ");
		   m_DemandScenarioData.append(tempString);
		}
		mapDS[demandScenarioPortfolio[k].first] = demandScenarioName[k]; //make a map for later when user change the name of demand scenario
	}


//emit a signal for demand scenario for the match to invoke the slot
	emit valueChanged(mapDS);
	emit valueChangedDSP(demandScenarioPortfolio);

//assign the new data to the model
	model = new DemandScenarioTreeModel(m_DemandScenarioData);
//set the view settings for TreeView
	tree->setSelectionMode(QAbstractItemView::SingleSelection);
	tree->header()->setClickable(true);
//deploy the profile to the selected list widget
	connect(tree, SIGNAL(clicked()), this, SLOT(deployProfiles()));

//set up the model with the tree view
	tree->setModel(model);

//expand the tree after the model has been set up
	for (int k=0; k<m_DemandScenarioData.count(); k++)
		tree->setExpanded(model->index(k,0), true);
//show the view
	tree->show();

//reduce the number of scenario
	m_DmdScnNo--;	
}

void Demand::deployProfiles()
{
	QStringList temp;
	QString demands = "";
	QString tempString = "";

	int position = 0;
	int next_position = 0;

	if (tree->selectionModel()->hasSelection())
	{
//if the current index is the parent item
		if (model->hasChildren(tree->currentIndex()))
		{
			demands = model->data(tree->currentIndex(), Qt::DisplayRole).toString();

			//navigate the elements within the map of demand scenario using iternator
			QString tempDS = "";
			QMap<QString, QString>::const_iterator i = mapDS.constBegin();
			while (i != mapDS.constEnd()) 
			{
				if (i.value() == demands)
				{
					tempDS = i.key();
					break;
				}
				++i;
			}

			//set up the demand scenario using the data in the selected list
			for (int i=0; i < demandScenarioPortfolio.count(); i++)
			{
				if (tempDS == demandScenarioPortfolio[i].first)
				{
					//add the list the RightListWidget for the selected demands
					dsSelected->listSelectedDemandWidget->clear();
					//insert the item to the selected list widget
					for (int j = 0; j < demandScenarioPortfolio[i].second.count(); j++)
					{
					   tempString = (demandScenarioPortfolio[i].second[j].demandName());
					   dsSelected->listSelectedDemandWidget-> insertItem(j, (demandScenarioPortfolio[i].second[j].demandName()));
					}
					break;
				}
			}
		}
	}
}

//slot to set the propeties for the available profiles
void Demand::demandProperty()
{
 int flag = 0;
//check if there is any item has been selected in the standard demand profile table widget
  if( !dsView->stdProfilesTable->currentItem() )
    return;

  retrieveDemandData(dsView->stdProfilesTable->currentItem()->text());

  QString temp = dsView->stdProfilesTable->currentItem()->text();

  DemandPropertyDialog dlg( this );

  for (int i=0;i<demandStartDayList.count(); i++)
  {
	  if (temp == demandStartDayList[i].first)
	  {
		dlg.setStartDay(start_day_list, demandStartDayList[i].second);
		dlg.setEndDay(end_day_list, demandEndDayList[i].second);
		dlg.setHourSteps(hour_steps_list, demandHourStepsList[i].second);
		flag = i;
		break;
	  }
  }

  if( dlg.exec() == QDialog::Accepted )
  {
	demandStartDayList[flag].second = dlg.startDay();
	demandEndDayList[flag].second = dlg.endDay();
	demandHourStepsList[flag].second = dlg.hourSteps();
  }
}

//slot to set the propeties for the process profiles
void Demand::procDemandProperty()
{
 int flag = 0;
//check if there is any item has been selected in the standard demand profile table widget
  if( !dsProcess->listProcessDemandWidget->currentItem() )
    return;

  QString temp = dsProcess->listProcessDemandWidget->currentItem()->text();

  DemandPropertyDialog dlg( this );

  for (int i=0;i<prcDemandStartDayList.count(); i++)
  {
	  if (temp == prcDemandStartDayList[i].first)
	  {
		dlg.setStartDay(start_day_list, prcDemandStartDayList[i].second);
		dlg.setEndDay(end_day_list, prcDemandEndDayList[i].second);
		dlg.setHourSteps(hour_steps_list, prcDemandHourStepsList[i].second);
		flag = i;
		break;
	  }
  }

  if( dlg.exec() == QDialog::Accepted )
  {
	prcDemandStartDayList[flag].second = dlg.startDay();
	prcDemandEndDayList[flag].second = dlg.endDay();
	prcDemandHourStepsList[flag].second = dlg.hourSteps();
  }
}

//slot to set the propeties for the selected profiles
void Demand::selDemandProperty()
{
 int flag = 0;
//check if there is any item has been selected in the standard demand profile table widget
  if( !dsSelected->listSelectedDemandWidget->currentItem() )
    return;

  QString temp = dsSelected->listSelectedDemandWidget->currentItem()->text();

//show the property dialog
  DemandPropertyDialog dlg( this );

  for (int i=0;i<selDemandStartDayList.count(); i++)
  {
	  if (temp == selDemandStartDayList[i].first)
	  {
		dlg.setStartDay(start_day_list, selDemandStartDayList[i].second);
		dlg.setEndDay(end_day_list, selDemandEndDayList[i].second);
		dlg.setHourSteps(hour_steps_list, selDemandHourStepsList[i].second);
		flag = i;
		break;
	  }
  }

  if( dlg.exec() == QDialog::Accepted )
  {
	selDemandStartDayList[flag].second = dlg.startDay();
	selDemandEndDayList[flag].second = dlg.endDay();
	selDemandHourStepsList[flag].second = dlg.hourSteps();
  }
}

//convert to julian day from day and month
/* Private Member Function: Converts calender date to julian day.
   Returns julian day for any given day of month, and accounts for leap years(?).*/   
int Demand::convertToJulian(int Day,int Month)
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

//supply the data for the three comboboxes in the demand property dialog
void Demand::setTemporalParameterList()
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

  QStringList start_date = demandDateDataVector[0].split(" ");
  QStringList end_date = demandDateDataVector[8759].split(" ");

  QStringList start_date_parts = start_date[0].split("-");
  QStringList end_date_parts = end_date[0].split("-");

  start_day = (start_date_parts[2].trimmed()).toInt();
  end_day = (end_date_parts[2].trimmed()).toInt();

  start_month = (start_date_parts[1].trimmed()).toInt();
  end_month = (end_date_parts[1].trimmed()).toInt();

  julian_start_day = convertToJulian(start_day, start_month);
  julian_end_day = convertToJulian(end_day, end_month);

  numberOfDataPoints = demandDateDataVector.count();

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
int Demand::getStartDay(const QString& itemName)
{
	for(int i=0; i<demandStartDayList.count(); i++)
	{
		if(itemName == demandStartDayList[i].first)
		{
			return demandStartDayList[i].second;
		}
	}
	return 0;
}

int Demand::getEndDay(const QString& itemName)
{
	for(int i=0; i<demandEndDayList.count(); i++)
	{
		if(itemName == demandEndDayList[i].first)
		{
			return demandEndDayList[i].second;
		}
	}
	return 0;
}

// get the number steps in an hour
int Demand::getHourSteps(const QString& itemName)
{
	for(int i=0; i<demandHourStepsList.count(); i++)
	{
		if(itemName == demandHourStepsList[i].first)
		{
			return demandHourStepsList[i].second;
		}
	}
	return 0;
}
