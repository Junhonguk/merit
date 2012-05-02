#include "plot_match.h"
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

PlotMatch::PlotMatch(QWidget *parent):
    QwtPlot( parent )
{
    // panning with the left mouse button
//    (void) new QwtPlotPanner( canvas() );

    // zoom in/out with the wheel
  //  (void) new QwtPlotMagnifier( canvas() );

    setAutoFillBackground( true );
    setPalette( QPalette( QColor( 165, 193, 228 ) ) );
    updateGradient();

    setTitle("Supply profile");
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

	cMatch = new QwtPlotCurve("Supply Profile");
    cMatch->attach(this);

	c1Match = new QwtPlotCurve("Supply");
    c1Match->attach(this);

	c2Match = new QwtPlotCurve("Supply");
    c2Match->attach(this);

	c3Match = new QwtPlotCurve("Supply");
    c3Match->attach(this);

	c4Match = new QwtPlotCurve("Supply");
    c4Match->attach(this);

	c5Match = new QwtPlotCurve("Supply");
    c5Match->attach(this);

	c6Match = new QwtPlotCurve("Supply");
    c6Match->attach(this);

	c7Match = new QwtPlotCurve("Supply");
    c7Match->attach(this);

	c8Match = new QwtPlotCurve("Supply");
    c8Match->attach(this);

	populate();
}




PlotMatch::~PlotMatch()
{
//	delete cMatch;
//	delete c1Match;
//	delete c2Match;
//	delete c3Match;
//	delete c4Match;
//	delete c5Match;
//	delete c6Match;
//	delete c7Match;
//	delete c8Match;
//	delete c9Supply;
//	delete c10Supply;
}
void PlotMatch::populate()
{
    // Insert new curves
	cMatch->detach();
	c1Match->detach();
	c2Match->detach();
	c3Match->detach();
	c4Match->detach();
	c5Match->detach();
	c6Match->detach();
	c7Match->detach();
	c8Match->detach();

    cMatch = new QwtPlotCurve("Supply Profile");
    cMatch->setRenderHint(QwtPlotItem::RenderAntialiased);
    cMatch->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    cMatch->setPen(QPen(Qt::red));
    cMatch->attach(this);
}

void PlotMatch::populateSingle()
{
    cMatch = new QwtPlotCurve("Supply Profile");
    cMatch->setRenderHint(QwtPlotItem::RenderAntialiased);
    cMatch->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    cMatch->setPen(QPen(Qt::red));
    cMatch->attach(this);
}

void PlotMatch::drawMatchGraph(const QString& name, const QVector<float> &match)
{
    //initialise the plot settings
//	PlotMatch();
	cMatch->detach();
	c1Match->detach();
	c2Match->detach();
	c3Match->detach();
	c4Match->detach();
	c5Match->detach();
	c6Match->detach();
	c7Match->detach();
	c8Match->detach();

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

	populateSingle();
//	populate();
    double min_value = 1000.0;
	double max_value = 0.0;
//    setAxisScale(yLeft, match.min(), match.max());

    // Set up Supply data
	const int numHrs = 8760;
    QVector<QPointF> matchData(8760);

	for ( int i = 0; i < numHrs; i++ )
    {
		if (match[i] < min_value)
			min_value = match[i];

		if (match[i] > max_value)
			max_value = match[i];

		matchData[i] = QPointF( double( i ), match[i]);
    }

	//set up the data for display
	setAxisScale(yLeft, min_value, max_value);
    cMatch->setSamples(matchData);

	//redraw the graph
	replot();
}

void PlotMatch::drawMultipleMatchesGraph(const QVector <QString> &name, const QVector< QVector <float> > &match)
{	
	m_NoOfMatches = 0;
	m_NoOfMatches = match.size();

	m_MatchNames = name;

//	PlotMatch();
	cMatch->detach();

	setAutoFillBackground( true );
    setPalette( QPalette( QColor( 165, 193, 228 ) ) );
    updateGradient();

    setTitle("Match");
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

//	PlotMatch();
    //initialise the plot settings
    double min_value = 1000.0;
	double max_value = 0.0;
//    setAxisScale(yLeft, match.min(), match.max());

    // Set up Supply data
	const int numHrs = 8760;
    QVector< QVector<QPointF> > matchData;
	//Allocate memory
	matchData.clear();
	matchData.reserve(match.size());
	matchData.resize(match.size());

	for (int k = 0; k < match.size(); k++)
	{
	  matchData[k].clear();
	  matchData[k].reserve(8760);
	  matchData[k].resize(8760);
	}

	int NOofMatches = 0;
	NOofMatches = match.size();

    switch (NOofMatches)
	{
	case 0:
		break;
	case 1:
		populateOne(name);
		for (int j = 0; j < NOofMatches; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (match[1][i] < min_value)
					min_value = match[1][i];

				if (match[1][i] > max_value)
					max_value = match[1][i];

				matchData[j][i] = QPointF( double( i ), match[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Match->setSamples(matchData[0]);

		//redraw the graph
		replot();
		break;
	case 2:
		c1Match->detach();
		c2Match->detach();
		c3Match->detach();
		c4Match->detach();
		c5Match->detach();
		c6Match->detach();
		c7Match->detach();
		c8Match->detach();
		populateTwo(name);
		for (int j = 0; j < NOofMatches; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (match[1][i] < min_value)
					min_value = match[1][i];

				if (match[1][i] > max_value)
					max_value = match[1][i];

				matchData[j][i] = QPointF( double( i ), match[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Match->setSamples(matchData[0]);
		c2Match->setSamples(matchData[1]);
		//redraw the graph
		replot();
//		setAutoReplot(true);

		break;
	case 3:
		c1Match->detach();
		c2Match->detach();
		c3Match->detach();
		c4Match->detach();
		c5Match->detach();
		c6Match->detach();
		c7Match->detach();
		c8Match->detach();
		populateThree(name);
		for (int j = 0; j < NOofMatches; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (match[2][i] < min_value)
					min_value = match[2][i];

				if (match[2][i] > max_value)
					max_value = match[2][i];

				matchData[j][i] = QPointF( double( i ), match[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Match->setSamples(matchData[2]);
		c2Match->setSamples(matchData[1]);
		c3Match->setSamples(matchData[0]);
		//redraw the graph
		replot();
		break;
	case 4:
		c1Match->detach();
		c2Match->detach();
		c3Match->detach();
		c4Match->detach();
		c5Match->detach();
		c6Match->detach();
		c7Match->detach();
		c8Match->detach();
		populateFour(name);
		for (int j = 0; j < NOofMatches; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (match[3][i] < min_value)
					min_value = match[3][i];

				if (match[3][i] > max_value)
					max_value = match[3][i];

				matchData[j][i] = QPointF( double( i ), match[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Match->setSamples(matchData[3]);
		c2Match->setSamples(matchData[2]);
		c3Match->setSamples(matchData[1]);
		c4Match->setSamples(matchData[0]);
		//redraw the graph
		replot();
		break;
	case 5:
		c1Match->detach();
		c2Match->detach();
		c3Match->detach();
		c4Match->detach();
		c5Match->detach();
		c6Match->detach();
		c7Match->detach();
		c8Match->detach();
		populateFive(name);
		for (int j = 0; j < NOofMatches; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (match[4][i] < min_value)
					min_value = match[4][i];

				if (match[4][i] > max_value)
					max_value = match[4][i];

				matchData[j][i] = QPointF( double( i ), match[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Match->setSamples(matchData[4]);
		c2Match->setSamples(matchData[3]);
		c3Match->setSamples(matchData[2]);
		c4Match->setSamples(matchData[1]);
		c5Match->setSamples(matchData[0]);
		//redraw the graph
		replot();

		break;
	case 6:
		c1Match->detach();
		c2Match->detach();
		c3Match->detach();
		c4Match->detach();
		c5Match->detach();
		c6Match->detach();
		c7Match->detach();
		c8Match->detach();
		populateSix(name);
		for (int j = 0; j < NOofMatches; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (match[5][i] < min_value)
					min_value = match[5][i];

				if (match[5][i] > max_value)
					max_value = match[5][i];

				matchData[j][i] = QPointF( double( i ), match[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Match->setSamples(matchData[5]);
		c2Match->setSamples(matchData[4]);
		c3Match->setSamples(matchData[3]);
		c4Match->setSamples(matchData[2]);
		c5Match->setSamples(matchData[1]);
		c6Match->setSamples(matchData[0]);
		//redraw the graph
		replot();
		break;
	case 7:
		c1Match->detach();
		c2Match->detach();
		c3Match->detach();
		c4Match->detach();
		c5Match->detach();
		c6Match->detach();
		c7Match->detach();
		c8Match->detach();
		populateSeven(name);
		for (int j = 0; j < NOofMatches; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (match[6][i] < min_value)
					min_value = match[6][i];

				if (match[6][i] > max_value)
					max_value = match[6][i];

				matchData[j][i] = QPointF( double( i ), match[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Match->setSamples(matchData[6]);
		c2Match->setSamples(matchData[5]);
		c3Match->setSamples(matchData[4]);
		c4Match->setSamples(matchData[3]);
		c5Match->setSamples(matchData[2]);
		c6Match->setSamples(matchData[1]);
		c7Match->setSamples(matchData[0]);
		//redraw the graph
		replot();
		break;
	case 8:
		c1Match->detach();
		c2Match->detach();
		c3Match->detach();
		c4Match->detach();
		c5Match->detach();
		c6Match->detach();
		c7Match->detach();
		c8Match->detach();
		populateEight(name);
		for (int j = 0; j < NOofMatches; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (match[7][i] < min_value)
					min_value = match[7][i];

				if (match[7][i] > max_value)
					max_value = match[7][i];

				matchData[j][i] = QPointF( double( i ), match[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Match->setSamples(matchData[7]);
		c2Match->setSamples(matchData[6]);
		c3Match->setSamples(matchData[5]);
		c4Match->setSamples(matchData[4]);
		c5Match->setSamples(matchData[3]);
		c6Match->setSamples(matchData[2]);
		c7Match->setSamples(matchData[1]);
		c8Match->setSamples(matchData[0]);
		//redraw the graph
		replot();
		break;
	case 9:
		populateNine(name);
		for (int j = 0; j < NOofMatches; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (match[8][i] < min_value)
					min_value = match[8][i];

				if (match[8][i] > max_value)
					max_value = match[8][i];

				matchData[j][i] = QPointF( double( i ), match[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Match->setSamples(matchData[8]);
		c2Match->setSamples(matchData[7]);
		c3Match->setSamples(matchData[6]);
		c4Match->setSamples(matchData[5]);
		c5Match->setSamples(matchData[4]);
		c6Match->setSamples(matchData[3]);
		c7Match->setSamples(matchData[2]);
		c8Match->setSamples(matchData[1]);
		c9Match->setSamples(matchData[0]);
		//redraw the graph
		replot();
		break;
	case 10:
//		detach
		populateTen(name);
		for (int j = 0; j < NOofMatches; j++)
		{
			for ( int i = 0; i < numHrs; i++ )
			{
				if (match[9][i] < min_value)
					min_value = match[9][i];

				if (match[9][i] > max_value)
					max_value = match[9][i];

				matchData[j][i] = QPointF( double( i ), match[j][i]);
			}
		}
		//set up the data for display
		setAxisScale(yLeft, min_value, max_value);
		c1Match->setSamples(matchData[9]);
		c2Match->setSamples(matchData[8]);
		c3Match->setSamples(matchData[7]);
		c4Match->setSamples(matchData[6]);
		c5Match->setSamples(matchData[5]);
		c6Match->setSamples(matchData[4]);
		c7Match->setSamples(matchData[3]);
		c8Match->setSamples(matchData[2]);
		c9Match->setSamples(matchData[1]);
		c10Match->setSamples(matchData[0]);
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

void PlotMatch::updateGradient()
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

void PlotMatch::resizeEvent( QResizeEvent *event )
{
    QwtPlot::resizeEvent( event );
#ifdef Q_WS_X11
    updateGradient();
#endif
}


void PlotMatch::populateOne(const QVector <QString> &name)
{
    // Insert new curves
	delete c1Match;
    c1Match = new QwtPlotCurve(name[0]);
    c1Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Match->setPen(QPen(Qt::red));
    c1Match->attach(this);
}

void PlotMatch::populateTwo(const QVector <QString> &name)
{
    // Insert new curves
    c1Match = new QwtPlotCurve(name[1]);	
    c1Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Match->setPen(QPen(Qt::red));
    c1Match->attach(this);

	c2Match = new QwtPlotCurve(name[0]);
    c2Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Match->setPen(QPen(Qt::green));
    c2Match->attach(this);
}

void PlotMatch::populateThree(const QVector <QString> &name)
{
    // Insert new curves
//	delete c1Match;
//	delete c2Match;
//	delete c3Match;
    c1Match = new QwtPlotCurve(name[2]);
    c1Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Match->setPen(QPen(Qt::red));
    c1Match->attach(this);

    c2Match = new QwtPlotCurve(name[1]);
    c2Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Match->setPen(QPen(Qt::green));
    c2Match->attach(this);

    c3Match = new QwtPlotCurve(name[0]);
    c3Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Match->setPen(QPen(Qt::blue));
    c3Match->attach(this);
}

void PlotMatch::populateFour(const QVector <QString> &name)
{
    // Insert new curves
//	delete c1Match;
//	delete c2Match;
//	delete c3Match;
//	delete c4Match;

    c1Match = new QwtPlotCurve(name[3]);
    c1Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Match->setPen(QPen(Qt::red));
    c1Match->attach(this);

    c2Match = new QwtPlotCurve(name[2]);
    c2Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Match->setPen(QPen(Qt::green));
    c2Match->attach(this);

    c3Match = new QwtPlotCurve(name[1]);
    c3Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Match->setPen(QPen(Qt::blue));
    c3Match->attach(this);

    c4Match = new QwtPlotCurve(name[0]);
    c4Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Match->setPen(QPen(Qt::cyan));
    c4Match->attach(this);
}

void PlotMatch::populateFive(const QVector <QString> &name)
{
    // Insert new curves
    c1Match = new QwtPlotCurve(name[4]);
    c1Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Match->setPen(QPen(Qt::red));
    c1Match->attach(this);

    c2Match = new QwtPlotCurve(name[3]);
    c2Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Match->setPen(QPen(Qt::green));
    c2Match->attach(this);

    c3Match = new QwtPlotCurve(name[2]);
    c3Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Match->setPen(QPen(Qt::blue));
    c3Match->attach(this);

    c4Match = new QwtPlotCurve(name[1]);
    c4Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Match->setPen(QPen(Qt::cyan));
    c4Match->attach(this);

    c5Match = new QwtPlotCurve(name[0]);
    c5Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Match->setPen(QPen(Qt::magenta));
    c5Match->attach(this);
}

void PlotMatch::populateSix(const QVector <QString> &name)
{
    // Insert new curves
    c1Match = new QwtPlotCurve(name[5]);
    c1Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Match->setPen(QPen(Qt::red));
    c1Match->attach(this);

    c2Match = new QwtPlotCurve(name[4]);
    c2Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Match->setPen(QPen(Qt::green));
    c2Match->attach(this);

    c3Match = new QwtPlotCurve(name[3]);
    c3Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Match->setPen(QPen(Qt::blue));
    c3Match->attach(this);

    c4Match = new QwtPlotCurve(name[2]);
    c4Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Match->setPen(QPen(Qt::cyan));
    c4Match->attach(this);

    c5Match = new QwtPlotCurve(name[1]);
    c5Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Match->setPen(QPen(Qt::magenta));
    c5Match->attach(this);

    c6Match = new QwtPlotCurve(name[0]);
    c6Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Match->setPen(QPen(Qt::yellow));
    c6Match->attach(this);
}

void PlotMatch::populateSeven(const QVector <QString> &name)
{
    // Insert new curves
    c1Match = new QwtPlotCurve(name[6]);
    c1Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Match->setPen(QPen(Qt::red));
    c1Match->attach(this);

    c2Match = new QwtPlotCurve(name[5]);
    c2Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Match->setPen(QPen(Qt::green));
    c2Match->attach(this);

    c3Match = new QwtPlotCurve(name[4]);
    c3Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Match->setPen(QPen(Qt::blue));
    c3Match->attach(this);

    c4Match = new QwtPlotCurve(name[3]);
    c4Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Match->setPen(QPen(Qt::cyan));
    c4Match->attach(this);

    c5Match = new QwtPlotCurve(name[2]);
    c5Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Match->setPen(QPen(Qt::magenta));
    c5Match->attach(this);

    c6Match = new QwtPlotCurve(name[1]);
    c6Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Match->setPen(QPen(Qt::yellow));
    c6Match->attach(this);

    c7Match = new QwtPlotCurve(name[0]);
    c7Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c7Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c7Match->setPen(QPen(Qt::gray));
    c7Match->attach(this);
}

void PlotMatch::populateEight(const QVector <QString> &name)
{
    // Insert new curves
    c1Match = new QwtPlotCurve(name[7]);
    c1Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Match->setPen(QPen(Qt::red));
    c1Match->attach(this);

    c2Match = new QwtPlotCurve(name[6]);
    c2Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Match->setPen(QPen(Qt::green));
    c2Match->attach(this);

    c3Match = new QwtPlotCurve(name[5]);
    c3Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Match->setPen(QPen(Qt::blue));
    c3Match->attach(this);

    c4Match = new QwtPlotCurve(name[4]);
    c4Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Match->setPen(QPen(Qt::cyan));
    c4Match->attach(this);

    c5Match = new QwtPlotCurve(name[3]);
    c5Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Match->setPen(QPen(Qt::magenta));
    c5Match->attach(this);

    c6Match = new QwtPlotCurve(name[2]);
    c6Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Match->setPen(QPen(Qt::yellow));
    c6Match->attach(this);

    c7Match = new QwtPlotCurve(name[1]);
    c7Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c7Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c7Match->setPen(QPen(Qt::gray));
    c7Match->attach(this);

    c8Match = new QwtPlotCurve(name[0]);
    c8Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c8Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c8Match->setPen(QPen(Qt::black));
    c8Match->attach(this);
}

void PlotMatch::populateNine(const QVector <QString> &name)
{
    // Insert new curves
    // Insert new curves
    c1Match = new QwtPlotCurve(name[8]);
    c1Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Match->setPen(QPen(Qt::red));
    c1Match->attach(this);

    c2Match = new QwtPlotCurve(name[7]);
    c2Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Match->setPen(QPen(Qt::green));
    c2Match->attach(this);

    c3Match = new QwtPlotCurve(name[6]);
    c3Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Match->setPen(QPen(Qt::blue));
    c3Match->attach(this);

    c4Match = new QwtPlotCurve(name[5]);
    c4Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Match->setPen(QPen(Qt::cyan));
    c4Match->attach(this);

    c5Match = new QwtPlotCurve(name[4]);
    c5Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Match->setPen(QPen(Qt::magenta));
    c5Match->attach(this);

    c6Match = new QwtPlotCurve(name[3]);
    c6Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Match->setPen(QPen(Qt::yellow));
    c6Match->attach(this);

    c7Match = new QwtPlotCurve(name[2]);
    c7Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c7Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c7Match->setPen(QPen(Qt::gray));
    c7Match->attach(this);

    c8Match = new QwtPlotCurve(name[1]);
    c8Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c8Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c8Match->setPen(QPen(Qt::black));
    c8Match->attach(this);

    c9Match = new QwtPlotCurve(name[0]);
    c9Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c9Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c9Match->setPen(QPen(Qt::darkRed));
    c9Match->attach(this);
}

void PlotMatch::populateTen(const QVector <QString> &name)
{
    // Insert new curves
    c1Match = new QwtPlotCurve(name[9]);
    c1Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c1Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c1Match->setPen(QPen(Qt::red));
    c1Match->attach(this);

    c2Match = new QwtPlotCurve(name[8]);
    c2Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c2Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c2Match->setPen(QPen(Qt::green));
    c2Match->attach(this);

    c3Match = new QwtPlotCurve(name[7]);
    c3Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c3Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c3Match->setPen(QPen(Qt::blue));
    c3Match->attach(this);

    c4Match = new QwtPlotCurve(name[6]);
    c4Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c4Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c4Match->setPen(QPen(Qt::cyan));
    c4Match->attach(this);

    c5Match = new QwtPlotCurve(name[5]);
    c5Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c5Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c5Match->setPen(QPen(Qt::magenta));
    c5Match->attach(this);

    c6Match = new QwtPlotCurve(name[4]);
    c6Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c6Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c6Match->setPen(QPen(Qt::yellow));
    c6Match->attach(this);

    c7Match = new QwtPlotCurve(name[3]);
    c7Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c7Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c7Match->setPen(QPen(Qt::gray));
    c7Match->attach(this);

    c8Match = new QwtPlotCurve(name[2]);
    c8Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c8Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c8Match->setPen(QPen(Qt::black));
    c8Match->attach(this);

    c9Match = new QwtPlotCurve(name[1]);
    c9Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c9Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c9Match->setPen(QPen(Qt::darkRed));
    c9Match->attach(this);

    c10Match = new QwtPlotCurve(name[0]);
    c10Match->setRenderHint(QwtPlotItem::RenderAntialiased);
    c10Match->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    c10Match->setPen(QPen(Qt::darkGreen));
    c10Match->attach(this);
}