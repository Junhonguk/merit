#ifndef	DEMANDPROFILE_H
#define DEMANDPROFILE_H
#include <QtGui>
#include <QVector>
#include <QPair>
#include <QTableView>
#include <QListView>
#include <QInputDialog>

#include <QWidget>
#include "ui_demandProfile.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QStandardItemModel>

class DemandProfile : public QWidget, public Ui::demandProfile
{
    Q_OBJECT
public:
	DemandProfile(QWidget *parent = 0);
	~DemandProfile();
	Ui_demandProfile *Ui;
    void LoadStandardDemandList();
	void retrieveDemandData(const QString& name);
	void AllocateMemory(int n);
	float getDemandData(int index);
	void setDemandData(int index, float demand);
	void itemClicked(QTableWidgetItem* item );

//	void mousePressEvent( QMouseEvent * event );
//	void customContextMenuRequested(const QPoint &pos);
//	void LocateItem(const QPoint &pos);
//	void moveMiddle();
//	void moveRight();

	QTableWidget *stdProfile;
	QTableWidgetItem *stdItem1;
	QTableWidgetItem *stdItem2;
//	QTableView *stdTableView;
	QTableView stdTableView;

//	QVector<QString> pRefNameVector;
//	QVector<QString> pDescriptionVector;

	QList<QPair<QString, QVector <float> > > selectedDemandList;
	QList<QPair<QString, QVector <float> > > processDemandList;
	
	QString m_ItemName;

	QVector<float> demandData;
//	QVector<float> demandTotal;
private:
	void updateView();

//	QString m_dsDescription;
//    QString m_dsName;
//	QString m_text;
//	QString m_title;


private:
	QVector<QString> pRefNameVector;
	QVector<QString> pDescriptionVector;

	QVector<float> processingDemand;
	QVector<float> selectedDemand;
	QVector<float> standardDemand;

	QString processDemandName;
	QString selectedDemandName;
	QString standardDemandName;

//  QString dsDescription() const {return m_dsDescription;}
//  void setdsDescription(QString dsr) {m_dsDescription = dsr;}

//  QString dsName() const {return m_dsName;}
//  void setdsName(QString name) {m_dsName = name;}

signals:
//  void textChanged (const QString &text);

public slots:
//  void setText(const QString &text);
//  void setTitle(const QString &title);
//	void moveMiddle();
//	void moveRight();
//	void removeMiddle();
//	void removeRight();
	void setTableItemName(QTableWidgetItem *Item);

//	void LocateItem(const QPoint &pos);
//	void demandInfo(QTableWidgetItem* item);

};

#endif
