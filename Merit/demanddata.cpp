#include "demanddata.h"

DemandData::DemandData()
{
	m_sDemandName = "";
	m_sDemandInfo = "";
	m_iStartDay = 0;
	m_iEndDay = 0;
	m_iHourSteps = 0;

	AllocateMemory((m_iEndDay-m_iStartDay+1)*m_iHourSteps*24);
}

void DemandData::AllocateMemory(int n)
{
	//allocate 8760 to the vectors for storing energy demand data
	m_pDemandDataVector.clear();
    m_pDemandDataVector.reserve(n);
    m_pDemandDataVector.resize(n);
}

//setter
//set demand name
void DemandData::setDemandName(const QString& temp)
{
	m_sDemandName = temp;
}

//set demand info
void DemandData::setDemandInfo(const QString& temp)
{
    m_sDemandInfo = temp;
}

//set start day
void DemandData::setStartDay(const int& temp)
{
	m_iStartDay = temp;
}

//set end day
void DemandData::setEndDay(const int& temp)
{
	m_iEndDay = temp;
}

//set hour steps
void DemandData::setHourSteps(const int& temp)
{
	m_iHourSteps = temp;
}

//getter
const QString& DemandData::demandName() const
{
	return m_sDemandName;
}

const QString& DemandData::demandInfo() const
{
	return m_sDemandInfo;
}

const int& DemandData::startDay() const
{
	return m_iStartDay;
}

const int& DemandData::endDay() const
{
	return m_iEndDay;
}

const int& DemandData::hourSteps() const
{
	return m_iHourSteps;
}

void DemandData::operator=(const DemandData& other)

{
	m_sDemandName = other.m_sDemandName;
	m_sDemandInfo = other.m_sDemandInfo;
	m_iStartDay = other.m_iStartDay;
	m_iEndDay = other.m_iEndDay;
	m_iHourSteps = other.m_iHourSteps;
}

bool DemandData::operator==(const DemandData& other)
{
	if (m_sDemandName == other.m_sDemandName && m_sDemandInfo == other.m_sDemandInfo &&
		m_iStartDay == other.m_iStartDay && m_iEndDay == other.m_iEndDay && m_iHourSteps == other.m_iHourSteps)
		return true;
	return false;
}

bool DemandData::operator!=(const DemandData& other)
{
	if (m_sDemandName == other.m_sDemandName || m_sDemandInfo == other.m_sDemandInfo ||
		m_iStartDay == other.m_iStartDay || m_iEndDay == other.m_iEndDay || m_iHourSteps == other.m_iHourSteps)
		return false;
	return true;
}