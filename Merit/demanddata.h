#ifndef DEMANDDATA_H_
#define DEMANDDATA_H_

#include <QtCore/QVector>
#include <QtCore/QString>

class DemandData
{
public:
	DemandData();

	void AllocateMemory(int n);

	void operator=(const DemandData &other);

	bool operator==(const DemandData &other);
	bool operator!=(const DemandData &other);

//setter
	void setDemandName(const QString& temp);
	void setDemandInfo(const QString& temp);
	void setStartDay(const int& temp);
	void setEndDay(const int& temp);
	void setHourSteps(const int& temp);

//getter
	const QString& demandName() const;
	const QString& demandInfo() const;

	QVector<float> m_pDemandDataVector;

	const int& startDay() const;
	const int& endDay() const;
	const int& hourSteps() const;

private:	
	QString m_sDemandName;
	QString m_sDemandInfo;
	int m_iStartDay;
	int m_iEndDay;
	int m_iHourSteps;
};

#endif