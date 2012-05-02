#ifndef _PLOT_H_
#define _PLOT_H_
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <QtCore/QVector>
#include <QtDebug>
#include <QRectF>

class QwtPlotCurve;

class Plot : public QwtPlot
{
	Q_OBJECT
public:
    Plot( QWidget *parent = NULL);
	~Plot();

protected:
    virtual void resizeEvent( QResizeEvent * );
	QwtScaleDiv yearScaleDiv() const;

public:
//customised functions
	void drawGraph(const QString& site, const QVector<float> &DbT, const QVector<float> &DrS, const QVector<float> &DfS,
		           const QVector<float> &WS, const QVector<float> &WD, const QVector<float> &RH);

private:
    void populate();
    void updateGradient();

    QwtPlotCurve *temperatureCurve;
    QwtPlotCurve *directSolarCurve;
	QwtPlotCurve *diffuseSolarCurve;
    QwtPlotCurve *windSpeedCurve;
    QwtPlotCurve *windDirectionCurve;
    QwtPlotCurve *relativeHumidityCurve;

	QwtPlotZoomer *zoomer;
	QRectF  originalRect;
	QwtPlotPanner *panner;

public slots:
	void whenZoom(const QRectF & );
	void showCurve(QwtPlotItem *item, bool on);
};

#endif
