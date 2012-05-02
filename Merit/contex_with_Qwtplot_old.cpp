#include <QtGui>
#include <QtGui/QComboBox>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "context.h"

Context::Context(QWidget *parent)
    : QWidget(parent)
{
//    meritTab = new MeritTab;
    setupUi(this);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	LoadWeatherLocationList();
    //meritTab = new MeritTab;
	connect(viewButton, SIGNAL(clicked()),this, SLOT(drawWeatherChart()));
}


void Context::LoadWeatherLocationList()
{
	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list for the weather location
	query.exec("SELECT climate_site FROM climate_site");
    if (!query.isActive())
		QMessageBox::warning(this, tr("Database Error"),
                         query.lastError().text());


	//clear the data in the weather location list
	weather->clear();
	//navigate through the fected query result set

	while (query.next()) 
	{
		QString location = query.value(0).toString();
		weather->addItem(location);
		//cerr << qPrintable(location) << endl;
    }

}

void Context::AllocateMemory(int n)
{
	//allocate 8760 to the vectors for storing weather data
    m_fDbTemperature.reserve(n);
    m_fDbTemperature.resize(n);

	m_fDirectSolar.reserve(n);
	m_fDirectSolar.resize(n);

	m_fDiffuseSolar.reserve(n);
	m_fDiffuseSolar.resize(n);

	m_fWindSpeed.reserve(n);
	m_fWindSpeed.resize(n);

	m_fWindDirection.reserve(n);
	m_fWindDirection.resize(n);

	m_fRelativeHumidity.reserve(n);
	m_fRelativeHumidity.resize(n);
}

void Context::drawWeatherChart()
{
	int i=0;
	AllocateMemory(8760);

	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list for the weather location
	QString strSQL;
	strSQL = "SELECT ambient, dir_solar, diff_solar, wind_speed, wind_dir, rh FROM climate_espr WHERE climate_site = 'Glasgow'";

//	strSQL.Format(_T("SELECT ambient, dir_solar, diff_solar, wind_speed, wind_dir, rh FROM climate_espr WHERE climate_site = '%s'"),getWeatherSite());
	query.exec(strSQL);
    if (!query.isActive())
		QMessageBox::warning(this, tr("Database Error"),
                         query.lastError().text());


	//clear the data in the weather location list
	//weather->clear();
	//navigate through the fected query result set

	while (query.next()) 
	{
		setDbTemperature(i, query.value(0).toFloat());
		setDirectSolar(i, query.value(1).toFloat());
		setDiffuseSolar(i, query.value(2).toFloat());
		setWindSpeed(i, query.value(3).toFloat());
		setWindDirection(i, query.value(4).toFloat());
		setRelativeHumidity(i, query.value(5).toFloat());
		i++;
    }

//	QwtPlot *weatherPlot; 
	double x[8760], s[8760], DrS[8760];//, c[100]; 
	QwtPlotCurve* w1 = new QwtPlotCurve("Temperature");
	QwtPlotCurve* w2 = new QwtPlotCurve("Temperature");
//	QwtPlotCurve* w3 = new QwtPlotCurve("Temperature");
//	QwtPlotCurve* w4 = new QwtPlotCurve("Temperature");
//	QwtPlotCurve* w5 = new QwtPlotCurve("Temperature");
//	QwtPlotCurve* w6 = new QwtPlotCurve("Temperature");

	for( int i=0; i<m_fDbTemperature.size(); i++ ) 
	{ 
		x[i] = (double)i; 
		s[i] = m_fDbTemperature[i]; 
		DrS[i] = m_fDirectSolar[i];
//		c[i] = cos(x[i]); 
	}


	// copy the data into the curves
	w1->setSamples(x, s, 8760);
	w1->attach(qwtWeatherPlot);

	w2->setSamples(x, DrS, 8760);
	w2->attach(qwtWeatherPlot);



	qwtWeatherPlot->replot();
}


void Context::setWeatherSite(const QString& site)
{
	m_sWeatherSite = site;
}