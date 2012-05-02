#ifndef	DEMANDSPLITTER_H
#define DEMANDSPLITTER_H
#include <QtGui>
#include <QtGui/QSplitter>
#include "qwt_plot.h"

//#include "ui_demand.h"
//#include "merittab.h"
//#include "plot.h"

class DemandSplitter : public QSplitter
{
    Q_OBJECT
public:
	DemandSplitter(QSplitter *parent = 0);
    QwtPlot *demandPlot;

//private:
//    Plot *d_plot;

public slots:
  //  void setCurrentTab();

};

#endif
