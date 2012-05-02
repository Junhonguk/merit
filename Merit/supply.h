#ifndef	SUPPLY_H
#define SUPPLY_H
#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QVector>
#include <QMap>

//#include "ui_demand.h"
#include "merittab.h"
#include "supplyprofile.h"
#include "supplyselection.h"
#include "supplyprocess.h"
//#include "supplyscenarioview.h"
#include "supplyscenariotreemodel.h"

#include "merit.h"
#include "qwt_plot.h"

//for the graph
#include "plot_supply.h"
//#include "plot_supply_two.h"
//for scaling dialog
#include "scaledialog.h"
#include "shiftdialog.h"
#include "supplypropertydialog.h"
#include "supplydata.h"

class SupplyData;

typedef  QList<SupplyData> SupplyDataList;
typedef  QList<SupplyData>::Iterator SupplyDataListIterator;

class Supply : public QWidget
{
    Q_OBJECT
public:
	Supply(QWidget *parent = 0);
	~Supply();

	void retrieveSupplyData(const QString& name);
	void setSupplyData(int index, float supply);
	float getSupplyData(int index);
	QString getSupplyDateData(int index);
	void setSupplyDateData(int index, QString supply_date);
	int convertToJulian(int Day,int Month);
	void setTemporalParameterList();

	int getStartDay(const QString& itemName);
	int getEndDay(const QString& itemName);
	int getHourSteps(const QString& itemName);

	void initialiseTemporalList(int n);
	void initialiseProcessSupplyTemporalList();
	void initialiseSelectedSupplyTemporalList();

	void AllocateMemory(int n);
	void drawMultipleSupplys(QListWidgetItem * items);
	void removeCurrentScenario();



    MeritTab *meritTab;
	SupplyProfile *ssView;
	SupplyProcess *ssProcess;
	SupplySelection *ssSelected;

	QGridLayout *gridLayout;

    QVBoxLayout *verticalLayout;
	QPushButton *addButton;
	QPushButton *delButton;

//	QPushButton *newButton;
    QSpacerItem *horizontalSpacer_1; 
    QSpacerItem *horizontalSpacer_2; 
	QSpacerItem *horizontalSpacer_3; 

	QTreeView *tree;
	QMdiArea *mdiArea;
	QwtPlot *qwtPlot;
	QSplitter* splitterHrt;
	QSplitter* splitterVrt;
	SupplyScenarioTreeModel* model;
    QWidget *widget;

	QHBoxLayout *horizontalLayout1; 
	QHBoxLayout *horizontalLayout2; 
	QVBoxLayout *verticalLayout1;

	PlotSupply *supplyPlot;
//	PlotDemandTwo *demandPlot2;

	QString m_ItemName;
	QStringList m_SupplyScenarioData;	
	QVector<float> supplyData;

//interface: for the combobox list int the demand property dialog
	QStringList start_day_list;
	QStringList end_day_list;
	QStringList hour_steps_list;

	QVector<float> supplyDataVector;
	QVector<float> supplyTotalVector;

	QList< QPair<QString, int> > supplyStartDayList;
	QList< QPair<QString, int> > supplyEndDayList;
	QList< QPair<QString, int> > supplyHourStepsList;

	QVector <QString> ItemNameVector;

//to install the datetime data for the relevant demand 
	QVector<QString> supplyDateDataVector;	
	int m_iStartDay;
	int m_iEndDay;
	int m_iHourSteps;

//for process supply
	QVector<float> processSupplyDataVector;
	QVector<float> processSupplyTotalVector;
	QVector <QVector <float> > processSupplyVector;
	QVector <QString> processItemNameVector;
	QList<QListWidgetItem *> processItems;

	QList< QPair<QString, int> > prcSupplyStartDayList;
	QList< QPair<QString, int> > prcSupplyEndDayList;
	QList< QPair<QString, int> > prcSupplyHourStepsList;

	QList<QPair<QString, QVector <float> > > processSupplyList;

//for selected supplys
	QList<QListWidgetItem *> ItemSelected;

	QList< QPair<QString, int> > selSupplyStartDayList;
	QList< QPair<QString, int> > selSupplyEndDayList;
	QList< QPair<QString, int> > selSupplyHourStepsList;

	QList<QPair<QString, QVector <float> > > selectedSupplyList;

	QVector <QVector <float> > SupplySelectedVector; 

//for supply scenario
	SupplyDataList supplyProfileList; //for demand profile list of a specific demand scenario
    SupplyDataListIterator spIter; //iterator for the demand profile list

	QList<QPair<QString, SupplyDataList > > supplyScenarioPortfolio; //demand scenario portfolio
	QMap <QString, QString> mapSS; //to map between the name and the ds sequence
	QStringList supplyScenarioName; //a list to store the names of the demand scenario

 signals:
	 void valueChanged(QMap<QString, QString>  &newValue);
	 void valueChangedSSP(QList<QPair<QString, SupplyDataList > > &newValue); //signal for ssp

protected slots:
//	void selectionChanged( const QItemSelection &selected, const QItemSelection &deselected );
//    void dataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight );
	void addSupplyScenario();
	void deleteCurrentScenario();


	void drawSupplyGraph();
	void drawSingleMiddleSupplyGraph();
	void drawSingleRightSupplyGraph();
	void drawRightSupplyGraph();

	void moveMiddle();
	void moveRight();

	void removeMiddle();
	void removeRight();
	void LocateItem(const QPoint &pos);

	void locateProcessListItem(const QPoint &pos);
	void locateSelectedListItem(const QPoint &pos);

	void showSupplyInfo();
	void showMiddleSupplyInfo();
	void showRightSupplyInfo();

//slot for setting the name of the item clicked in the tablewidget and processlistwidget
	void setTableItemName(QTableWidgetItem *Item);
	void setListItemName(QListWidgetItem *Item);

//	QModelIndex ItemName();
//	void populateProfiles();
	void scaleSupplyProfile();
	void shiftSupplyProfile();
	void clearSupplyProfile();

// save scenario
	void saveCurrentScenario();
	void saveCurrentScenarioAs();
//shift profile
	void combineSupplyProfile();
//deploy profiles in scenario
	void deployProfiles();
//supply property
	void supplyProperty();
//process supply property
	void procSupplyProperty();
//selected supply property
	void selSupplyProperty();

public:
	void setItemSelected(QTableWidgetItem * temp);
	QTableWidgetItem * getItemSelected();

//for selected demand
	void setListItemSelected(QListWidgetItem * temp);
	QListWidgetItem * getListItemSelected();

//for process demand
	void setProcessListItems(QListWidgetItem * temp);
	QListWidgetItem * getProcessListItems();

private:
	int m_SupScnNo;
	QTableWidgetItem* m_ItemSelected;
	QListWidgetItem* m_ListItemSelected;
	QListWidgetItem* m_ProcessListItems;
};

#endif
