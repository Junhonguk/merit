#ifndef	MATCH_H
#define MATCH_H
#include <QtGui>
#include <QColorGroup>
#include <QWidget>
#include <QtCore/QTimer>
#include <QStandardItemModel>
#include "ui_match.h"
#include "merittab.h"
#include "profilematchview.h"
#include "drawmatchview.h"
#include "matchresultview.h"
//proxy filter model
#include "sortonsecondmodel.h"

//for the graph
#include "plot_match.h"

class QTimer;
class DrawMatchView;
class ProfileMatchView;
class MatchResultView;

class Match : public QWidget, public Ui::Match
{
    Q_OBJECT
public:
	Match(QWidget *parent = 0);
    MeritTab *meritTab;

	DrawMatchView *scnMatchView;
	ProfileMatchView *profileMatchView;
	MatchResultView *matchResultsTable;

	QWidget* profileWidget;

	QSplitter *splitterHrt;

	QWidget* widget;
	QWidget* widget1;
	QWidget* widgetProfile;

	QGroupBox *profileGroupBox;
	QGroupBox *matchResultGroupBox;
	
	QVBoxLayout *verticalLayout;
	QHBoxLayout *horizontalLayout;
	QHBoxLayout *horizontalLayout1;
	QHBoxLayout *horizontalLayoutProfile;
//	QHBoxLayout *horizontalLayout1;

	QPushButton *matchButton;
	QGridLayout *gridLayout_profile;
	QGridLayout *gridLayout_scenario;
	QGridLayout *matchResultGridLayout;

	QPushButton *searchButton;
	QPushButton *abortButton;
	QProgressBar *matchProgressBar;

    QSpacerItem *horizontalSpacer_1; 
    QSpacerItem *horizontalSpacer_2; 
	QSpacerItem *horizontalSpacer_3; 

//	QTableView *matchResultsTable;
//for graph
	QwtPlot *qwtPlot;
//	PlotMatch *matchPlot;
//	MyButton *myButton;
//QProxyFilterModel
	SortOnSecondModel *sorter;

//	void paintEvent(QPaintEvent *event);
//	void DrawButton(const CRect& DrawRect,CBrush* pBrush);
//	void DrawLine(const CRect& pRect1,const CRect& pRect2,CPen* pPen);

	bool m_bDrawProfileView;
	int profileDrawTime;
//	void AddMatchCombinations();
	void matchCombinations(int timestep);
	QStandardItemModel *model;

	int dsNumValue();
	int ssNumValue();

private:
	void createProgressBar();

protected:
	QTimer timer;
	int timestep2;

	QTimer timerMatchTable;

	QVector<int> pMatchRankVector;

	QVector<QString> pMatchIdVector;
	QVector<float> pMatchRateVector;
	QVector<float> pCorCoefVector;

	QVector<float> MatchResultVector;

	QTableWidgetItem *resultItem1;
	QTableWidgetItem *resultItem2;
	QTableWidgetItem *resultItem3;

	//to enable the paint event in profile match view
	bool m_bFlag;

	QList<QPair<QString, DemandDataList > > m_pTemp; //for demand scenario
	QList<QPair<QString, SupplyDataList > > m_pTempSupply; //for supply scenario

	int m_iDsValue;
	int m_iSsValue;

protected:
  void paintEvent( QPaintEvent* event);

 // void setDrawProfileView(bool bDraw);
signals:
  	 void valueChangedDSP(QList<QPair<QString, DemandDataList > > &newValue); //signal for demand scenario portfolio
	 void valueChangedSSP(QList<QPair<QString, SupplyDataList > > &newValue); //signal for supply scenario portfolio

public slots:
  //	 void setCurrentTab();
    void slotStart();
    void slotReset();
    void slotTimeout();
    void drawProfileView();
	void AddMatchCombinations();
	void deployMatchResults(const QModelIndex& index);
	void clearMatchResultsTable();
	void slotPause();
	void setNewModel();
	void setNewProxyModel();

	void setDsNumValue(int ds);
	void setSsNumValue(int ss);

	void setValue(QList<QPair<QString, DemandDataList > > &value);
	void setValue(QList<QPair<QString, SupplyDataList > > &value);

};

#endif
