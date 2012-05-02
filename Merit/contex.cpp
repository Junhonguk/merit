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
	d = new ContextData;

    setupUi(this);


    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	LoadWeatherLocationList();

//set the initial index as -1 
//	weather->setEditable(true);
	weather->setCurrentIndex(-1);
//set the default text "site"
	weather->setEditText("--Select site--");
//set the text as grey color
	startDateTimeEdit->setDateTime(QDateTime::currentDateTime());	
	endDateTimeEdit->setDateTime(QDateTime::currentDateTime());
//	weather->setEditable(false);

	retrieveWeatherData(weather->currentText());
//	drawWeatherProfile(weather->currentText());

	connect(weather, SIGNAL(activated(QString)),this, SLOT(setWeatherSite(QString)));
	connect(weather, SIGNAL(activated(QString)),this, SLOT(retrieveWeatherData(QString)));
	connect(weather, SIGNAL(activated(QString)),this, SLOT(drawWeatherProfile(QString)));
	connect(weather, SIGNAL(activated(QString)),this, SLOT(setLatitude(QString)));
	connect(weather, SIGNAL(activated(QString)),this, SLOT(setLongitude(QString)));
	connect(weather, SIGNAL(activated(QString)),this, SLOT(setStartDate(QString)));
	connect(weather, SIGNAL(activated(QString)),this, SLOT(setEndDate(QString)));

//when the current index changes, call the setEditText() slot to reset the text in the combo box
	connect(weather, SIGNAL(currentIndexChanged(QString)),this, SLOT(setEditText(QString)));
}


Context::Context(const Context &other)
{
	d = new ContextData(*other.d);
}

Context::~Context()
{
   delete d;
}
Context& Context::operator= (const Context& other)
{
	*d = *other.d;
	return *this;
}

void Context::setDbTemperature(int index, float T)
{
	d->m_pDbTemperature[index] = T;
}

void Context::setDirectSolar(int index, float DrS)
{
	d->m_pDirectSolar[index] = DrS;
}

void Context::setDiffuseSolar(int index, float DfS)
{
	d->m_pDiffuseSolar[index] = DfS;
}

void Context::setWindSpeed(int index, float WS)
{
	d->m_pWindSpeed[index] = WS;
}

void Context::setWindDirection(int index, float WD)
{
	d->m_pWindDirection[index] = WD;
}

void Context::setRelativeHumidity(int index, float RH)
{
	d->m_pRelativeHumidity[index] = RH;
}

void Context::setWeatherSite(const QString& site)
{
	d->m_sWeatherSite = site;
}

//set the latitude to a particular weather site
void Context::setLatitude(const QString& site)
{

	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list for the weather location
	QString strSQL, strSite;
	float fLatitude = 0.0;
	strSite = "'"  + site + "'";
	strSQL = "SELECT ref_x FROM climate_site WHERE climate_site = ";
	strSQL = strSQL + strSite;
//fetch the latitude data for the selected site 
	query.exec(strSQL);
    if (!query.isActive())
		QMessageBox::warning(this, tr("Database Error"),
                         query.lastError().text());

	//navigate through the fected query result set

	while (query.next()) 
	{
		fLatitude = query.value(0).toFloat();
    }
	d->m_fLatitude = fLatitude;
	latitude->setText(QString("%1").arg(d->m_fLatitude));
}
//set the longitude to a particular weather site
void Context::setLongitude(const QString& site)
{

	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list for the weather location
	QString strSQL, strSite;
	float fLongitude = 0.0;
	strSite = "'"  + site + "'";
	strSQL = "SELECT ref_y FROM climate_site WHERE climate_site = ";
	strSQL = strSQL + strSite;
	//fetch the longitude data for the selected site
	query.exec(strSQL);

    if (!query.isActive())
		QMessageBox::warning(this, tr("Database Error"),
                         query.lastError().text());

	//navigate through the fected query result set

	while (query.next()) 
	{
		fLongitude = query.value(0).toFloat();
    }
	d->m_fLongitude = fLongitude;
	longitude->setText(QString("%1").arg(d->m_fLongitude));
}

//set the start date
void Context::setStartDate(const QString& site)
{

	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list for the weather location
	QString strSQL, strSite;
	QString strStartDate = "";
	strSite = "'"  + site + "'";
	strSQL = "SELECT datetime FROM climate_espr WHERE climate_site = ";
	strSQL = strSQL + strSite;
	strSQL = strSQL + " LIMIT 1";
//fetch the latitude data for the selected site 
	query.exec(strSQL);
    if (!query.isActive())
		QMessageBox::warning(this, tr("Database Error"),
                         query.lastError().text());

	//navigate through the fected query result set

	while (query.next()) 
	{
		strStartDate = query.value(0).toString();
    }

	startDateTimeEdit->setDateTime(QDateTime::fromString(strStartDate, "yyyy-MM-dd hh:mm:ss"));
}

//set the end date
void Context::setEndDate(const QString& site)
{

	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list for the weather location
	QString strSQL, strSite;
	QString strEndDate = "";
	strSite = "'"  + site + "'";
	strSQL = "SELECT datetime FROM climate_espr WHERE climate_site = ";
	strSQL = strSQL + strSite;
	strSQL = strSQL + " ORDER BY datetime DESC LIMIT 1";

//fetch the latitude data for the selected site 
	query.exec(strSQL);
    if (!query.isActive())
		QMessageBox::warning(this, tr("Database Error"),
                         query.lastError().text());

	//navigate through the fected query result set

	while (query.next()) 
	{
		strEndDate = query.value(0).toString();
    }
	
	endDateTimeEdit->setDateTime(QDateTime::fromString(strEndDate, "yyyy-MM-dd hh:mm:ss"));
}


float Context::getLatitude()
{
	return d->m_fLatitude;
}
float Context::getLongitude()
{
	return d->m_fLongitude;
}

QString& Context::WeatherSite()
{
	return d->m_sWeatherSite;
}
float Context::DbTemperature(int index)
{
	return d->m_pDbTemperature[index];
}

float Context::DirectSolar(int index)
{
	return d->m_pDirectSolar[index];
}

float Context::DiffuseSolar(int index)
{
	return d->m_pDiffuseSolar[index];
}

float Context::WindSpeed(int index)
{
	return d->m_pWindSpeed[index];
}

float Context::WindDirection(int index)
{
	return d->m_pWindDirection[index];
}
float Context::RelativeHumidity(int index)
{
	return d->m_pRelativeHumidity[index];
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

//add the item to the list		
		weather->addItem(location);
		
//		d->m_WeatherSiteList
    }

}


void Context::retrieveWeatherData(const QString& site)
{
	int i=0;
	d->AllocateMemory(8760);

	QSqlQuery query;
	query.setForwardOnly(true);
	//fetch the list for the weather location
	QString strSQL, strSite;
	strSite = "'"  + site + "'";
	strSQL = "SELECT ambient, dir_solar, diff_solar, wind_speed, wind_dir, rh FROM climate_espr WHERE climate_site = ";
	strSQL = strSQL + strSite;

//	strSQL.Format(_T("SELECT ambient, dir_solar, diff_solar, wind_speed, wind_dir, rh FROM climate_espr WHERE climate_site = '%s'"),site);
	query.exec(strSQL);
    if (!query.isActive())
		QMessageBox::warning(this, tr("Database Error"),
                         query.lastError().text());

	//clear the data in the weather location list

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
}


void Context::drawWeatherProfile(const QString& site)
{
//	qwtWeatherPlot->setWeatherLocation(d->m_sWeatherSite);
	Plot* weatherPlot = Ui_Context::qwtWeatherPlot;
//	retrieveWeatherData(d->m_sWeatherSite);
	weatherPlot->detachItems(QwtPlotItem::Rtti_PlotCurve, true);

 	weatherPlot->drawGraph(site, d->m_pDbTemperature, d->m_pDirectSolar, d->m_pDiffuseSolar,
							d->m_pWindSpeed, d->m_pWindDirection, d->m_pRelativeHumidity);

}
/*
void Context::focusOutEvent(QFocusEvent * event)
{
	QWidget::focusOutEvent(event);
	if (weather->currentIndex() == -1 ) 
		weather->setEditText( tr( "Please select" ) );
}
*/