#include <QtGui>
#include "welcome.h"
#include "context.h"
#include "demand.h"
#include "supply.h"
#include "match.h"
#include "merittab.h"

MeritTab::MeritTab(QWidget *parent)
    : QTabWidget(parent)
{

	Welcome* welcomeWidget = new Welcome(this); 
	addTab(welcomeWidget, QString("Welcome") );

	//Plotter *plotter;
    //plotter->readFlightCurves("in1.txt");
    Context* contextWidget = new Context(this);
	addTab(contextWidget, QString("Weather") ); 
    
	Demand* demandWidget = new Demand(this);
	addTab(demandWidget, QString("Demand") ); 

	Supply* supplyWidget = new Supply(this);
	addTab(supplyWidget, QString("Supply") ); 

	Match* matchWidget = new Match(this);
	addTab(matchWidget, QString("Match") );

	//setCurrentIndex(1);

    //createActions();
//	connect(welcomeWidget->startButton, SIGNAL(clicked()), this, SLOT(setCurrentTab()));
	connect(demandWidget, SIGNAL(valueChanged(QMap<QString, QString>&)), matchWidget->scnMatchView, SLOT(setValue(QMap<QString, QString>&)));
	connect(supplyWidget, SIGNAL(valueChanged(QMap<QString, QString>&)), matchWidget->scnMatchView, SLOT(setSSValue(QMap<QString, QString>&)));

	connect(demandWidget, SIGNAL(valueChangedDSP(QList<QPair<QString, DemandDataList > >&)), matchWidget, SLOT(setValue(QList<QPair<QString, DemandDataList > >&)));
	connect(supplyWidget, SIGNAL(valueChangedSSP(QList<QPair<QString, SupplyDataList > >&)), matchWidget, SLOT(setValue(QList<QPair<QString, SupplyDataList > >&)));
}

/*
void MeritTab::createActions()
{
    connect(uiwel.startButton, SIGNAL(clicked()), this, SLOT(closed()));
}
*/

//void MeritTab::setCurrentTab()
//{
//    setCurrentIndex(1);
//}
