#include <QtGui>
#include<QtGui/QSplitter>
#include "demand.h"
#include "DemandSplitter.h"


DemandSplitter::DemandSplitter(QSplitter *parent)
    : QSplitter(parent)
{
//    meritTab = new MeritTab;
	Demand* demandWidget = new Demand(this);
    QwtPlot *demandPlot = new QwtPlot;

	QSplitter splitter(Qt::Vertical);
	splitter.addWidget(demandWidget);
	splitter.addWidget(demandPlot);

//    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //meritTab = new MeritTab;
//	connect(startButton, SIGNAL(clicked()), , SLOT(quit()));
}
/*
void Welcome::setCurrentTab()
{ 
	meritTab = new MeritTab;
	meritTab->setCurrentTab();
}
*/

