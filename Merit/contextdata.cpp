#include "contextdata.h"

ContextData::ContextData()
{
}
void ContextData::AllocateMemory(int n)
{
	//allocate 8760 to the vectors for storing weather data
    m_pDbTemperature.reserve(n);
    m_pDbTemperature.resize(n);

	m_pDirectSolar.reserve(n);
	m_pDirectSolar.resize(n);

	m_pDiffuseSolar.reserve(n);
	m_pDiffuseSolar.resize(n);

	m_pWindSpeed.reserve(n);
	m_pWindSpeed.resize(n);

	m_pWindDirection.reserve(n);
	m_pWindDirection.resize(n);

	m_pRelativeHumidity.reserve(n);
	m_pRelativeHumidity.resize(n);
}
