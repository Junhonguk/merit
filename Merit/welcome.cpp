#include <QtGui>

#include "welcome.h"


Welcome::Welcome(QWidget *parent)
    : QWidget(parent)
{
//    meritTab = new MeritTab;
    setupUi(this);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	
    //meritTab = new MeritTab;
//	connect(startButton, SIGNAL(clicked()), , SLOT(quit()));
}

//Load a Merit introduction file in the specified directory
//show the content in the text browser widget
//

/*
void Welcome::setCurrentTab()
{ 
	meritTab = new MeritTab;
	meritTab->setCurrentTab();
}
*/
/*
void Welcome::setText(QString data)
{
// Set some html data into QTextBrowser
	textBrowser->setHtml(data);
}
*/