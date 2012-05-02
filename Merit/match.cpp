#include <QtGui>

#include "match.h"

Match::Match(QWidget *parent)
    : QWidget(parent)
{
//initialisations
	timestep2 = 0;
	m_bDrawProfileView = false;
	profileDrawTime = 0;

	m_iDsValue = 0;
	m_iSsValue = 0;

	pMatchIdVector.clear();
	pMatchRateVector.clear();
	pCorCoefVector.clear();
	MatchResultVector.clear();

//setup the user interface 
    setupUi(this);

	splitterVertical->setStretchFactor(0,5);
	splitterVertical->setStretchFactor(1,1);

	gridLayout_scenario = new QGridLayout(scenarioGroupBox);
    gridLayout_scenario->setObjectName(QString::fromUtf8("gridLayout_scenario"));

    scnMatchView = new DrawMatchView(scenarioGroupBox);
    scnMatchView->setObjectName(QString::fromUtf8("scnMatchView"));
	gridLayout_scenario->addWidget(scnMatchView);

	//save the current selected demand/supply scenario
	connect(this->scnMatchView, SIGNAL(dsValueChanged(int)),this, SLOT(setDsNumValue(int)));
	connect(this->scnMatchView, SIGNAL(ssValueChanged(int)),this, SLOT(setSsNumValue(int)));

	connect(this->scnMatchView, SIGNAL(emitProfileDrawSignal()), this, SLOT(drawProfileView()));	
    connect( &timer, SIGNAL( timeout() ), this, SLOT( slotTimeout() ) );

}

void Match::drawProfileView()
{
//set up the model with no data
	model = new QStandardItemModel();
//headers for the table view
	model->setHorizontalHeaderItem( 0, new QStandardItem( "Reference" ) );
    model->setHorizontalHeaderItem( 1, new QStandardItem( "MI (%)" ) );
	model->setHorizontalHeaderItem( 2, new QStandardItem( "( IC" ) );
    model->setHorizontalHeaderItem( 3, new QStandardItem( "CC )" ) );
	model->setHorizontalHeaderItem( 4, new QStandardItem( "ED (kWh)" ) );
    model->setHorizontalHeaderItem( 5, new QStandardItem( "ES (kWh)" ) );
	model->setHorizontalHeaderItem( 6, new QStandardItem( "TD (kWh)" ) );
    model->setHorizontalHeaderItem( 7, new QStandardItem( "TS (kWh)" ) );

//only add the following interface when it is the first time, to make it dynamic effect  
	if (profileDrawTime == 0)
	{
//a new group box for profile match view
        widgetProfile = new QWidget(splitter);
        widgetProfile->setObjectName(QString::fromUtf8("widgetProfile"));

        horizontalLayoutProfile = new QHBoxLayout(widgetProfile);
        horizontalLayoutProfile->setObjectName(QString::fromUtf8("horizontalLayoutProfile"));
        horizontalLayoutProfile->setContentsMargins(0, 0, 0, 0);
        profileGroupBox = new QGroupBox("Selected scenario");
        profileGroupBox->setObjectName(QString::fromUtf8("profileGroupBox"));

//create a progress bar
		matchProgressBar = new QProgressBar;
		matchProgressBar->setObjectName(QString::fromUtf8("matchProgressBar"));

		QSizePolicy progressBarSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		progressBarSizePolicy.setHorizontalStretch(1);
		progressBarSizePolicy.setVerticalStretch(0);

		matchProgressBar->setSizePolicy(progressBarSizePolicy);
		matchProgressBar->setFixedHeight(10);
//red background color when the progress bar is progressing
		matchProgressBar->setStyleSheet("QProgressBar::chunk {background-color: red;width: 10px; margin: 0.5px;} QProgressBar { text-align: right;}");

		horizontalLayout = new QHBoxLayout;

//Auto button and its size policy
		searchButton = new QPushButton("Auto");
		searchButton->setObjectName(QString::fromUtf8("searchButton"));
		QSizePolicy	searchButtonSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		searchButtonSizePolicy.setHorizontalStretch(1);
		searchButtonSizePolicy.setVerticalStretch(0);
		searchButtonSizePolicy.setHeightForWidth(searchButton->sizePolicy().hasHeightForWidth());
		searchButton->setSizePolicy(searchButtonSizePolicy);

//Stop button and its size policy
		abortButton = new QPushButton("Stop");
		abortButton->setObjectName(QString::fromUtf8("abortButton"));
		QSizePolicy	stopButtonSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		stopButtonSizePolicy.setHorizontalStretch(1);
		stopButtonSizePolicy.setVerticalStretch(0);
		stopButtonSizePolicy.setHeightForWidth(abortButton->sizePolicy().hasHeightForWidth());
		abortButton->setSizePolicy(stopButtonSizePolicy);

		horizontalSpacer_1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

//put the buttons and spaces into a horizontal layout
		horizontalLayout->addWidget(searchButton);
		horizontalLayout->addItem(horizontalSpacer_1);
		horizontalLayout->addWidget(abortButton);

//organise the profile match view, match progress bar and the aboved horizontal layout into a vertical layout
		verticalLayout = new QVBoxLayout(profileGroupBox);
	
		profileMatchView = new ProfileMatchView;
		profileMatchView->drawDemandProfileButton(dsNumValue(), m_pTemp);
		profileMatchView->drawSupplyProfileButton(ssNumValue(), m_pTempSupply);

		verticalLayout->addWidget(profileMatchView);
		verticalLayout->addWidget(matchProgressBar);
		verticalLayout->addLayout(horizontalLayout);		

//apply the grid layout to the group box of profile match
		gridLayout_profile = new QGridLayout(profileGroupBox);
		gridLayout_profile->setObjectName(QString::fromUtf8("gridLayout_profile"));
		gridLayout_profile->addLayout(verticalLayout,0, 0, 1, 1);

////////////////////////////////////////////////////
        horizontalLayoutProfile->addWidget(profileGroupBox);

//implement a match accelerate button to carry out the mannual match for demand/supply combination
        matchButton = new QPushButton(">>");
        matchButton->setObjectName(QString::fromUtf8("matchButton"));
//set the fix size of the match accelerate buttton
		matchButton->setFixedSize(40,23);

        horizontalLayoutProfile->addWidget(matchButton);

        splitter->addWidget(widgetProfile);

//create a new group box for match table result
        matchResultGroupBox = new QGroupBox("Statistics");
        matchResultGroupBox->setObjectName(QString::fromUtf8("matchResultGroupBox"));
        matchResultGridLayout = new QGridLayout(matchResultGroupBox);
        matchResultGridLayout->setObjectName(QString::fromUtf8("matchResultGridLayout"));

//within the statistics groupbox, add a table view to display the match statistics
        matchResultsTable = new MatchResultView();
        matchResultsTable->setObjectName(QString::fromUtf8("matchResultsTable"));
        matchResultGridLayout->addWidget(matchResultsTable, 0, 0, 1, 1);

        splitter->addWidget(matchResultGroupBox);
//assign demand and supply vector
		profileMatchView->setProfileValue(m_pTemp);
		profileMatchView->setSupplyProfileValue(m_pTempSupply);

		profileDrawTime++;
	}
	else
	{
		profileMatchView->drawDemandProfileButton(dsNumValue(), m_pTemp);
		profileMatchView->drawSupplyProfileButton(ssNumValue(), m_pTempSupply);
	}

//Wrap the model within a sorter (QSortFilterProxyModel)
	sorter = new SortOnSecondModel();
	sorter->setSourceModel(model);
	sorter->setDynamicSortFilter(true); //enable dynamic sorting when the sort is enabled
	matchResultsTable->setModel(sorter);
//set the match result table view can only be selected by row  
	matchResultsTable->setSelectionBehavior( QAbstractItemView::SelectRows );
//resize the column size to the contents of table view 
	matchResultsTable->resizeColumnsToContents(); 
//show the table
	matchResultsTable->show();

//create the progress bar with the inital parameters
	createProgressBar();

//Signals and Slots
//enable full search by clicking the auto button 
	connect(searchButton, SIGNAL(clicked()), this->profileMatchView, SLOT(doFullSearch()));
	connect(matchResultsTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(deployMatchResults(QModelIndex)));

    // connect the clicked() SIGNALs of the pushbuttons to SLOTs
    connect( searchButton, SIGNAL( clicked() ), this, SLOT( slotStart() ) );
    connect( abortButton, SIGNAL( clicked() ), this, SLOT( slotReset() ) );
	connect( matchButton, SIGNAL( clicked() ), this, SLOT( AddMatchCombinations()));
	connect(this->matchResultsTable, SIGNAL(emitSetModelSignal()), this, SLOT(setNewModel()));
	connect(this->matchResultsTable, SIGNAL(emitSetProxyModelSignal()), this, SLOT(setNewProxyModel()));
	connect(this->scnMatchView, SIGNAL(dsValueChanged(int)),this->profileMatchView, SLOT(setDsNumValue(int)));
	connect(this->scnMatchView, SIGNAL(ssValueChanged(int)),this->profileMatchView, SLOT(setSsNumValue(int)));

	connect(this, SIGNAL(valueChangedDSP(QList<QPair<QString, DemandDataList > >&)), this->profileMatchView, SLOT(setProfileValue(QList<QPair<QString, DemandDataList > >&)));
	connect(this, SIGNAL(valueChangedSSP(QList<QPair<QString, SupplyDataList > >&)), this->profileMatchView, SLOT(setSupplyProfileValue(QList<QPair<QString, SupplyDataList > >&)));
}

 void Match::createProgressBar()
 {
     matchProgressBar->setRange(0, 10000); //to reduce the number can adjust the speed of the progress bar processing 
     matchProgressBar->setValue(0); //set the initial value for the progress bar
 }

 /*
 * SLOT slotStart
 *
 * This SLOT is called if the user clicks auto/pause/continue
 * button
 */

void Match::slotStart()
{
    // If the progress bar is at the beginning...
    if ( matchProgressBar->value() == -1 ) 
	{
        // the number of steps which are needed to complete the process
		//it could determine the speed the progress bar, based on the value specified in the setMaximum function
        matchProgressBar->setMaximum( 10000 );
    }

    // If the progress is not running...
    if ( !timer.isActive() ) 
	{
        // ...start the timer (and so the progress) with a interval of 1 ms...
        timer.start( 1 );
		timerMatchTable.start(1);
		timestep2 = profileMatchView->timerStep();
        // ...and rename the start/pause/continue button to Pause
        searchButton->setText( "&Pause" );

    } 
	else 
	{ // if the prgress is running...
        // ...stop the timer (and so the prgress)...
        timer.stop();
		timerMatchTable.stop();
		profileMatchView->stopTimer();
        // ...and rename the start/pause/continue button to Continue
        searchButton->setText( "&Continue" );
		profileMatchView->setTimerStep(timestep2);
    }
}

/*
 * SLOT slotReset
 *
 * This SLOT is called when the user clicks the stop button
 */

void Match::slotReset()
{
    // stop the timer and progress
    timer.stop();
	timerMatchTable.stop();
	profileMatchView->stopTimer();

    // rename the start/pause/continue button to Start...
    searchButton->setText( "&Search" );
    // ...and enable this button
    searchButton->setEnabled( TRUE );
//	timer.start();

	profileMatchView->setTimerStep(-1);
    // reset the progressbar
    matchProgressBar->reset();
	profileMatchView->InitialiseAllButtons();
}

/*
 * SLOT slotTimeout
 *
 * This SLOT is called each ms when the timer is
 * active (== progress is running)
 */

void Match::slotTimeout()
{
    int p = matchProgressBar->value();
	timestep2 = profileMatchView->timerStep();
	p = (int)(timestep2*matchProgressBar->maximum()/45);

#if 1
    // If the progress is complete...
    if ( p == matchProgressBar->maximum() )  {
        // ...rename the start/pause/continue button to Start...
        searchButton->setText( "&Auto" );
        // ...and disable it...
        searchButton->setEnabled( FALSE );
		matchProgressBar->setValue( 10000 );
//if the timer is active, then fill the result to the table
        if ( timerMatchTable.isActive() ) 
		   matchCombinations(timestep2);
        // ...and return
//clear the selection of the buttons
		profileMatchView->InitialiseAllButtons();
        return;
    }
#endif
    // If the process is not complete increase it
	matchProgressBar->setValue( p );
    // If the progress is not running...
//if the timer is active, then fill the results to the match result table
    if ( timerMatchTable.isActive() ) 
		matchCombinations(timestep2);
}

void Match::slotPause()
{
    // If the progress is not running...
    if ( !timerMatchTable.isActive() ) 
	{
        // ...start the timer (and so the progress) with a interval of 1 ms...
		timerMatchTable.start(1);
		//set the time step
		matchCombinations(timestep2);
        // ...and rename the start/pause/continue button to Pause
//        pauseButton->setText( "&Pause" );

    } 
	else 
	{ // if the prgress is running...
        // ...stop the timer (and so the prgress)...	
        timerMatchTable.stop();
        // ...and rename the start/pause/continue button to Continue
//        pauseButton->setText( "&Continue" );
//		profileMatchView->setTimerStep(timestep2);
		matchCombinations(timestep2);
    }
}

//the following function doesn't do anything at the moment
void Match::paintEvent( QPaintEvent* event)
{

}


///////////////////////////////
//add match results to the table widget
///////////////////////////////
void Match::AddMatchCombinations()
{
	profileMatchView->dmdProfileSel.clear();
	profileMatchView->supProfileSel.clear();
	profileMatchView->m_LeftDraw = false;
	profileMatchView->m_RightDraw = false;

//for demand buttons
//check the mouse press area is within the defined button range 
	for (int i = 0; i < profileMatchView->dPButtonList.count(); i++)
	{
		if(profileMatchView->dPButtonList[i]->isChecked()) //if it is checked
		{
			profileMatchView->dmdProfileSel.append(i);
			profileMatchView->m_LeftDraw = true;
		}
	}

//for supply buttons
//check the mouse press area is within the defined button range 
	for (int i = 0; i < profileMatchView->sPButtonList.count(); i++)
	{
		if(profileMatchView->sPButtonList[i]->isChecked()) //if it is checked
		{
			profileMatchView->supProfileSel.append(i);
			profileMatchView->m_RightDraw = true;
		}
	}

	if (profileMatchView->m_LeftDraw && profileMatchView->m_RightDraw)
	{
		profileMatchView->setupMatchIdVector(profileMatchView->dmdProfileSel, profileMatchView->supProfileSel);
		profileMatchView->setupMatchStatisticsVector(profileMatchView->dmdProfileSel, profileMatchView->supProfileSel);
		profileMatchView->setupMatchRateVector();
		profileMatchView->setupMatchRankVector();
	}

	if (model->rowCount()!= 0)
		model->removeRows(0, model->rowCount());

	for (int i= 0; i<profileMatchView->pMatchIdVector.size(); i++)
	{
//Match result table header
		model->setVerticalHeaderItem( i, new QStandardItem(QString("%1").arg(profileMatchView->pMatchRankVector[i])) );

//Match reference
		QStandardItem *item = 
			new QStandardItem(profileMatchView->pMatchIdVector[i]);
		item->setEditable( false ); 
		model->setItem(i, 0, item);

//Match index
		QStandardItem *item1 = 
			new QStandardItem(QString::number( profileMatchView->pMatchRateVector[i], 'f', 1 ));
		item1->setEditable( false ); 
		model->setItem(i, 1, item1);
//Inequality coefficient
		QStandardItem *item2 = 
			new QStandardItem(QString::number( profileMatchView->pMatchICVector[i], 'f', 2 ));//QString("%1, 'f', 2").arg(profileMatchView->pMatchICVector[i]));
		item2->setEditable( false ); 
		model->setItem(i, 2, item2);
//Correlation coefficient
		QStandardItem *item3 = 
			new QStandardItem(QString::number( profileMatchView->pMatchCCVector[i], 'f', 2 ));//QString("%1, 'f', 2").arg(profileMatchView->pMatchICVector[i]));
		item3->setEditable( false ); 
		model->setItem(i, 3, item3);
//Energy Deficit
		QStandardItem *item4 = 
			new QStandardItem(QString::number( profileMatchView->pMatchEDVector[i], 'f', 1 ));//QString("%1, 'f', 2").arg(profileMatchView->pMatchICVector[i]));
		item4->setEditable( false ); 
		model->setItem(i, 4, item4);
//Energy Surplus
		QStandardItem *item5 = 
			new QStandardItem(QString::number( profileMatchView->pMatchESVector[i], 'f', 1 ));//QString("%1, 'f', 2").arg(profileMatchView->pMatchICVector[i]));
		item5->setEditable( false ); 
		model->setItem(i, 5, item5);

//Total demand
		QStandardItem *item6 = 
			new QStandardItem(QString::number( profileMatchView->pMatchTDVector[i], 'f', 1 ));//QString("%1, 'f', 2").arg(profileMatchView->pMatchICVector[i]));
		item6->setEditable( false ); 
		model->setItem(i, 6, item6);
//Total supply
		QStandardItem *item7 = 
			new QStandardItem(QString::number( profileMatchView->pMatchTSVector[i], 'f', 1 ));//QString("%1, 'f', 2").arg(profileMatchView->pMatchICVector[i]));
		item7->setEditable( false ); 
		model->setItem(i, 7, item7);
	}
}

//use in auto search
////generate match combination at a certain time step for search
void Match::matchCombinations(int timestep)
{
	profileMatchView->setupMatchRankVector(timestep);

	for (int i = 0; i < timestep; i++)
	{
//Match result table header
		model->setVerticalHeaderItem( i, new QStandardItem(QString("%1").arg(profileMatchView->pMatchRankVector[i]) ));
//Match reference
		QStandardItem *item = 
			new QStandardItem(profileMatchView->pMatchIdVector[i]);
		item->setEditable( false ); 
		model->setItem(i, 0, item);

	
//Match index
		QStandardItem *item1 = 
			new QStandardItem(QString("%1").arg(profileMatchView->pMatchRateVector[i]));
		item1->setEditable( false ); 
		model->setItem(i, 1, item1);

//Inequality coefficient
		QStandardItem *item2 = 
			new QStandardItem(QString("%1").arg(profileMatchView->pMatchICVector[i]));
		item2->setEditable( false ); 
		model->setItem(i, 2, item2);
	}
}
//////////////////////////////////////////////////////
//mainly realise two functions: 
//1 paint the buttons with the selected combinations; 
//2 draw the demand/supply match graph.
/////////////////////////////////////////////////
void Match::deployMatchResults(const QModelIndex& index)
{
//set the flag to enable the buttons being painted
	QString temp = " ";
	PlotMatch* matchPlot = Ui_Match::matchPlot;
	int temp_index = 0;
	profileMatchView->InitialiseAllButtons();
	profileMatchView->m_bFlag = true;

	QVector<float> tempTotalDemand;
	QVector<float> tempTotalSupply;
	
	tempTotalDemand.clear();
	tempTotalSupply.clear();

	tempTotalDemand.resize(m_pTemp[dsNumValue()].second[0].m_pDemandDataVector.count());
	tempTotalSupply.resize(m_pTemp[dsNumValue()].second[0].m_pDemandDataVector.count());

	temp = (index.data(Qt::DisplayRole)).toString();

	profileMatchView->temp_index = profileMatchView->pMatchIdVector.indexOf(temp);
//update the profile match view to show the selected combination
	this->profileMatchView->update();

	QVector<int> tempDmdProfileSel;
	QVector<int> tempSupProfileSel;

	tempDmdProfileSel.clear();
	tempSupProfileSel.clear();

//save the index of the selected demand and supply profiles in temporary vectors
	tempDmdProfileSel = profileMatchView->dmdProfileSelVector[profileMatchView->temp_index];
	tempSupProfileSel = profileMatchView->supProfileSelVector[profileMatchView->temp_index];

//draw graph, this will implement later
// 	demandPlot->drawDemandGraph(item->text(),demandDataVector);
//calculate the total demand and supply
	for (int i = 0; i < m_pTemp[dsNumValue()].second[0].m_pDemandDataVector.count(); i++)
	{
		for (int j =0 ; j < tempDmdProfileSel.count(); j++)
		{
			tempTotalDemand[i] = + m_pTemp[dsNumValue()].second[tempDmdProfileSel[j]].m_pDemandDataVector[i];
		}

		for (int k = 0; k < tempSupProfileSel.count(); k++)
		{
			tempTotalSupply[i] = + m_pTempSupply[ssNumValue()].second[tempSupProfileSel[k]].m_pSupplyDataVector[i];
		}
	}
	matchPlot->detachItems(QwtPlotItem::Rtti_PlotCurve, true);
	matchPlot->drawMatchGraph(tempTotalDemand, tempTotalSupply);

}



void Match::clearMatchResultsTable()
{
	this->profileMatchView->pMatchIdVector.clear();
	this->profileMatchView->pMatchRateVector.clear();
}

//set new model when the "S" button in the table view is unchecked
void Match::setNewModel()
{
	profileMatchView->dmdProfileSel.clear();
	profileMatchView->supProfileSel.clear();
	profileMatchView->m_LeftDraw = false;
	profileMatchView->m_RightDraw = false;

//for demand buttons
//check the mouse press area is within the defined button range 
	for (int i = 0; i < profileMatchView->dPButtonList.count(); i++)
	{
		if(profileMatchView->dPButtonList[i]->isChecked()) //if it is checked
		{
			profileMatchView->dmdProfileSel.append(i);
			profileMatchView->m_LeftDraw = true;
		}
	}

//for supply buttons
//check the mouse press area is within the defined button range 
	for (int i = 0; i < profileMatchView->sPButtonList.count(); i++)
	{
		if(profileMatchView->sPButtonList[i]->isChecked()) //if it is checked
		{
			profileMatchView->supProfileSel.append(i);
			profileMatchView->m_RightDraw = true;
		}
	}

	if (profileMatchView->m_LeftDraw && profileMatchView->m_RightDraw)
	{
		profileMatchView->setupMatchIdVector(profileMatchView->dmdProfileSel, profileMatchView->supProfileSel);
		profileMatchView->setupMatchRateVector();
		profileMatchView->setupMatchRankVector();
	}

	if (model->rowCount()!= 0)
		model->removeRows(0, model->rowCount());

	for (int i= 0; i<profileMatchView->pMatchIdVector.size(); i++)
	{
//Match result table header
		model->setVerticalHeaderItem( i, new QStandardItem(QString("%1").arg(profileMatchView->pMatchRankVector[i])) );

//Match reference
		QStandardItem *item = 
			new QStandardItem(profileMatchView->pMatchIdVector[i]);
		item->setEditable( false ); 
		model->setItem(i, 0, item);

//Match index
		QStandardItem *item1 = 
			new QStandardItem(QString("%1").arg(profileMatchView->pMatchRateVector[i]));
		item1->setEditable( false ); 
		model->setItem(i, 1, item1);

	}
	matchResultsTable->setModel(model);
}

//set new proxy model when the "S" button in the table view is highlighted
void Match::setNewProxyModel()
{
	profileMatchView->dmdProfileSel.clear();
	profileMatchView->supProfileSel.clear();
	profileMatchView->m_LeftDraw = false;
	profileMatchView->m_RightDraw = false;

//for demand buttons
//check the mouse press area is within the defined button range 
	for (int i = 0; i < profileMatchView->dPButtonList.count(); i++)
	{
		if(profileMatchView->dPButtonList[i]->isChecked()) //if it is checked
		{
			profileMatchView->dmdProfileSel.append(i);
			profileMatchView->m_LeftDraw = true;
		}
	}

//for supply buttons
//check the mouse press area is within the defined button range 
	for (int i = 0; i < profileMatchView->sPButtonList.count(); i++)
	{
		if(profileMatchView->sPButtonList[i]->isChecked()) //if it is checked
		{
			profileMatchView->supProfileSel.append(i);
			profileMatchView->m_RightDraw = true;
		}
	}

	if (profileMatchView->m_LeftDraw && profileMatchView->m_RightDraw)
	{
		profileMatchView->setupMatchIdVector(profileMatchView->dmdProfileSel, profileMatchView->supProfileSel);
		profileMatchView->setupMatchRateVector();
		profileMatchView->setupMatchRankVector();
	}

	if (model->rowCount()!= 0)
		model->removeRows(0, model->rowCount());

	for (int i= 0; i<profileMatchView->pMatchIdVector.size(); i++)
	{
//Match result table header
		model->setVerticalHeaderItem( i, new QStandardItem(QString("%1").arg(profileMatchView->pMatchRankVector[i])) );

//Match reference
		QStandardItem *item = 
			new QStandardItem(profileMatchView->pMatchIdVector[i]);
		item->setEditable( false ); 
		model->setItem(i, 0, item);

//Match index
		QStandardItem *item1 = 
			new QStandardItem(QString("%1").arg(profileMatchView->pMatchRateVector[i]));
		item1->setEditable( false ); 
		model->setItem(i, 1, item1);

	}
	sorter->setSourceModel(model);
	sorter->setDynamicSortFilter(true);

	matchResultsTable->setModel(sorter);
}


/*
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(Qt::ControlModifier == QApplication::keyboardModifiers())
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if(mouseEvent)
            {
                if(mouseEvent->button()== Qt::LeftButton)
                {
                    ui->tableWidget->selectColumn(ui->tableWidget->itemAt(mouseEvent->pos())->column());
                    return true;
                }
            }
        }
    }

    return QWidget::eventFilter(object,event);
}
*/

/*
void CApplication::slotCloseAssembly()
{
//	do something

	Qt::KeyboardModifiers keyMod = QApplication::keyboardModifiers ();
	bool isSHIFT = keyMod.testFlag(Qt::ShiftModifier);
	bool isCTRL = keyMod.testFlag(Qt::ControlModifier);
	
	if (isSHIFT && isCTRL)
	{
          // to-do smth
        }
}// end slot
*/

/*
#include "mainfrm.h"
#include "ui_mainfrm.h"
#include "graphicsselectionscene.h"
#include <QGraphicsItem>

MainFrm::MainFrm(QWidget *parent) : QGraphicsView(parent), ui(new Ui::MainFrm) {
    ui->setupUi(this);

    // Create a scene with our own selection behavior
    QGraphicsScene* pScene = new GraphicsSelectionScene(this);
    this->setScene(pScene);

    // Create a few items for testing
    QGraphicsItem* pRect1 = pScene->addRect(10,10,50,50, QColor(Qt::red), QBrush(Qt::blue));
    QGraphicsItem* pRect2 = pScene->addRect(100,-10,50,50);
    QGraphicsItem* pRect3 = pScene->addRect(-200,-30,50,50);

    // Make sure the items are selectable
    pRect1->setFlag(QGraphicsItem::ItemIsSelectable, true);
    pRect2->setFlag(QGraphicsItem::ItemIsSelectable, true);
    pRect3->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

*/

void Match::setValue(QList<QPair<QString, DemandDataList > > &value)
{
	if (value != m_pTemp) 
	{
		m_pTemp = value;
		emit valueChangedDSP(value);
	}
}

void Match::setValue(QList<QPair<QString, SupplyDataList > > &value)
{
	if (value != m_pTempSupply) 
	{
		m_pTempSupply = value;
		emit valueChangedSSP(value);
	}
}


void Match::setDsNumValue(int value)
{
	if (value != m_iDsValue) 
	{
		m_iDsValue = value;
	}
}

void Match::setSsNumValue(int value)
{
	if (value != m_iSsValue) 
	{
		m_iSsValue = value;
	}
}

int Match::dsNumValue()
{
	return m_iDsValue;
}

int Match::ssNumValue()
{
	return m_iSsValue;
}