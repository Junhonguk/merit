#ifndef _PLOTSUPPLY_H_
#define _PLOTSUPPLY_H_
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <QtCore/QVector>
class QwtPlotCurve;
class QwtPlotZoomer;

class PlotSupply : public QwtPlot
{
public:
    PlotSupply( QWidget *parent = NULL);

	QwtPlotCurve *cSupply;
	QwtPlotCurve *supplyCurve;

	QwtPlotCurve *c1Supply;
	QwtPlotCurve *c2Supply;
	QwtPlotCurve *c3Supply;
	QwtPlotCurve *c4Supply;
	QwtPlotCurve *c5Supply;
	QwtPlotCurve *c6Supply;
	QwtPlotCurve *c7Supply;
	QwtPlotCurve *c8Supply;
	QwtPlotCurve *c9Supply;
	QwtPlotCurve *c10Supply;

	int m_NoOfSupplys;
	QVector <QString> m_SupplyNames;

	~PlotSupply();
protected:
    virtual void resizeEvent( QResizeEvent * );
	QwtScaleDiv yearScaleDiv() const;
public:
//customised functions
	void drawSupplyGraph(const QString& name, const QVector<float> &supply);
	void drawMultipleSupplysGraph(const QVector <QString> &name, const QVector< QVector <float> > &supply);
private:
    void populate();
    void updateGradient();

	void populateSingle();

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

	QwtPlotZoomer *supplyZoomer;
	QRectF  originalRect;
	QwtPlotPanner *panner;

public slots:
	void whenSupplyZoom(const QRectF & rect);
};

#endif
