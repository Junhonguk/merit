#ifndef	DEMANDPROCESS_H
#define DEMANDPROCESS_H
#include <QtGui>
#include <QVector>
#include <QPair>
#include <QTableView>
#include <QListView>
#include <QInputDialog>

#include <QWidget>
#include "ui_demandProcess.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QStandardItemModel>

class DemandProcess : public QWidget, public Ui::demandProcess
{
    Q_OBJECT
public:
//functions
	DemandProcess(QWidget *parent = 0);
	~DemandProcess();

	void AllocateMemory(int n);

//variables
	Ui_demandProcess *Ui;
	QList<QPair<QString, QVector <float> > > processDemandList;
	QString m_ItemName;

private:
//functions
	void updateView();

//variables
	QVector<float> processingDemand;
	QString processDemandName;


public slots:
	void setListItemName(QListWidgetItem *Item);

};

#endif
