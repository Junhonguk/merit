#include "plot_supply.h"
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
class SupplyYearScaleDraw: public QwtScaleDraw
{
public:
    SupplyYearScaleDraw()
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
PlotSupply::PlotSupply(QWidget *parent):
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
    setAxisScaleDraw( xBottom, new SupplyYearScaleDraw() );

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

PlotSupply::~PlotSupply()
{

}

void PlotSupply::populate()
{
    // Insert new curves
	cSupply->detach();
	c1Supply->detach();
	c2Supply->detach();
	c3Supply->detach();
	c4Supply->detach();
	c5Supply->detach();
	c6Supply->detach();
	c7Supply->detach();
	c8Supply->detach();

    cSupply = new QwtPlotCurve("Supply Profile");
    cSupply->setRenderHint(QwtPlotItem::RenderAntialiased);
    cSupply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    cSupply->setPen(QPen(Qt::red));
    cSupply->attach(this);
}

void PlotSupply::populateSingle()
{
    cSupply = new QwtPlotCurve("Supply Profile");
    cSupply->setRenderHint(QwtPlotItem::RenderAntialiased);
    cSupply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    cSupply->setPen(QPen(Qt::red));
    cSupply->attach(this);
}

//check if the year is leap year and then sepcify the different set of days to the specific year
QwtScaleDiv PlotSupply::yearScaleDiv() const
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

void PlotSupply::drawSupplyGraph(const QString& name, const QVector<float> &supply)
{
    //initialise the plot settings
	PlotSupply();

    // Set up supply data
	const int numHrs = 8760; //hourly data for the whole year

    QVector<QPointF> supplyData(numHrs);

	for ( int i = 0; i < numHrs; i++ )
    {
		supplyData[i] = QPointF( double( i ), supply[i]);
//		qDebug("%4.3f",temperatureData[i].y());
    }

//set up the curve for the demand
    supplyCurve = new QwtPlotCurve(name);
    supplyCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    supplyCurve->setStyle( QwtPlotCurve::Lines );
	supplyCurve->setPen( QPen( Qt::green ) );
    supplyCurve->setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
	supplyCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine);

    QwtSymbol *symbol1 = new QwtSymbol( QwtSymbol::Ellipse );
    symbol1->setSize( 4 );
    symbol1->setPen( QPen( Qt::green ) );
	symbol1->setBrush(QBrush(Qt::black));
    supplyCurve->setSymbol( symbol1 );
    supplyCurve->setSamples( supplyData );
	supplyCurve->attach( this );

//enable zooming functionality
    supplyZoomer = new QwtPlotZoomer( QwtPlot::xBottom, QwtPlot::yLeft, this->canvas() );
	supplyZoomer->setZoomBase();
    supplyZoomer->setRubberBandPen( QColor( Qt::black ) );
    supplyZoomer->setTrackerPen( QColor( Qt::black ) );
    supplyZoomer->setMousePattern( QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier );
    supplyZoomer->setMousePattern( QwtEventPattern::MouseSelect3,
        Qt::RightButton );

	connect(supplyZoomer, SIGNAL(zoomed(const QRectF &)), this, SLOT(whenSupplyZoom(const QRectF &)));
    panner = new QwtPlotPanner( canvas() );
    panner->setMouseButton( Qt::MidButton );

    // axes 
	QwtText title("Year");
    title.setFont(QFont("Ms Shell Dlg 2", 10, QFont::Bold));
    setAxisTitle(xBottom, title);  

	setAxisScaleDiv( xBottom, yearScaleDiv() );
    setAxisScaleDraw( xBottom, new SupplyYearScaleDraw() );

//set the scale of y axis update automatically
	setAxisAutoScale(yLeft, true);

	//redraw the graph
	replot();
}

void PlotSupply::drawMultipleSupplysGraph(const QVector <QString> &name, const QVector< QVector <float> > &supply)
{	
	m_NoOfSupplys = 0;
	m_NoOfSupplys = supply.size();

	m_SupplyNames = name;

//	PlotSupply();
	cSupply->detach();

	setAutoFillBackground( true );
    setPalette( QPalette( QColor( 165, 193, 228 ) ) );
    updateGradient();

    setTitle("Supply");
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

//	PlotSupply();
    //initialise the plot settings
    double min_value = 1000.0;
	double max_value = 0.0;
//    setAxisScale(yLeft, supply.min(), supply.max());

    // Set up Supply data
	const int numHrs = 8760;
    QVector< QVector<QPointF> > supplyData;
	//Allocate memory
	supplyData.clear();
	supplyData.reserve(supply.size());
	supplyData.resize(supply.size());

	for (int k = 0; k < supply.size(); k++)
	{
	  supplyData[k].clear();
	  supplyData[k].reserve(8760);
	  supplyData[k].resize(8760);
	}

	int NOofSupplys = 0;
	NOofSupplys = supply.size();

    switch (NOofSupplys)
	{
	case 0:
		break;
	case 1:
		populateOne(name);
		for (int j = 0; j < NOofSupplys; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (supply[1][i] < min_value)
					min_value = supply[1][i];

				if (supply[1][i] > max_value)
					max_value = supply[1][i];

				supplyData[j][i] = QPointF( double( i ), supply[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Supply->setSamples(supplyData[0]);

		//redraw the graph
		replot();
		break;
	case 2:
		c1Supply->detach();
		c2Supply->detach();
		c3Supply->detach();
		c4Supply->detach();
		c5Supply->detach();
		c6Supply->detach();
		c7Supply->detach();
		c8Supply->detach();
		populateTwo(name);
		for (int j = 0; j < NOofSupplys; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (supply[1][i] < min_value)
					min_value = supply[1][i];

				if (supply[1][i] > max_value)
					max_value = supply[1][i];

				supplyData[j][i] = QPointF( double( i ), supply[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Supply->setSamples(supplyData[0]);
		c2Supply->setSamples(supplyData[1]);
		//redraw the graph
		replot();
//		setAutoReplot(true);

		break;
	case 3:
		c1Supply->detach();
		c2Supply->detach();
		c3Supply->detach();
		c4Supply->detach();
		c5Supply->detach();
		c6Supply->detach();
		c7Supply->detach();
		c8Supply->detach();
		populateThree(name);
		for (int j = 0; j < NOofSupplys; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (supply[2][i] < min_value)
					min_value = supply[2][i];

				if (supply[2][i] > max_value)
					max_value = supply[2][i];

				supplyData[j][i] = QPointF( double( i ), supply[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Supply->setSamples(supplyData[2]);
		c2Supply->setSamples(supplyData[1]);
		c3Supply->setSamples(supplyData[0]);
		//redraw the graph
		replot();
		break;
	case 4:
		c1Supply->detach();
		c2Supply->detach();
		c3Supply->detach();
		c4Supply->detach();
		c5Supply->detach();
		c6Supply->detach();
		c7Supply->detach();
		c8Supply->detach();
		populateFour(name);
		for (int j = 0; j < NOofSupplys; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (supply[3][i] < min_value)
					min_value = supply[3][i];

				if (supply[3][i] > max_value)
					max_value = supply[3][i];

				supplyData[j][i] = QPointF( double( i ), supply[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Supply->setSamples(supplyData[3]);
		c2Supply->setSamples(supplyData[2]);
		c3Supply->setSamples(supplyData[1]);
		c4Supply->setSamples(supplyData[0]);
		//redraw the graph
		replot();
		break;
	case 5:
		c1Supply->detach();
		c2Supply->detach();
		c3Supply->detach();
		c4Supply->detach();
		c5Supply->detach();
		c6Supply->detach();
		c7Supply->detach();
		c8Supply->detach();
		populateFive(name);
		for (int j = 0; j < NOofSupplys; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (supply[4][i] < min_value)
					min_value = supply[4][i];

				if (supply[4][i] > max_value)
					max_value = supply[4][i];

				supplyData[j][i] = QPointF( double( i ), supply[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Supply->setSamples(supplyData[4]);
		c2Supply->setSamples(supplyData[3]);
		c3Supply->setSamples(supplyData[2]);
		c4Supply->setSamples(supplyData[1]);
		c5Supply->setSamples(supplyData[0]);
		//redraw the graph
		replot();

		break;
	case 6:
		c1Supply->detach();
		c2Supply->detach();
		c3Supply->detach();
		c4Supply->detach();
		c5Supply->detach();
		c6Supply->detach();
		c7Supply->detach();
		c8Supply->detach();
		populateSix(name);
		for (int j = 0; j < NOofSupplys; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (supply[5][i] < min_value)
					min_value = supply[5][i];

				if (supply[5][i] > max_value)
					max_value = supply[5][i];

				supplyData[j][i] = QPointF( double( i ), supply[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Supply->setSamples(supplyData[5]);
		c2Supply->setSamples(supplyData[4]);
		c3Supply->setSamples(supplyData[3]);
		c4Supply->setSamples(supplyData[2]);
		c5Supply->setSamples(supplyData[1]);
		c6Supply->setSamples(supplyData[0]);
		//redraw the graph
		replot();
		break;
	case 7:
		c1Supply->detach();
		c2Supply->detach();
		c3Supply->detach();
		c4Supply->detach();
		c5Supply->detach();
		c6Supply->detach();
		c7Supply->detach();
		c8Supply->detach();
		populateSeven(name);
		for (int j = 0; j < NOofSupplys; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (supply[6][i] < min_value)
					min_value = supply[6][i];

				if (supply[6][i] > max_value)
					max_value = supply[6][i];

				supplyData[j][i] = QPointF( double( i ), supply[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Supply->setSamples(supplyData[6]);
		c2Supply->setSamples(supplyData[5]);
		c3Supply->setSamples(supplyData[4]);
		c4Supply->setSamples(supplyData[3]);
		c5Supply->setSamples(supplyData[2]);
		c6Supply->setSamples(supplyData[1]);
		c7Supply->setSamples(supplyData[0]);
		//redraw the graph
		replot();
		break;
	case 8:
		c1Supply->detach();
		c2Supply->detach();
		c3Supply->detach();
		c4Supply->detach();
		c5Supply->detach();
		c6Supply->detach();
		c7Supply->detach();
		c8Supply->detach();
		populateEight(name);
		for (int j = 0; j < NOofSupplys; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (supply[7][i] < min_value)
					min_value = supply[7][i];

				if (supply[7][i] > max_value)
					max_value = supply[7][i];

				supplyData[j][i] = QPointF( double( i ), supply[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Supply->setSamples(supplyData[7]);
		c2Supply->setSamples(supplyData[6]);
		c3Supply->setSamples(supplyData[5]);
		c4Supply->setSamples(supplyData[4]);
		c5Supply->setSamples(supplyData[3]);
		c6Supply->setSamples(supplyData[2]);
		c7Supply->setSamples(supplyData[1]);
		c8Supply->setSamples(supplyData[0]);
		//redraw the graph
		replot();
		break;
	case 9:
		populateNine(name);
		for (int j = 0; j < NOofSupplys; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (supply[8][i] < min_value)
					min_value = supply[8][i];

				if (supply[8][i] > max_value)
					max_value = supply[8][i];

				supplyData[j][i] = QPointF( double( i ), supply[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Supply->setSamples(supplyData[8]);
		c2Supply->setSamples(supplyData[7]);
		c3Supply->setSamples(supplyData[6]);
		c4Supply->setSamples(supplyData[5]);
		c5Supply->setSamples(supplyData[4]);
		c6Supply->setSamples(supplyData[3]);
		c7Supply->setSamples(supplyData[2]);
		c8Supply->setSamples(supplyData[1]);
		c9Supply->setSamples(supplyData[0]);
		//redraw the graph
		replot();
		break;
	case 10:
//		detach
		populateTen(name);
		for (int j = 0; j < NOofSupplys; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (supply[9][i] < min_value)
					min_value = supply[9][i];

				if (supply[9][i] > max_value)
					max_value = supply[9][i];

				supplyData[j][i] = QPointF( double( i ), supply[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Supply->setSamples(supplyData[9]);
		c2Supply->setSamples(supplyData[8]);
		c3Supply->setSamples(supplyData[7]);
		c4Supply->setSamples(supplyData[6]);
		c5Supply->setSamples(supplyData[5]);
		c6Supply->setSamples(supplyData[4]);
		c7Supply->setSamples(supplyData[3]);
		c8Supply->setSamples(supplyData[2]);
		c9Supply->setSamples(supplyData[1]);
		c10Supply->setSamples(supplyData[0]);
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

void PlotSupply::updateGradient()
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

void PlotSupply::resizeEvent( QResizeEvent *event )
{
    QwtPlot::resizeEvent( event );
#ifdef Q_WS_X11
    updateGradient();
#endif
}


void PlotSupply::populateOne(const QVector <QString> &name)
{
    // Insert new curves
	delete c1Supply;
    c1Supply = new QwtPlotCurve(name[0]);
    c1Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Supply->setPen(QPen(Qt::red));
    c1Supply->attach(this);
}

void PlotSupply::populateTwo(const QVector <QString> &name)
{
    // Insert new curves
    c1Supply = new QwtPlotCurve(name[1]);	
    c1Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Supply->setPen(QPen(Qt::red));
    c1Supply->attach(this);

	c2Supply = new QwtPlotCurve(name[0]);
    c2Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Supply->setPen(QPen(Qt::green));
    c2Supply->attach(this);
}

void PlotSupply::populateThree(const QVector <QString> &name)
{
    // Insert new curves
//	delete c1Supply;
//	delete c2Supply;
//	delete c3Supply;
    c1Supply = new QwtPlotCurve(name[2]);
    c1Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Supply->setPen(QPen(Qt::red));
    c1Supply->attach(this);

    c2Supply = new QwtPlotCurve(name[1]);
    c2Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Supply->setPen(QPen(Qt::green));
    c2Supply->attach(this);

    c3Supply = new QwtPlotCurve(name[0]);
    c3Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Supply->setPen(QPen(Qt::blue));
    c3Supply->attach(this);
}

void PlotSupply::populateFour(const QVector <QString> &name)
{
    // Insert new curves
//	delete c1Supply;
//	delete c2Supply;
//	delete c3Supply;
//	delete c4Supply;

    c1Supply = new QwtPlotCurve(name[3]);
    c1Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Supply->setPen(QPen(Qt::red));
    c1Supply->attach(this);

    c2Supply = new QwtPlotCurve(name[2]);
    c2Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Supply->setPen(QPen(Qt::green));
    c2Supply->attach(this);

    c3Supply = new QwtPlotCurve(name[1]);
    c3Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Supply->setPen(QPen(Qt::blue));
    c3Supply->attach(this);

    c4Supply = new QwtPlotCurve(name[0]);
    c4Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Supply->setPen(QPen(Qt::cyan));
    c4Supply->attach(this);
}

void PlotSupply::populateFive(const QVector <QString> &name)
{
    // Insert new curves
    c1Supply = new QwtPlotCurve(name[4]);
    c1Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Supply->setPen(QPen(Qt::red));
    c1Supply->attach(this);

    c2Supply = new QwtPlotCurve(name[3]);
    c2Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Supply->setPen(QPen(Qt::green));
    c2Supply->attach(this);

    c3Supply = new QwtPlotCurve(name[2]);
    c3Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Supply->setPen(QPen(Qt::blue));
    c3Supply->attach(this);

    c4Supply = new QwtPlotCurve(name[1]);
    c4Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Supply->setPen(QPen(Qt::cyan));
    c4Supply->attach(this);

    c5Supply = new QwtPlotCurve(name[0]);
    c5Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Supply->setPen(QPen(Qt::magenta));
    c5Supply->attach(this);
}

void PlotSupply::populateSix(const QVector <QString> &name)
{
    // Insert new curves
    c1Supply = new QwtPlotCurve(name[5]);
    c1Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Supply->setPen(QPen(Qt::red));
    c1Supply->attach(this);

    c2Supply = new QwtPlotCurve(name[4]);
    c2Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Supply->setPen(QPen(Qt::green));
    c2Supply->attach(this);

    c3Supply = new QwtPlotCurve(name[3]);
    c3Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Supply->setPen(QPen(Qt::blue));
    c3Supply->attach(this);

    c4Supply = new QwtPlotCurve(name[2]);
    c4Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Supply->setPen(QPen(Qt::cyan));
    c4Supply->attach(this);

    c5Supply = new QwtPlotCurve(name[1]);
    c5Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Supply->setPen(QPen(Qt::magenta));
    c5Supply->attach(this);

    c6Supply = new QwtPlotCurve(name[0]);
    c6Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Supply->setPen(QPen(Qt::yellow));
    c6Supply->attach(this);
}

void PlotSupply::populateSeven(const QVector <QString> &name)
{
    // Insert new curves
    c1Supply = new QwtPlotCurve(name[6]);
    c1Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Supply->setPen(QPen(Qt::red));
    c1Supply->attach(this);

    c2Supply = new QwtPlotCurve(name[5]);
    c2Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Supply->setPen(QPen(Qt::green));
    c2Supply->attach(this);

    c3Supply = new QwtPlotCurve(name[4]);
    c3Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Supply->setPen(QPen(Qt::blue));
    c3Supply->attach(this);

    c4Supply = new QwtPlotCurve(name[3]);
    c4Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Supply->setPen(QPen(Qt::cyan));
    c4Supply->attach(this);

    c5Supply = new QwtPlotCurve(name[2]);
    c5Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Supply->setPen(QPen(Qt::magenta));
    c5Supply->attach(this);

    c6Supply = new QwtPlotCurve(name[1]);
    c6Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Supply->setPen(QPen(Qt::yellow));
    c6Supply->attach(this);

    c7Supply = new QwtPlotCurve(name[0]);
    c7Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c7Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c7Supply->setPen(QPen(Qt::gray));
    c7Supply->attach(this);
}

void PlotSupply::populateEight(const QVector <QString> &name)
{
    // Insert new curves
    c1Supply = new QwtPlotCurve(name[7]);
    c1Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Supply->setPen(QPen(Qt::red));
    c1Supply->attach(this);

    c2Supply = new QwtPlotCurve(name[6]);
    c2Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Supply->setPen(QPen(Qt::green));
    c2Supply->attach(this);

    c3Supply = new QwtPlotCurve(name[5]);
    c3Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Supply->setPen(QPen(Qt::blue));
    c3Supply->attach(this);

    c4Supply = new QwtPlotCurve(name[4]);
    c4Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Supply->setPen(QPen(Qt::cyan));
    c4Supply->attach(this);

    c5Supply = new QwtPlotCurve(name[3]);
    c5Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Supply->setPen(QPen(Qt::magenta));
    c5Supply->attach(this);

    c6Supply = new QwtPlotCurve(name[2]);
    c6Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Supply->setPen(QPen(Qt::yellow));
    c6Supply->attach(this);

    c7Supply = new QwtPlotCurve(name[1]);
    c7Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c7Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c7Supply->setPen(QPen(Qt::gray));
    c7Supply->attach(this);

    c8Supply = new QwtPlotCurve(name[0]);
    c8Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c8Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c8Supply->setPen(QPen(Qt::black));
    c8Supply->attach(this);
}

void PlotSupply::populateNine(const QVector <QString> &name)
{
    // Insert new curves
    // Insert new curves
    c1Supply = new QwtPlotCurve(name[8]);
    c1Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Supply->setPen(QPen(Qt::red));
    c1Supply->attach(this);

    c2Supply = new QwtPlotCurve(name[7]);
    c2Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Supply->setPen(QPen(Qt::green));
    c2Supply->attach(this);

    c3Supply = new QwtPlotCurve(name[6]);
    c3Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Supply->setPen(QPen(Qt::blue));
    c3Supply->attach(this);

    c4Supply = new QwtPlotCurve(name[5]);
    c4Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Supply->setPen(QPen(Qt::cyan));
    c4Supply->attach(this);

    c5Supply = new QwtPlotCurve(name[4]);
    c5Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Supply->setPen(QPen(Qt::magenta));
    c5Supply->attach(this);

    c6Supply = new QwtPlotCurve(name[3]);
    c6Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Supply->setPen(QPen(Qt::yellow));
    c6Supply->attach(this);

    c7Supply = new QwtPlotCurve(name[2]);
    c7Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c7Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c7Supply->setPen(QPen(Qt::gray));
    c7Supply->attach(this);

    c8Supply = new QwtPlotCurve(name[1]);
    c8Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c8Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c8Supply->setPen(QPen(Qt::black));
    c8Supply->attach(this);

    c9Supply = new QwtPlotCurve(name[0]);
    c9Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c9Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c9Supply->setPen(QPen(Qt::darkRed));
    c9Supply->attach(this);
}

void PlotSupply::populateTen(const QVector <QString> &name)
{
    // Insert new curves
    c1Supply = new QwtPlotCurve(name[9]);
    c1Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Supply->setPen(QPen(Qt::red));
    c1Supply->attach(this);

    c2Supply = new QwtPlotCurve(name[8]);
    c2Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Supply->setPen(QPen(Qt::green));
    c2Supply->attach(this);

    c3Supply = new QwtPlotCurve(name[7]);
    c3Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Supply->setPen(QPen(Qt::blue));
    c3Supply->attach(this);

    c4Supply = new QwtPlotCurve(name[6]);
    c4Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Supply->setPen(QPen(Qt::cyan));
    c4Supply->attach(this);

    c5Supply = new QwtPlotCurve(name[5]);
    c5Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Supply->setPen(QPen(Qt::magenta));
    c5Supply->attach(this);

    c6Supply = new QwtPlotCurve(name[4]);
    c6Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Supply->setPen(QPen(Qt::yellow));
    c6Supply->attach(this);

    c7Supply = new QwtPlotCurve(name[3]);
    c7Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c7Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c7Supply->setPen(QPen(Qt::gray));
    c7Supply->attach(this);

    c8Supply = new QwtPlotCurve(name[2]);
    c8Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c8Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c8Supply->setPen(QPen(Qt::black));
    c8Supply->attach(this);

    c9Supply = new QwtPlotCurve(name[1]);
    c9Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c9Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c9Supply->setPen(QPen(Qt::darkRed));
    c9Supply->attach(this);

    c10Supply = new QwtPlotCurve(name[0]);
    c10Supply->setRenderHint(QwtPlotItem::RenderAntialiased);
    c10Supply->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c10Supply->setPen(QPen(Qt::darkGreen));
    c10Supply->attach(this);
}

void PlotSupply::whenSupplyZoom(const QRectF & rect)
{
//	double xBegin;
//	double xEnd;
//	QwtScaleDiv scaleDiv =  this->map();
//	setAxisAutoScale(xBottom);
//	zoomer->setZoomBase(rect);
//	zoomer->zoomBase();
//	qDebug("Plot2D::autoRescale called!");
	if( supplyZoomer->zoomRectIndex() == 0 ) // autorescale only if you are at base...
	{	
		this->setAxisScaleDiv( xBottom, yearScaleDiv() );
		this->setAxisScaleDraw( xBottom, new SupplyYearScaleDraw() );

		this->setAxisAutoScale(xBottom, false );
		this->setAxisAutoScale(yLeft, true);
		this->updateAxes();
		supplyZoomer->setZoomBase();
	}
//zoom rect dimension
//	xBegin = zoomer->zoomRect().x();// this->yearScaleDiv().lowerBound();
//    xEnd = zoomer->zoomRect().x()+zoomer->zoomRect().width();
}
