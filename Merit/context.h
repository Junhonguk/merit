#ifndef	CONTEXT_H
#define CONTEXT_H
#include <QtGui>
#include <QWidget>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QSharedDataPointer>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "ui_context.h"
#include "merittab.h"
#include "plot.h"
//#include "weatherdata.h"
#include "contextdata.h"

class ContextData;

class Context : public QWidget, public Ui::Context
{
    Q_OBJECT
public:
	Context(QWidget *parent = 0);
	Context(const Context &other);
	~Context();
	Context& operator=(const Context& other);

    MeritTab *meritTab;
    Ui_Context* Ui;

	Plot* qwtWeatherPlot;

public:
	void LoadWeatherLocationList();
//	void AllocateMemory(int n);
//	void retrieveWeatherData(const QString& site);
//getter
	QString getWeatherSite() const;
	float getLatitude();
	float getLongitude();


//setter
//	void setWeatherSite(const QString& site);
	void setDbTemperature(int index, float T);
	void setDirectSolar(int index, float DrS);
	void setDiffuseSolar(int index, float DfS);
	void setWindSpeed(int index, float WS);
	void setWindDirection(int index, float WD);
	void setRelativeHumidity(int index, float RH);

//getter
	QString& WeatherSite();
	float DbTemperature(int index);
	float DirectSolar(int index);
	float DiffuseSolar(int index);
	float WindSpeed(int index);
	float WindDirection(int index);
	float RelativeHumidity(int index);

protected:
//	virtual void focusOutEvent(QFocusEvent * event); 

public slots:
    void drawWeatherProfile(const QString& site);
	void setWeatherSite(const QString& site);
	void retrieveWeatherData(const QString& site);
	void setLongitude(const QString& site);
	void setLatitude(const QString& site);
	void setStartDate(const QString& site);
	void setEndDate(const QString& site);

private:
	ContextData* d;
};
#endif
