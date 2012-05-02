#ifndef SUPPLYDATA_H_
#define SUPPLYDATA_H_

#include <QtCore/QVector>
#include <QtCore/QString>

class SupplyData
{
public:
	SupplyData();

	void AllocateMemory(int n);

	void operator=(const SupplyData &other);

	bool operator==(const SupplyData &other);
	bool operator!=(const SupplyData &other);

//setter
	void setSupplyName(const QString& temp);
	void setSupplyInfo(const QString& temp);
	void setStartDay(const int& temp);
	void setEndDay(const int& temp);
	void setHourSteps(const int& temp);

//getter
	const QString& supplyName() const;
	const QString& supplyInfo() const;

	QVector<float> m_pSupplyDataVector;

	const int& startDay() const;
	const int& endDay() const;
	const int& hourSteps() const;

private:	
	QString m_sSupplyName;
	QString m_sSupplyInfo;
	int m_iStartDay;
	int m_iEndDay;
	int m_iHourSteps;
};

#endif