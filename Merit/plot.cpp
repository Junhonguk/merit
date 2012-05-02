#include "plot.h"
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_intervalcurve.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
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
class YearScaleDraw: public QwtScaleDraw
{
public:
    YearScaleDraw()
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

//when the period defined is in the range of (24~744) 
class MonthScaleDraw: public QwtScaleDraw
{
public:
    MonthScaleDraw()
    {
        setTickLength( QwtScaleDiv::MajorTick, 0 );
        setTickLength( QwtScaleDiv::MinorTick, 0 );
        setTickLength( QwtScaleDiv::MediumTick, 6 );

        setLabelRotation( -60.0 );
        setLabelAlignment( Qt::AlignLeft | Qt::AlignVCenter );

        setSpacing( 5 );
    }

    virtual QwtText label( QDate date ) const
    {
/*
		QwtText title = QDate::longDayName( date.dayOfWeek() );
		title.setFont(QFont("Ms Shell Dlg 2", 8));
		return title;
*/
        return QDate::longDayName( date.dayOfWeek() ); //return the day of week when the month is selected

    }
};

//constructor for the weather plot
Plot::Plot(QWidget *parent):
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

//set up checkable legends
    QwtLegend *legend = new QwtLegend;
    legend->setItemMode(QwtLegend::CheckableItem);
    insertLegend(legend, QwtPlot::RightLegend);

    // axes 
//    setAxisTitle( xBottom, "Year" ); //set the year data to this and show as the title for x 
	QwtText titleX("Year");
    titleX.setFont(QFont("Ms Shell Dlg 2", 10, QFont::Bold));
    setAxisTitle(xBottom, titleX);

 //   setAxisScale(xBottom, 0.0, 8760.0);    
	setAxisScaleDiv( xBottom, yearScaleDiv() );
    setAxisScaleDraw( xBottom, new YearScaleDraw() );

//    setAxisTitle(xBottom, "Time (hrs)" );
	QwtText titleY("Weather data");
    titleY.setFont(QFont("Ms Shell Dlg 2", 10, QFont::Bold));
    setAxisTitle(yLeft, titleY);

//    setAxisTitle(yLeft, "Weather parameters");
//    setAxisScale(yLeft, -10.0, 1000.0);


    // canvas
    canvas()->setLineWidth( 1 );
    canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas()->setBorderRadius( 10 );

    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas()->setPalette( canvasPalette );

 //   populate();
    connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),
        SLOT(showCurve(QwtPlotItem *, bool)));

}
Plot::~Plot()
{
//	delete temperatureCurve;
//	delete directSolarCurve;
}

//check if the year is leap year and then sepcify the different set of days to the specific year
QwtScaleDiv Plot::yearScaleDiv() const
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

void Plot::drawGraph(const QString& site, const QVector<float> &DbT, const QVector<float> &DrS, const QVector<float> &DfS,
		           const QVector<float> &WS, const QVector<float> &WD, const QVector<float> &RH)
{
    //initialise the plot settings
	Plot();

    // Set up weather data
	const int numHrs = 8760; //hourly data for the whole year

    QVector<QPointF> temperatureData(numHrs);
    QVector<QPointF> directSolarData(numHrs);
    QVector<QPointF> diffuseSolarData(numHrs);
    QVector<QPointF> windSpeedData(numHrs);
    QVector<QPointF> windDirectionData(numHrs);
    QVector<QPointF> relativeHumidityData(numHrs);

	for ( int i = 0; i < numHrs; i++ )
    {
		temperatureData[i] = QPointF( double( i ), DbT[i]);
        directSolarData[i] = QPointF( double( i ), DrS[i]);
        diffuseSolarData[i] = QPointF( double( i ), DfS[i]);
		windSpeedData[i] = QPointF( double( i ), WS[i]);
        windDirectionData[i] = QPointF( double( i ), WD[i]);
        relativeHumidityData[i] = QPointF( double( i ), RH[i]);
//		qDebug("%4.3f",temperatureData[i].y());
    }

//set up the curve for the temperature
    temperatureCurve = new QwtPlotCurve(QString( "Temperature [%1C]" ).arg( QChar( 0x00B0 ) ));

    temperatureCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    temperatureCurve->setStyle( QwtPlotCurve::Lines );
	temperatureCurve->setPen( QPen( Qt::black ) );
    temperatureCurve->setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
	temperatureCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine);

    QwtSymbol *symbol1 = new QwtSymbol( QwtSymbol::Ellipse );
    symbol1->setSize( 4 );
    symbol1->setPen( QPen( Qt::black ) );
	symbol1->setBrush(QBrush(Qt::white));
    temperatureCurve->setSymbol( symbol1 );
    temperatureCurve->setSamples( temperatureData );
	temperatureCurve->attach( this );

//set up the curve for the direct solar
    directSolarCurve = new QwtPlotCurve("Direct solar [W/m<sup>2</sup>]");

    directSolarCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    directSolarCurve->setStyle( QwtPlotCurve::Lines );
	directSolarCurve->setPen( QPen( Qt::red ) );
    directSolarCurve->setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
	directSolarCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine);

    QwtSymbol *symbol2 = new QwtSymbol( QwtSymbol::Rect );
    symbol2->setSize( 4 );
    symbol2->setPen( QPen( Qt::red ) );
	symbol2->setBrush(QBrush(Qt::white));
    directSolarCurve->setSymbol( symbol2 );
    directSolarCurve->setSamples( directSolarData );
	directSolarCurve->attach( this );

//set up the curve for the diffuse solar
    diffuseSolarCurve = new QwtPlotCurve("Diffuse solar [W/m<sup>2</sup>]");

    diffuseSolarCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    diffuseSolarCurve->setStyle( QwtPlotCurve::Lines );
	diffuseSolarCurve->setPen( QPen( Qt::yellow ) );
    diffuseSolarCurve->setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
	diffuseSolarCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine);

    QwtSymbol *symbol3 = new QwtSymbol( QwtSymbol::Triangle );
    symbol3->setSize( 4 );
    symbol3->setPen( QPen( Qt::yellow ) );
//	symbol3->setBrush(QBrush(Qt::white));
    diffuseSolarCurve->setSymbol( symbol3 );
    diffuseSolarCurve->setSamples( diffuseSolarData );
	diffuseSolarCurve->attach( this );
/////////////////////////////////////////////

//set up the curve for the temperature
    windSpeedCurve = new QwtPlotCurve("Wind speed [m/s]");

    windSpeedCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    windSpeedCurve->setStyle( QwtPlotCurve::Lines );
	windSpeedCurve->setPen( QPen( Qt::blue ) );
    windSpeedCurve->setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
	windSpeedCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine);

    QwtSymbol *symbol4 = new QwtSymbol( QwtSymbol::Cross );
    symbol4->setSize( 4 );
    symbol4->setPen( QPen( Qt::blue ) );
//	symbol4->setBrush(QBrush(Qt::white));
    windSpeedCurve->setSymbol( symbol4 );
    windSpeedCurve->setSamples( windSpeedData );
	windSpeedCurve->attach( this );

//set up the curve for the wind direction
    windDirectionCurve = new QwtPlotCurve(QString("Wind direction [%1]" ).arg( QChar( 0x00B0 ) ));

    windDirectionCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    windDirectionCurve->setStyle( QwtPlotCurve::Lines );
	windDirectionCurve->setPen( QPen( Qt::cyan) );
    windDirectionCurve->setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
	windDirectionCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine);

    QwtSymbol *symbol5 = new QwtSymbol( QwtSymbol::Diamond );
    symbol5->setSize( 4 );
    symbol5->setPen( QPen( Qt::cyan ) );
//	symbol5->setBrush(QBrush(Qt::white));
    windDirectionCurve->setSymbol( symbol5 );
    windDirectionCurve->setSamples( windDirectionData );
	windDirectionCurve->attach( this );

//set up the curve for the relative humidity
    relativeHumidityCurve = new QwtPlotCurve("Relative humidity [%]");

    relativeHumidityCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    relativeHumidityCurve->setStyle( QwtPlotCurve::Lines );
	relativeHumidityCurve->setPen( QPen( Qt::green ) );
    relativeHumidityCurve->setLegendAttribute( QwtPlotCurve::LegendShowSymbol);
	relativeHumidityCurve->setLegendAttribute( QwtPlotCurve::LegendShowLine);

    QwtSymbol *symbol6 = new QwtSymbol( QwtSymbol::Star2 );
    symbol6->setSize( 4 );
    symbol6->setPen( QPen( Qt::green ) );
//	symbol6->setBrush(QBrush(Qt::white));
    relativeHumidityCurve->setSymbol( symbol6 );
    relativeHumidityCurve->setSamples( relativeHumidityData );
	relativeHumidityCurve->attach( this );

///////////////////////////////////////////////
	showCurve(temperatureCurve, true);
	showCurve(directSolarCurve, false);
	showCurve(diffuseSolarCurve, false);
	showCurve(windSpeedCurve, false);
	showCurve(windDirectionCurve, false);
	showCurve(relativeHumidityCurve, false);

//	//set up the data for display
//	insertCurve( "Temperature", temperatureData, Qt::black );
//	insertCurve( "Direct solar", directsolarData, Qt::red);
	
//    cTemperature->setSamples(temperatureData);
//	cDirectSolar->setSamples(directsolarData);
    // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size

    zoomer = new QwtPlotZoomer( QwtPlot::xBottom, QwtPlot::yLeft, this->canvas() );
	zoomer->setZoomBase();
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
        Qt::RightButton );

//	originalRect = zoomer->zoomBase();

//    zoomerPrev = zoomer;
	connect(zoomer, SIGNAL(zoomed(const QRectF &)), this, SLOT(whenZoom(const QRectF &)));
    panner = new QwtPlotPanner( canvas() );
    panner->setMouseButton( Qt::MidButton );

    // axes 
	QwtText title("Year");
    title.setFont(QFont("Ms Shell Dlg 2", 10, QFont::Bold));
    setAxisTitle(xBottom, title);  

	setAxisScaleDiv( xBottom, yearScaleDiv() );
    setAxisScaleDraw( xBottom, new YearScaleDraw() );

	//redraw the graph
	replot();
}

void Plot::updateGradient()
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

void Plot::resizeEvent( QResizeEvent *event )
{
    QwtPlot::resizeEvent( event );
#ifdef Q_WS_X11
    updateGradient();
#endif
}

void Plot::whenZoom(const QRectF & rect)
{
//	double xBegin;
//	double xEnd;
//	QwtScaleDiv scaleDiv =  this->map();
//	setAxisAutoScale(xBottom);
//	zoomer->setZoomBase(rect);
//	zoomer->zoomBase();
//	qDebug("Plot2D::autoRescale called!");
	if( zoomer->zoomRectIndex() == 0 ) // autorescale only if you are at base...
	{	
//		this->setAxisScaleDiv( xBottom, yearScaleDiv() );
//		this->setAxisScaleDraw( xBottom, new YearScaleDraw() );

		this->setAxisAutoScale(xBottom, false );
		this->setAxisAutoScale(yLeft, true);
		this->updateAxes();
		zoomer->setZoomBase();
	}
//zoom rect dimension
//	xBegin = zoomer->zoomRect().x();// this->yearScaleDiv().lowerBound();
//    xEnd = zoomer->zoomRect().x()+zoomer->zoomRect().width();
}

void Plot::showCurve(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
    QWidget *w = legend()->find(item);
    if ( w && w->inherits("QwtLegendItem") )
        ((QwtLegendItem *)w)->setChecked(on);
    
    replot();
}