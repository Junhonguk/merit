#ifndef	DEMAND_H
#define DEMAND_H
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
#include "demandprofile.h"
#include "demandselection.h"
#include "demandprocess.h"
//#include "demandscenarioview.h"
#include "demandscenariotreemodel.h"
#include "merit.h"
#include "qwt_plot.h"

//for the graph
#include "plot_demand.h"
#include "plot_demand_two.h"
//for scaling dialog
#include "scaledialog.h"
#include "shiftdialog.h"
#include "demandpropertydialog.h"
#include "demanddata.h"

class DemandData;

typedef  QList<DemandData> DemandDataList;
typedef  QList<DemandData>::Iterator DemandDataListIterator;

class Demand : public QWidget
{
    Q_OBJECT
public:
	Demand(QWidget *parent = 0);
	~Demand();
//functions
	void retrieveDemandData(const QString& name);
	void setDemandData(int index, float demand);
	float getDemandData(int index);

	QString getDemandDateData(int index);
	void setDemandDateData(int index, QString demand_date);
	int convertToJulian(int Day,int Month);
	void setTemporalParameterList();

	int getStartDay(const QString& itemName);
	int getEndDay(const QString& itemName);
	int getHourSteps(const QString& itemName);

	void initialiseTemporalList(int n);
	void initialiseProcessDemandTemporalList();
	void initialiseSelectedDemandTemporalList();

	void AllocateMemory(int n);
	void drawMultipleDemands(QListWidgetItem * items);
	void removeCurrentScenario();



//variables
    MeritTab *meritTab;
	DemandProfile *dsView;
	DemandProcess *dsProcess;
	DemandSelection *dsSelected;

	QGridLayout *gridLayout;

    QVBoxLayout *verticalLayout;
	QPushButton *addButton;
	QPushButton *delButton;

    QSpacerItem *horizontalSpacer_1; 
    QSpacerItem *horizontalSpacer_2; 
	QSpacerItem *horizontalSpacer_3; 

	QTreeView *tree;
	QMdiArea *mdiArea;
	QwtPlot *qwtPlot;
	QSplitter* splitterHrt;
	QSplitter* splitterVrt;
	DemandScenarioTreeModel* model;
    QWidget *widget;

	QHBoxLayout *horizontalLayout1; 
	QHBoxLayout *horizontalLayout2; 
	QVBoxLayout *verticalLayout1;

	PlotDemand *demandPlot;

	QString m_ItemName;
	QStringList m_DemandScenarioData;	
	QVector<float> demandData;

//interface: for the combobox list int the demand property dialog
	QStringList start_day_list;
	QStringList end_day_list;
	QStringList hour_steps_list;

//for standard available demand profiles
	QVector<float> demandDataVector;
	QVector<float> demandTotalVector;

	QList< QPair<QString, int> > demandStartDayList;
	QList< QPair<QString, int> > demandEndDayList;
	QList< QPair<QString, int> > demandHourStepsList;

	QVector <QString> ItemNameVector;

//to install the datetime data for the relevant demand 
	QVector<QString> demandDateDataVector;	
	int m_iStartDay;
	int m_iEndDay;
	int m_iHourSteps;

//for process demand
	QVector<float> processDemandDataVector;
	QVector<float> processDemandTotalVector;
	QVector <QVector <float> > processDemandVector;
	QVector <QString> processItemNameVector;
	QList<QListWidgetItem *> processItems;

	QList< QPair<QString, int> > prcDemandStartDayList;
	QList< QPair<QString, int> > prcDemandEndDayList;
	QList< QPair<QString, int> > prcDemandHourStepsList;

	QList<QPair<QString, QVector <float> > > processDemandList;

//for selected demands
	QList<QListWidgetItem *> ItemSelected;

	QList< QPair<QString, int> > selDemandStartDayList;
	QList< QPair<QString, int> > selDemandEndDayList;
	QList< QPair<QString, int> > selDemandHourStepsList;

	QList<QPair<QString, QVector <float> > > selectedDemandList;

	QVector <QVector <float> > DemandSelectedVector; 

//for demand scenario
//	DemandData* demandProfileData; //for the required data in demand profile
	DemandDataList demandProfileList; //for demand profile list of a specific demand scenario
    DemandDataListIterator dpIter; //iterator for the demand profile list

	QList<QPair<QString, DemandDataList > > demandScenarioPortfolio; //demand scenario portfolio
	QMap <QString, QString> mapDS; //to map between the name and the ds sequence
	QStringList demandScenarioName; //a list to store the names of the demand scenario

 signals:
//     void valueChanged(int newValue);
//const QMap <QString, QVariant> &Names
	 void valueChanged(QMap<QString, QString>  &newValue);
	 void valueChangedDSP(QList<QPair<QString, DemandDataList > > &newValue); //signal for dsp

protected slots:
//	void selectionChanged( const QItemSelection &selected, const QItemSelection &deselected );
//    void dataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight );
	void addDemandScenario();
	void deleteCurrentScenario();

	void drawDemandGraph();
	void drawSingleMiddleDemandGraph();
	void drawSingleRightDemandGraph();
	void drawRightDemandGraph();

	void moveMiddle();
	void moveRight();

	void removeMiddle();
	void removeRight();

	void LocateItem(const QPoint &pos);
	void locateProcessListItem(const QPoint &pos);
	void locateSelectedListItem(const QPoint &pos);
//	void showDemandInfo(QTableWidgetItem* item);
	void showDemandInfo();
	void showMiddleDemandInfo();
	void showRightDemandInfo();
//	void showDemandInfo1();
//	void drawRightDemandGraph();//QListWidgetItem *items);
//	void saveCurrentScenario();
//	void saveCurrentScenarioAs();

//slot for setting the name of the item clicked in the tablewidget and processlistwidget
	void setTableItemName(QTableWidgetItem *Item);
	void setListItemName(QListWidgetItem *Item);

//	QModelIndex ItemName();
//	void populateProfiles();
	void scaleDemandProfile();
	void shiftDemandProfile();
	void clearDemandProfile();
//	QPair<QString, QVector<float> > demandProfileData;

// save scenario
	void saveCurrentScenario();
	void saveCurrentScenarioAs();
//shift profile
	void combineDemandProfile();
//deploy profiles in scenario
	void deployProfiles();
//demand property
	void demandProperty();
//process demand property
	void procDemandProperty();
//selected demand property
	void selDemandProperty();
public:
	void setItemSelected(QTableWidgetItem * temp);
	QTableWidgetItem * getItemSelected();

//for selected demand
	void setListItemSelected(QListWidgetItem * temp);
	QListWidgetItem * getListItemSelected();

//for process demand
	void setProcessListItems(QListWidgetItem * temp);
	QListWidgetItem * getProcessListItems();

  //  void setCurrentTab();
//	void addDemandScenario();
//	void updateView();
//	void selectionChanged();

private:
	int m_DmdScnNo;
	QTableWidgetItem* m_ItemSelected;
	QListWidgetItem* m_ListItemSelected;
	QListWidgetItem* m_ProcessListItems;
};

#endif
