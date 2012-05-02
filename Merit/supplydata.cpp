#include "supplydata.h"

SupplyData::SupplyData()
{
	m_sSupplyName = "";
	m_sSupplyInfo = "";
	m_iStartDay = 0;
	m_iEndDay = 0;
	m_iHourSteps = 0;

	AllocateMemory((m_iEndDay-m_iStartDay+1)*m_iHourSteps*24);
}

void SupplyData::AllocateMemory(int n)
{
	//allocate 8760 to the vectors for storing energy demand data
	m_pSupplyDataVector.clear();
    m_pSupplyDataVector.reserve(n);
    m_pSupplyDataVector.resize(n);
}

//setter
//set supply name
void SupplyData::setSupplyName(const QString& temp)
{
	m_sSupplyName = temp;
}

//set demand info
void SupplyData::setSupplyInfo(const QString& temp)
{
    m_sSupplyInfo = temp;
}

//set start day
void SupplyData::setStartDay(const int& temp)
{
	m_iStartDay = temp;
}

//set end day
void SupplyData::setEndDay(const int& temp)
{
	m_iEndDay = temp;
}

//set hour steps
void SupplyData::setHourSteps(const int& temp)
{
	m_iHourSteps = temp;
}

//getter
const QString& SupplyData::supplyName() const
{
	return m_sSupplyName;
}

const QString& SupplyData::supplyInfo() const
{
	return m_sSupplyInfo;
}

const int& SupplyData::startDay() const
{
	return m_iStartDay;
}

const int& SupplyData::endDay() const
{
	return m_iEndDay;
}

const int& SupplyData::hourSteps() const
{
	return m_iHourSteps;
}

void SupplyData::operator=(const SupplyData& other)

{
	m_sSupplyName = other.m_sSupplyName;
	m_sSupplyInfo = other.m_sSupplyInfo;
	m_iStartDay = other.m_iStartDay;
	m_iEndDay = other.m_iEndDay;
	m_iHourSteps = other.m_iHourSteps;
}

bool SupplyData::operator==(const SupplyData& other)
{
	if (m_sSupplyName == other.m_sSupplyName && m_sSupplyInfo == other.m_sSupplyInfo &&
		m_iStartDay == other.m_iStartDay && m_iEndDay == other.m_iEndDay && m_iHourSteps == other.m_iHourSteps)
		return true;
	return false;
}

bool SupplyData::operator!=(const SupplyData& other)
{
	if (m_sSupplyName == other.m_sSupplyName || m_sSupplyInfo == other.m_sSupplyInfo ||
		m_iStartDay == other.m_iStartDay || m_iEndDay == other.m_iEndDay || m_iHourSteps == other.m_iHourSteps)
		return false;
	return true;
}