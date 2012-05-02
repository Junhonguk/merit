#include "plot_demand_two.h"
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_intervalcurve.h>
#include <qwt_legend.h>
#include <qwt_interval_symbol.h>
#include <qwt_symbol.h>
#include <qwt_series_data.h>
#include <qwt_text.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qdatetime.h>
#include <qfiledialog.h>
#include <qimagewriter.h>
#include <qprintdialog.h>
#include <qfileinfo.h>

#include <qwt_math.h>
#include <math.h>

class FunctionData: public QwtSyntheticPointData
{
public:
    FunctionData(double(*y)(double)):
        QwtSyntheticPointData(100),
        d_y(y)
    {
    }

    virtual double y(double x) const
    {
        return d_y(x);
    }

private:
    double(*d_y)(double);
};

PlotDemandTwo::PlotDemandTwo(QWidget *parent):
    QwtPlot( parent )
{
    // panning with the left mouse button
//    (void) new QwtPlotPanner( canvas() );

    // zoom in/out with the wheel
//    (void) new QwtPlotMagnifier( canvas() );
//	m_sWeatherLocation = 1;
    setAutoFillBackground( true );
    setPalette( QPalette( QColor( 165, 193, 228 ) ) );
    updateGradient();

    setTitle("Demand profile");
    insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    // axes 
    setAxisTitle(xBottom, "Time (hrs)" );
    setAxisScale(xBottom, 0.0, 8760.0);

    setAxisTitle(yLeft, "Power (kW)");
    setAxisScale(yLeft, -10.0, 1000.0);

    // canvas
    canvas()->setLineWidth( 1 );
    canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas()->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas()->setPalette( canvasPalette );

	populate();
}




PlotDemandTwo::~PlotDemandTwo()
{
//	delete cDemand;
//	delete c1Demand;
//	delete c2Demand;
//	delete c3Demand;
//	delete c4Demand;
//	delete c5Demand;
//	delete c6Demand;
//	delete c7Demand;
//	delete c8Demand;
//	delete c9Demand;
//	delete c10Demand;
}
void PlotDemandTwo::populate()
{
    // Insert new curves
    c1Demand = new QwtPlotCurve("Total");
    c1Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Demand->setPen(QPen(Qt::red));
    c1Demand->attach(this);

    c2Demand = new QwtPlotCurve("Demand 1");
    c2Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Demand->setPen(QPen(Qt::green));
    c2Demand->attach(this);
}

void PlotDemandTwo::drawDemandGraph(const QString& name, const QVector<float> &demand)
{
    //initialise the plot settings
	PlotDemandTwo();
//	populate();
    double min_value = 1000.0;
	double max_value = 0.0;
//    setAxisScale(yLeft, demand.min(), demand.max());

    // Set up demand data
	const int numHrs = 8760;
    QVector<QPointF> demandData(8760);

	for ( int i = 0; i < numHrs; i++ )
    {
		if (demand[i] < min_value)
			min_value = demand[i];

		if (demand[i] > max_value)
			max_value = demand[i];

		demandData[i] = QPointF( double( i ), demand[i]);
    }

	//set up the data for display
	setAxisScale(yLeft, min_value, max_value);
    cDemand->setSamples(demandData);

	//redraw the graph
	replot();
}

void PlotDemandTwo::drawMultipleDemandsGraph(const QVector <QString> &name, const QVector< QVector <float> > &demand)
{	
	PlotDemandTwo();
    //initialise the plot settings
    double min_value = 1000.0;
	double max_value = 0.0;
//    setAxisScale(yLeft, demand.min(), demand.max());

    // Set up demand data
	const int numHrs = 8760;
    QVector< QVector<QPointF> > demandData;
	//Allocate memory
	demandData.clear();
	demandData.reserve(demand.size());
	demandData.resize(demand.size());

	for (int k = 0; k < demand.size(); k++)
	{
	  demandData[k].clear();
	  demandData[k].reserve(8760);
	  demandData[k].resize(8760);
	}
	int NOofDemands = 0;
	NOofDemands = demand.size();
	for (int j = 0; j < NOofDemands; j++)
	{
		for ( int i = 0; i < numHrs; i++ )
		{
			if (demand[1][i] < min_value)
				min_value = demand[1][i];

			if (demand[1][i] > max_value)
				max_value = demand[1][i];

			demandData[j][i] = QPointF( double( i ), demand[j][i]);
		}
	}

	//set up the data for display
	setAxisScale(yLeft, min_value, max_value);
	c1Demand->setSamples(demandData[0]);
	c2Demand->setSamples(demandData[1]);
	//redraw the graph
	replot();
}

void PlotDemandTwo::updateGradient()
{
    QPalette pal = palette();

    const QColor buttonColor = pal.color( QPalette::Button );

#ifdef Q_WS_X11
    // Qt 4.7.1: QGradient::StretchToDeviceMode is buggy on X11

    QLinearGradient gradient( rect().topLeft(), rect().bottomLeft() );
    gradient.setColorAt( 0.0, Qt::white );
    gradient.setColorAt( 0.7, buttonColor );
    gradient.setColorAt( 1.0, buttonColor );
#else
    QLinearGradient gradient( 0, 0, 0, 1 );
    gradient.setCoordinateMode( QGradient::StretchToDeviceMode );
    gradient.setColorAt( 0.0, Qt::white );
    gradient.setColorAt( 0.7, buttonColor );
    gradient.setColorAt( 1.0, buttonColor );
#endif

    pal.setBrush( QPalette::Window, gradient );
    setPalette( pal );
}

void PlotDemandTwo::resizeEvent( QResizeEvent *event )
{
    QwtPlot::resizeEvent( event );
#ifdef Q_WS_X11
    updateGradient();
#endif
}
