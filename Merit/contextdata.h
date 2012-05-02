#ifndef _CONTEXTDATA_H_
#define _CONTEXTDATA_H_

#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QStringList>

class ContextData
{
public:
	ContextData();

	void AllocateMemory(int n);

	QVector<float> m_pDbTemperature;
	QVector<float> m_pDirectSolar;
	QVector<float> m_pDiffuseSolar;
	QVector<float> m_pWindSpeed;
	QVector<float> m_pWindDirection;
	QVector<float> m_pRelativeHumidity;

	QStringList m_WeatherSiteList;
	
	float m_fLatitude;
	float m_fLongitude;

	QString m_sWeatherSite;
};

#endif