#ifndef	SUPPLYSELECTION_H
#define SUPPLYSELECTION_H
#include <QtGui>
#include <QVector>
#include <QPair>
#include <QTableView>
#include <QListView>
#include <QInputDialog>

#include <QWidget>
#include "ui_supplySelected.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QStandardItemModel>

class SupplySelection : public QWidget, public Ui::supplySelected
{
    Q_OBJECT
public:
//functions
	SupplySelection(QWidget *parent = 0);
	~SupplySelection();
	void AllocateMemory(int n);

//variables
	Ui_supplySelected *Ui;
	QList<QPair<QString, QVector <float> > > selectedSupplyList;

private:
//functions
	void updateView();

//variables
	QVector<float> selectedSupply;
	QString selectedSupplyName;


};

#endif
