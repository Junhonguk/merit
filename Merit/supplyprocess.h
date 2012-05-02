#ifndef	SUPPLYPROCESS_H
#define SUPPLYPROCESS_H
#include <QtGui>
#include <QVector>
#include <QPair>
#include <QTableView>
#include <QListView>
#include <QInputDialog>

#include <QWidget>
#include "ui_supplyProcess.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QStandardItemModel>

class SupplyProcess : public QWidget, public Ui::supplyProcess
{
    Q_OBJECT
public:
//functions
	SupplyProcess(QWidget *parent = 0);
	~SupplyProcess();
	void AllocateMemory(int n);

//variables
	QList<QPair<QString, QVector <float> > > processSupplyList;
	Ui_supplyProcess *Ui;
	QString m_ItemName;
private:
//functions	
	void updateView();

//variables
	QVector<float> processingSupply;
	QString processSupplyName;
public slots:
	void setListItemName(QListWidgetItem *Item);

};

#endif
