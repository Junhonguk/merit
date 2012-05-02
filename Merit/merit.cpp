#include <QtGui>
#include "merit.h"
#include "merittab.h"

Merit::Merit(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
//add the meritTab widgets
	meritTab = new MeritTab;
	setCentralWidget(meritTab);
    
// to show the information about Merit
	createActions();
}

Merit::~Merit()
{

}
/*
void Merit::createStatusBar()
{
    locationLabel = new QLabel(" W999 ");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());
    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);
    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);
    connect(spreadsheet, SIGNAL(currentCellChanged(int, int, int, int)),
            this, SLOT(updateStatusBar()));
    connect(spreadsheet, SIGNAL(modified()),
            this, SLOT(spreadsheetModified()));
    updateStatusBar();
}
*/

void Merit::aboutMerit()
{
    QMessageBox::about(this, tr("About Merit"),
            tr("<h2>Merit v1.1</h2>"
               "<p>Copyright &copy; 2012 ESRU, University of Strathclyde"
               "<p>Merit is a tool for demand-supply matching."));
}

void Merit::createActions()
{
//	meritTab = new MeritTab;
//	welcome = new Welcome;

//	connect(welcome->startButton, SIGNAL(clicked()), this, SLOT(aboutMerit()));
	connect(ui.actionAbout_Merit, SIGNAL(triggered()), this, SLOT(aboutMerit()));
//	connect(ui.actionNew_project, SIGNAL(triggered()), this, SLOT(addSubWin()));

   
}