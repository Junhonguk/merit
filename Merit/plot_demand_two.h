#ifndef _PLOTDEMANDTWO_H_
#define _PLOTDEMANDTWO_H_
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <QtCore/QVector>
class QwtPlotCurve;

class PlotDemandTwo : public QwtPlot
{
public:
    PlotDemandTwo( QWidget *parent = NULL);

	QwtPlotCurve *cDemand;

	QwtPlotCurve *c1Demand;
	QwtPlotCurve *c2Demand;

	int m_NoOfDemands;
	QVector <QString> m_DemandNames;

	~PlotDemandTwo();
protected:
    virtual void resizeEvent( QResizeEvent * );

public:
//customised functions
	void drawDemandGraph(const QString& name, const QVector<float> &demand);
	void drawMultipleDemandsGraph(const QVector <QString> &name, const QVector< QVector <float> > &demand);
private:
    void populate();
    void updateGradient();

//	void populateTwo(const QVector <QString> &name);

};

#endif
