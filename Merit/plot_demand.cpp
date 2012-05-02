#include "plot_demand.h"
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

//when the period defined is in the range of (745~8760) 
class DemandYearScaleDraw: public QwtScaleDraw
{
public:
    DemandYearScaleDraw()
    {
		m_iCountMax = 0;

        setTickLength( QwtScaleDiv::MajorTick, 0 );
        setTickLength( QwtScaleDiv::MinorTick, 0 );
        setTickLength( QwtScaleDiv::MediumTick, 10 );

        setLabelRotation( -60.0 );
        setLabelAlignment( Qt::AlignLeft | Qt::AlignVCenter );
//		title.setFont(QFont("Ms Shell Dlg 2", 10, QFont::Bold));
//		setAxisTitle(xBottom, title);
	//        setSpacing( 15 );
//for adjusting the distance between the x axis item label and the x axis
		setSpacing( 5 );
    }

    virtual QwtText label( double value ) const
    {
		 double difference = 0;
		 double xBegin = scaleDiv().lowerBound();
		 double xEnd = scaleDiv().upperBound();
 		 QwtText title; 

		 difference = xEnd - xBegin;
		 QDate t = QDate::fromJulianDay((int)(value/24));


		 if ( difference > 760 )
		 {
			title = QDate::longMonthName( int( value / (30*24) ) + 1 );
		 }
		 else
		 {
			title = t.toString("ddd dd/MM"); //show the date time format in "Mon 01/02"
		 }
		 title.setFont(QFont("Ms Shell Dlg 2", 8));
		 return title;
    }

private:
	int m_iCountMax;

};
PlotDemand::PlotDemand(QWidget *parent):
    QwtPlot( parent )
{
    // panning with the left mouse button
//    (void) new QwtPlotPanner( canvas() );

    // zoom in/out with the wheel
  //  (void) new QwtPlotMagnifier( canvas() );

    setAutoFillBackground( true );
    setPalette( QPalette( QColor( 165, 193, 228 ) ) );
    updateGradient();

   // setTitle("Demand profile");
    insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    // axes 
	QwtText titleX("Year");
    titleX.setFont(QFont("Ms Shell Dlg 2", 10, QFont::Bold));
    setAxisTitle(xBottom, titleX);

	setAxisScaleDiv( xBottom, yearScaleDiv() );
    setAxisScaleDraw( xBottom, new DemandYearScaleDraw() );

	QwtText titleY("Power [kW]");
    titleY.setFont(QFont("Ms Shell Dlg 2", 10, QFont::Bold));
    setAxisTitle(yLeft, titleY);

    // canvas
    canvas()->setLineWidth( 1 );
    canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas()->setBorderRadius( 10 );

    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas()->setPalette( canvasPalette );
}




PlotDemand::~PlotDemand()
{
}

//check if the year is leap year and then sepcify the different set of days to the specific year
QwtScaleDiv PlotDemand::yearScaleDiv() const
{
//    const int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    const int days[] = { 744, 672, 744, 720, 744, 720, 744, 744, 720, 744, 720, 744 }; //in hours

    QList<double> ticks[QwtScaleDiv::NTickTypes];

    QList<double> &mediumTicks = ticks[QwtScaleDiv::MediumTick];
    mediumTicks += 0.0;
    for ( uint i = 0; i < sizeof( days ) / sizeof( days[0] ); i++ )
//        mediumTicks += mediumTicks.last() + days[i];
        mediumTicks += mediumTicks.last() + days[i]; //get the right tick for the new hourly data for each month

    QList<double> &minorTicks = ticks[QwtScaleDiv::MinorTick];
    for ( int i = 1; i <= 365*24; i += 168 )
        minorTicks += i;

    QList<double> &majorTicks = ticks[QwtScaleDiv::MajorTick];
    for ( int i = 0; i < 12; i++ )
        majorTicks += i * 30 *24 + 15*24;

    QwtScaleDiv scaleDiv( mediumTicks.first(), mediumTicks.last() + 1, ticks );
    return scaleDiv;
}
/*
void PlotDemand::populate()
{
    // Insert new curves
	cDemand->detach();
	c1Demand->detach();
	c2Demand->detach();
	c3Demand->detach();
	c4Demand->detach();
	c5Demand->detach();
	c6Demand->detach();
	c7Demand->detach();
	c8Demand->detach();

    cDemand = new QwtPlotCurve("");//("Demand Profile");
    cDemand->setRenderHint(QwtPlotItem::RenderAntialiased);
    cDemand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    cDemand->setPen(QPen(Qt::red));
    cDemand->attach(this);
}
*/
void PlotDemand::populateSingle(const QString &name)
{
    cDemand = new QwtPlotCurve(name);
    cDemand->setRenderHint(QwtPlotItem::RenderAntialiased);
    cDemand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    cDemand->setPen(QPen(Qt::red));
    cDemand->attach(this);
}

void PlotDemand::drawDemandGraph(const QString& name, const QVector<float> &demand)
{
    //initialise the plot settings
//	PlotDemand();
	PlotDemand();

    // Set up demand data
	const int numHrs = 8760; //hourly data for the whole year

    QVector<QPointF> demandData(numHrs);

	for ( int i = 0; i < numHrs; i++ )
    {
		demandData[i] = QPointF( double( i ), demand[i]);
//		qDebug("%4.3f",temperatureData[i].y());
    }

//set up the curve for the demand
    demandCurve = new QwtPlotCurve(name);
    demandCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    demandCurve->setStyle( QwtPlotCurve::Lines );
	demandCurve->setPen( QPen( Qt::red ) );
    demandCurve->setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
	demandCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine);

    QwtSymbol *symbol1 = new QwtSymbol( QwtSymbol::Ellipse );
    symbol1->setSize( 4 );
    symbol1->setPen( QPen( Qt::red ) );
	symbol1->setBrush(QBrush(Qt::black));
    demandCurve->setSymbol( symbol1 );
    demandCurve->setSamples( demandData );
	demandCurve->attach( this );

//enable zooming functionality
    demandZoomer = new QwtPlotZoomer( QwtPlot::xBottom, QwtPlot::yLeft, this->canvas() );
	demandZoomer->setZoomBase();
    demandZoomer->setRubberBandPen( QColor( Qt::black ) );
    demandZoomer->setTrackerPen( QColor( Qt::black ) );
    demandZoomer->setMousePattern( QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier );
    demandZoomer->setMousePattern( QwtEventPattern::MouseSelect3,
        Qt::RightButton );

	connect(demandZoomer, SIGNAL(zoomed(const QRectF &)), this, SLOT(whenDemandZoom(const QRectF &)));
    panner = new QwtPlotPanner( canvas() );
    panner->setMouseButton( Qt::MidButton );

    // axes 
	QwtText title("Year");
    title.setFont(QFont("Ms Shell Dlg 2", 10, QFont::Bold));
    setAxisTitle(xBottom, title);  

	setAxisScaleDiv( xBottom, yearScaleDiv() );
    setAxisScaleDraw( xBottom, new DemandYearScaleDraw() );

//set the scale of y axis update automatically
	setAxisAutoScale(yLeft, true);
//	updateAxes();
	//redraw the graph
	replot();
}

void PlotDemand::drawMultipleDemandsGraph(const QVector <QString> &name, const QVector< QVector <float> > &demand)
{	
	m_NoOfDemands = 0;
	m_NoOfDemands = demand.size();

	m_DemandNames = name;

//	PlotDemand();
	cDemand->detach();

	setAutoFillBackground( true );
    setPalette( QPalette( QColor( 165, 193, 228 ) ) );
    updateGradient();

//    setTitle("Demand");
    insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    // axes 
    setAxisTitle(xBottom, "Time(hrs)" );
    setAxisScale(xBottom, 0.0, 8760.0);

    setAxisTitle(yLeft, "Power(kW)");
    setAxisScale(yLeft, -10.0, 1000.0);

    // canvas
    canvas()->setLineWidth( 1 );
    canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas()->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas()->setPalette( canvasPalette );

//	PlotDemand();
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

    switch (NOofDemands)
	{
	case 0:
		break;
	case 1:
		populateOne(name);
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

		//redraw the graph
		replot();
		break;
	case 2:
		c1Demand->detach();
		c2Demand->detach();
		c3Demand->detach();
		c4Demand->detach();
		c5Demand->detach();
		c6Demand->detach();
		c7Demand->detach();
		c8Demand->detach();
		populateTwo(name);
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
//		setAutoReplot(true);

		break;
	case 3:
		c1Demand->detach();
		c2Demand->detach();
		c3Demand->detach();
		c4Demand->detach();
		c5Demand->detach();
		c6Demand->detach();
		c7Demand->detach();
		c8Demand->detach();
		populateThree(name);
		for (int j = 0; j < NOofDemands; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (demand[2][i] < min_value)
					min_value = demand[2][i];

				if (demand[2][i] > max_value)
					max_value = demand[2][i];

				demandData[j][i] = QPointF( double( i ), demand[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Demand->setSamples(demandData[2]);
		c2Demand->setSamples(demandData[1]);
		c3Demand->setSamples(demandData[0]);
		//redraw the graph
		replot();
		break;
	case 4:
		c1Demand->detach();
		c2Demand->detach();
		c3Demand->detach();
		c4Demand->detach();
		c5Demand->detach();
		c6Demand->detach();
		c7Demand->detach();
		c8Demand->detach();
		populateFour(name);
		for (int j = 0; j < NOofDemands; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (demand[3][i] < min_value)
					min_value = demand[3][i];

				if (demand[3][i] > max_value)
					max_value = demand[3][i];

				demandData[j][i] = QPointF( double( i ), demand[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Demand->setSamples(demandData[3]);
		c2Demand->setSamples(demandData[2]);
		c3Demand->setSamples(demandData[1]);
		c4Demand->setSamples(demandData[0]);
		//redraw the graph
		replot();
		break;
	case 5:
		c1Demand->detach();
		c2Demand->detach();
		c3Demand->detach();
		c4Demand->detach();
		c5Demand->detach();
		c6Demand->detach();
		c7Demand->detach();
		c8Demand->detach();
		populateFive(name);
		for (int j = 0; j < NOofDemands; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (demand[4][i] < min_value)
					min_value = demand[4][i];

				if (demand[4][i] > max_value)
					max_value = demand[4][i];

				demandData[j][i] = QPointF( double( i ), demand[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Demand->setSamples(demandData[4]);
		c2Demand->setSamples(demandData[3]);
		c3Demand->setSamples(demandData[2]);
		c4Demand->setSamples(demandData[1]);
		c5Demand->setSamples(demandData[0]);
		//redraw the graph
		replot();

		break;
	case 6:
		c1Demand->detach();
		c2Demand->detach();
		c3Demand->detach();
		c4Demand->detach();
		c5Demand->detach();
		c6Demand->detach();
		c7Demand->detach();
		c8Demand->detach();
		populateSix(name);
		for (int j = 0; j < NOofDemands; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (demand[5][i] < min_value)
					min_value = demand[5][i];

				if (demand[5][i] > max_value)
					max_value = demand[5][i];

				demandData[j][i] = QPointF( double( i ), demand[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Demand->setSamples(demandData[5]);
		c2Demand->setSamples(demandData[4]);
		c3Demand->setSamples(demandData[3]);
		c4Demand->setSamples(demandData[2]);
		c5Demand->setSamples(demandData[1]);
		c6Demand->setSamples(demandData[0]);
		//redraw the graph
		replot();
		break;
	case 7:
		c1Demand->detach();
		c2Demand->detach();
		c3Demand->detach();
		c4Demand->detach();
		c5Demand->detach();
		c6Demand->detach();
		c7Demand->detach();
		c8Demand->detach();
		populateSeven(name);
		for (int j = 0; j < NOofDemands; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (demand[6][i] < min_value)
					min_value = demand[6][i];

				if (demand[6][i] > max_value)
					max_value = demand[6][i];

				demandData[j][i] = QPointF( double( i ), demand[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Demand->setSamples(demandData[6]);
		c2Demand->setSamples(demandData[5]);
		c3Demand->setSamples(demandData[4]);
		c4Demand->setSamples(demandData[3]);
		c5Demand->setSamples(demandData[2]);
		c6Demand->setSamples(demandData[1]);
		c7Demand->setSamples(demandData[0]);
		//redraw the graph
		replot();
		break;
	case 8:
		c1Demand->detach();
		c2Demand->detach();
		c3Demand->detach();
		c4Demand->detach();
		c5Demand->detach();
		c6Demand->detach();
		c7Demand->detach();
		c8Demand->detach();
		populateEight(name);
		for (int j = 0; j < NOofDemands; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (demand[7][i] < min_value)
					min_value = demand[7][i];

				if (demand[7][i] > max_value)
					max_value = demand[7][i];

				demandData[j][i] = QPointF( double( i ), demand[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Demand->setSamples(demandData[7]);
		c2Demand->setSamples(demandData[6]);
		c3Demand->setSamples(demandData[5]);
		c4Demand->setSamples(demandData[4]);
		c5Demand->setSamples(demandData[3]);
		c6Demand->setSamples(demandData[2]);
		c7Demand->setSamples(demandData[1]);
		c8Demand->setSamples(demandData[0]);
		//redraw the graph
		replot();
		break;
	case 9:
		populateNine(name);
		for (int j = 0; j < NOofDemands; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (demand[8][i] < min_value)
					min_value = demand[8][i];

				if (demand[8][i] > max_value)
					max_value = demand[8][i];

				demandData[j][i] = QPointF( double( i ), demand[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Demand->setSamples(demandData[8]);
		c2Demand->setSamples(demandData[7]);
		c3Demand->setSamples(demandData[6]);
		c4Demand->setSamples(demandData[5]);
		c5Demand->setSamples(demandData[4]);
		c6Demand->setSamples(demandData[3]);
		c7Demand->setSamples(demandData[2]);
		c8Demand->setSamples(demandData[1]);
		c9Demand->setSamples(demandData[0]);
		//redraw the graph
		replot();
		break;
	case 10:
//		detach
		populateTen(name);
		for (int j = 0; j < NOofDemands; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (demand[9][i] < min_value)
					min_value = demand[9][i];

				if (demand[9][i] > max_value)
					max_value = demand[9][i];

				demandData[j][i] = QPointF( double( i ), demand[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Demand->setSamples(demandData[9]);
		c2Demand->setSamples(demandData[8]);
		c3Demand->setSamples(demandData[7]);
		c4Demand->setSamples(demandData[6]);
		c5Demand->setSamples(demandData[5]);
		c6Demand->setSamples(demandData[4]);
		c7Demand->setSamples(demandData[3]);
		c8Demand->setSamples(demandData[2]);
		c9Demand->setSamples(demandData[1]);
		c10Demand->setSamples(demandData[0]);
		//redraw the graph
		replot();
		break;
	case 11:

		break;
	case 12:

		break;
	case 13:

		break;
	case 14:

		break;
	case 15:

		break;
	}


}

void PlotDemand::updateGradient()
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

void PlotDemand::resizeEvent( QResizeEvent *event )
{
    QwtPlot::resizeEvent( event );
#ifdef Q_WS_X11
    updateGradient();
#endif
}


void PlotDemand::populateOne(const QVector <QString> &name)
{
    // Insert new curves
	delete c1Demand;
    c1Demand = new QwtPlotCurve(name[0]);
    c1Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Demand->setPen(QPen(Qt::red));
    c1Demand->attach(this);
}

void PlotDemand::populateTwo(const QVector <QString> &name)
{
    // Insert new curves
    c1Demand = new QwtPlotCurve(name[1]);	
    c1Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Demand->setPen(QPen(Qt::red));
    c1Demand->attach(this);

	c2Demand = new QwtPlotCurve(name[0]);
    c2Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Demand->setPen(QPen(Qt::green));
    c2Demand->attach(this);
}

void PlotDemand::populateThree(const QVector <QString> &name)
{
    // Insert new curves
//	delete c1Demand;
//	delete c2Demand;
//	delete c3Demand;
    c1Demand = new QwtPlotCurve(name[2]);
    c1Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Demand->setPen(QPen(Qt::red));
    c1Demand->attach(this);

    c2Demand = new QwtPlotCurve(name[1]);
    c2Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Demand->setPen(QPen(Qt::green));
    c2Demand->attach(this);

    c3Demand = new QwtPlotCurve(name[0]);
    c3Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Demand->setPen(QPen(Qt::blue));
    c3Demand->attach(this);
}

void PlotDemand::populateFour(const QVector <QString> &name)
{
    // Insert new curves
//	delete c1Demand;
//	delete c2Demand;
//	delete c3Demand;
//	delete c4Demand;

    c1Demand = new QwtPlotCurve(name[3]);
    c1Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Demand->setPen(QPen(Qt::red));
    c1Demand->attach(this);

    c2Demand = new QwtPlotCurve(name[2]);
    c2Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Demand->setPen(QPen(Qt::green));
    c2Demand->attach(this);

    c3Demand = new QwtPlotCurve(name[1]);
    c3Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Demand->setPen(QPen(Qt::blue));
    c3Demand->attach(this);

    c4Demand = new QwtPlotCurve(name[0]);
    c4Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Demand->setPen(QPen(Qt::cyan));
    c4Demand->attach(this);
}

void PlotDemand::populateFive(const QVector <QString> &name)
{
    // Insert new curves
    c1Demand = new QwtPlotCurve(name[4]);
    c1Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Demand->setPen(QPen(Qt::red));
    c1Demand->attach(this);

    c2Demand = new QwtPlotCurve(name[3]);
    c2Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Demand->setPen(QPen(Qt::green));
    c2Demand->attach(this);

    c3Demand = new QwtPlotCurve(name[2]);
    c3Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Demand->setPen(QPen(Qt::blue));
    c3Demand->attach(this);

    c4Demand = new QwtPlotCurve(name[1]);
    c4Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Demand->setPen(QPen(Qt::cyan));
    c4Demand->attach(this);

    c5Demand = new QwtPlotCurve(name[0]);
    c5Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Demand->setPen(QPen(Qt::magenta));
    c5Demand->attach(this);
}

void PlotDemand::populateSix(const QVector <QString> &name)
{
    // Insert new curves
    c1Demand = new QwtPlotCurve(name[5]);
    c1Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Demand->setPen(QPen(Qt::red));
    c1Demand->attach(this);

    c2Demand = new QwtPlotCurve(name[4]);
    c2Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Demand->setPen(QPen(Qt::green));
    c2Demand->attach(this);

    c3Demand = new QwtPlotCurve(name[3]);
    c3Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Demand->setPen(QPen(Qt::blue));
    c3Demand->attach(this);

    c4Demand = new QwtPlotCurve(name[2]);
    c4Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Demand->setPen(QPen(Qt::cyan));
    c4Demand->attach(this);

    c5Demand = new QwtPlotCurve(name[1]);
    c5Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Demand->setPen(QPen(Qt::magenta));
    c5Demand->attach(this);

    c6Demand = new QwtPlotCurve(name[0]);
    c6Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Demand->setPen(QPen(Qt::yellow));
    c6Demand->attach(this);
}

void PlotDemand::populateSeven(const QVector <QString> &name)
{
    // Insert new curves
    c1Demand = new QwtPlotCurve(name[6]);
    c1Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Demand->setPen(QPen(Qt::red));
    c1Demand->attach(this);

    c2Demand = new QwtPlotCurve(name[5]);
    c2Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Demand->setPen(QPen(Qt::green));
    c2Demand->attach(this);

    c3Demand = new QwtPlotCurve(name[4]);
    c3Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Demand->setPen(QPen(Qt::blue));
    c3Demand->attach(this);

    c4Demand = new QwtPlotCurve(name[3]);
    c4Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Demand->setPen(QPen(Qt::cyan));
    c4Demand->attach(this);

    c5Demand = new QwtPlotCurve(name[2]);
    c5Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Demand->setPen(QPen(Qt::magenta));
    c5Demand->attach(this);

    c6Demand = new QwtPlotCurve(name[1]);
    c6Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Demand->setPen(QPen(Qt::yellow));
    c6Demand->attach(this);

    c7Demand = new QwtPlotCurve(name[0]);
    c7Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c7Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c7Demand->setPen(QPen(Qt::gray));
    c7Demand->attach(this);
}

void PlotDemand::populateEight(const QVector <QString> &name)
{
    // Insert new curves
    c1Demand = new QwtPlotCurve(name[7]);
    c1Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Demand->setPen(QPen(Qt::red));
    c1Demand->attach(this);

    c2Demand = new QwtPlotCurve(name[6]);
    c2Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Demand->setPen(QPen(Qt::green));
    c2Demand->attach(this);

    c3Demand = new QwtPlotCurve(name[5]);
    c3Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Demand->setPen(QPen(Qt::blue));
    c3Demand->attach(this);

    c4Demand = new QwtPlotCurve(name[4]);
    c4Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Demand->setPen(QPen(Qt::cyan));
    c4Demand->attach(this);

    c5Demand = new QwtPlotCurve(name[3]);
    c5Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Demand->setPen(QPen(Qt::magenta));
    c5Demand->attach(this);

    c6Demand = new QwtPlotCurve(name[2]);
    c6Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Demand->setPen(QPen(Qt::yellow));
    c6Demand->attach(this);

    c7Demand = new QwtPlotCurve(name[1]);
    c7Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c7Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c7Demand->setPen(QPen(Qt::gray));
    c7Demand->attach(this);

    c8Demand = new QwtPlotCurve(name[0]);
    c8Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c8Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c8Demand->setPen(QPen(Qt::black));
    c8Demand->attach(this);
}

void PlotDemand::populateNine(const QVector <QString> &name)
{
    // Insert new curves
    // Insert new curves
    c1Demand = new QwtPlotCurve(name[8]);
    c1Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Demand->setPen(QPen(Qt::red));
    c1Demand->attach(this);

    c2Demand = new QwtPlotCurve(name[7]);
    c2Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Demand->setPen(QPen(Qt::green));
    c2Demand->attach(this);

    c3Demand = new QwtPlotCurve(name[6]);
    c3Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Demand->setPen(QPen(Qt::blue));
    c3Demand->attach(this);

    c4Demand = new QwtPlotCurve(name[5]);
    c4Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Demand->setPen(QPen(Qt::cyan));
    c4Demand->attach(this);

    c5Demand = new QwtPlotCurve(name[4]);
    c5Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Demand->setPen(QPen(Qt::magenta));
    c5Demand->attach(this);

    c6Demand = new QwtPlotCurve(name[3]);
    c6Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Demand->setPen(QPen(Qt::yellow));
    c6Demand->attach(this);

    c7Demand = new QwtPlotCurve(name[2]);
    c7Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c7Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c7Demand->setPen(QPen(Qt::gray));
    c7Demand->attach(this);

    c8Demand = new QwtPlotCurve(name[1]);
    c8Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c8Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c8Demand->setPen(QPen(Qt::black));
    c8Demand->attach(this);

    c9Demand = new QwtPlotCurve(name[0]);
    c9Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c9Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c9Demand->setPen(QPen(Qt::darkRed));
    c9Demand->attach(this);
}

void PlotDemand::populateTen(const QVector <QString> &name)
{
    // Insert new curves
    c1Demand = new QwtPlotCurve(name[9]);
    c1Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Demand->setPen(QPen(Qt::red));
    c1Demand->attach(this);

    c2Demand = new QwtPlotCurve(name[8]);
    c2Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Demand->setPen(QPen(Qt::green));
    c2Demand->attach(this);

    c3Demand = new QwtPlotCurve(name[7]);
    c3Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Demand->setPen(QPen(Qt::blue));
    c3Demand->attach(this);

    c4Demand = new QwtPlotCurve(name[6]);
    c4Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Demand->setPen(QPen(Qt::cyan));
    c4Demand->attach(this);

    c5Demand = new QwtPlotCurve(name[5]);
    c5Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Demand->setPen(QPen(Qt::magenta));
    c5Demand->attach(this);

    c6Demand = new QwtPlotCurve(name[4]);
    c6Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Demand->setPen(QPen(Qt::yellow));
    c6Demand->attach(this);

    c7Demand = new QwtPlotCurve(name[3]);
    c7Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c7Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c7Demand->setPen(QPen(Qt::gray));
    c7Demand->attach(this);

    c8Demand = new QwtPlotCurve(name[2]);
    c8Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c8Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c8Demand->setPen(QPen(Qt::black));
    c8Demand->attach(this);

    c9Demand = new QwtPlotCurve(name[1]);
    c9Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c9Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c9Demand->setPen(QPen(Qt::darkRed));
    c9Demand->attach(this);

    c10Demand = new QwtPlotCurve(name[0]);
    c10Demand->setRenderHint(QwtPlotItem::RenderAntialiased);
    c10Demand->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c10Demand->setPen(QPen(Qt::darkGreen));
    c10Demand->attach(this);
}

void PlotDemand::whenDemandZoom(const QRectF & rect)
{
//	double xBegin;
//	double xEnd;
//	QwtScaleDiv scaleDiv =  this->map();
//	setAxisAutoScale(xBottom);
//	zoomer->setZoomBase(rect);
//	zoomer->zoomBase();
//	qDebug("Plot2D::autoRescale called!");
	if( demandZoomer->zoomRectIndex() == 0 ) // autorescale only if you are at base...
	{	
		this->setAxisScaleDiv( xBottom, yearScaleDiv() );
		this->setAxisScaleDraw( xBottom, new DemandYearScaleDraw() );

		this->setAxisAutoScale(xBottom, false );
		this->setAxisAutoScale(yLeft, true);
		this->updateAxes();
		demandZoomer->setZoomBase();
	}
//zoom rect dimension
//	xBegin = zoomer->zoomRect().x();// this->yearScaleDiv().lowerBound();
//    xEnd = zoomer->zoomRect().x()+zoomer->zoomRect().width();
}

