#ifndef	DEMANDSELECTION_H
#define DEMANDSELECTION_H
#include <QtGui>
#include <QVector>
#include <QPair>
#include <QTableView>
#include <QListView>
#include <QInputDialog>

#include <QWidget>
#include "ui_demandSelected.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QStandardItemModel>

class DemandSelection : public QWidget, public Ui::demandSelected
{
    Q_OBJECT
public:
	DemandSelection(QWidget *parent = 0);
	~DemandSelection();
	Ui_demandSelected *Ui;

	void AllocateMemory(int n);


	QList<QPair<QString, QVector <float> > > selectedDemandList;

private:
	void updateView();



private:
	QVector<float> selectedDemand;

	QString selectedDemandName;



};

#endif
