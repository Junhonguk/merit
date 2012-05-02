#include <QtGui>

#include "supply.h"


Supply::Supply(QWidget *parent)
    : QWidget(parent)
{
//    meritTab = new MeritTab;
    setupUi(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	supplySource->addItem("Supply profile generator");
	supplySource->addItem("Supply database");
	supplySource->addItem("Import supply");
    //meritTab = new MeritTab;

	profileType->addItem("Electricity");
	profileType->addItem("Thermal");
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