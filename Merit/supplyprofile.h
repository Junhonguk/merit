#ifndef	SUPPLYPROFILE_H
#define SUPPLYPROFILE_H
#include <QtGui>
#include <QVector>
#include <QPair>
#include <QTableView>
#include <QListView>
#include <QInputDialog>

#include <QWidget>
#include "ui_supplyProfile.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QStandardItemModel>

class SupplyProfile : public QWidget, public Ui::supplyProfile
{
    Q_OBJECT
public:
	SupplyProfile(QWidget *parent = 0);
	~SupplyProfile();
	Ui_supplyProfile *Ui;
    void LoadStandardSupplyList();
	void retrieveSupplyData(const QString& name);
	void AllocateMemory(int n);
	float getSupplyData(int index);
	void setSupplyData(int index, float supply);
	void itemClicked(QTableWidgetItem* item );

//	void moveMiddle();
//	void moveRight();

	QTableWidget *stdProfile;
	QTableWidgetItem *stdItem1;
	QTableWidgetItem *stdItem2;
//	QTableView *stdTableView;
	QTableView stdTableView;

//	QVector<QString> pRefNameVector;
//	QVector<QString> pDescriptionVector;

	QList<QPair<QString, QVector <float> > > selectedSupplyList;
	QList<QPair<QString, QVector <float> > > processSupplyList;
	
	QString m_ItemName;

	QVector<float> supplyData;
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

	QVector<float> processingSupply;
	QVector<float> selectedSupply;
	QVector<float> standardSupply;

	QString processSupplyName;
	QString selectedSupplyName;
	QString standardSupplyName;

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
//	void setListItemName(QListWidgetItem *Item);


};

#endif
