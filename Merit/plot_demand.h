#ifndef _PLOT_DEMAND_H
#define _PLOT_DEMAND_H
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <QtCore/QVector>
class QwtPlotCurve;
class QwtPlotZoomer;

class PlotDemand : public QwtPlot
{
Q_OBJECT
public:
    PlotDemand( QWidget *parent = NULL);

    QwtPlotCurve *cDemand;
	QwtPlotCurve *demandCurve;

	QwtPlotCurve *c1Demand;
	QwtPlotCurve *c2Demand;
	QwtPlotCurve *c3Demand;
	QwtPlotCurve *c4Demand;
	QwtPlotCurve *c5Demand;
	QwtPlotCurve *c6Demand;
	QwtPlotCurve *c7Demand;
	QwtPlotCurve *c8Demand;
	QwtPlotCurve *c9Demand;
	QwtPlotCurve *c10Demand;

	int m_NoOfDemands;
	QVector <QString> m_DemandNames;

	~PlotDemand();
protected:
    virtual void resizeEvent( QResizeEvent * );
	QwtScaleDiv yearScaleDiv() const;

public:
//customised functions
	void drawDemandGraph(const QString& name, const QVector<float> &demand);
	void drawMultipleDemandsGraph(const QVector <QString> &name, const QVector< QVector <float> > &demand);
private:
//    void populate();
    void updateGradient();

	void populateSingle(const QString &name);

	void populateOne(const QVector <QString> &name);
	void populateTwo(const QVector <QString> &name);
	void populateThree(const QVector <QString> &name);
	void populateFour(const QVector <QString> &name);
	void populateFive(const QVector <QString> &name);
	void populateSix(const QVector <QString> &name);
	void populateSeven(const QVector <QString> &name);
	void populateEight(const QVector <QString> &name);
	void populateNine(const QVector <QString> &name);
	void populateTen(const QVector <QString> &name);

	QwtPlotZoomer *demandZoomer;
	QRectF  originalRect;
	QwtPlotPanner *panner;

public slots:
	void whenDemandZoom(const QRectF & rect);

};
#endif
