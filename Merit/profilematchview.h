#ifndef PROFILEMATCHVIEW_H
#define PROFILEMATCHVIEW_H
#include <QtCore>
#include <QtGui>
#include <QApplication>

#include <QWidget>
#include "mybutton.h"
#include <QAbstractButton>
#include <QPushButton>
#include <QTimer>
#include <QVector>
#include <QMap>
//#include <QBasicTimer>
#include "demanddata.h"
#include "supplydata.h"

class DemandData;
typedef  QList<DemandData> DemandDataList;
typedef  QList<DemandData>::Iterator DemandDataListIterator;

class SupplyData;
typedef  QList<SupplyData> SupplyDataList;
typedef  QList<SupplyData>::Iterator SupplyDataListIterator;

struct ProfileMatchViewData
{
	bool m_bDraw;
};

class ProfileMatchView : public QWidget
{
  Q_OBJECT

public:
	MyButton *myButton;
	MyButton *addButton;

	QFormLayout *formLayout1;
	QFormLayout *formLayout2;

	QGridLayout *gridLayout;
	//for demand scenario
	QVBoxLayout *verticalLayout1;
	QVBoxLayout *verticalLayout2;

	QHBoxLayout *horizontalLayout;

	QSpacerItem  *horizontalSpacer;
	QSpacerItem  *horizontalSpacer1;
	QSpacerItem  *horizontalSpacer2;

	MyButton *testButton;
	MyButton *testButton2;
	
	//QTimer *m_Timer;

	///
	QVector <QVector <int> > selectedDemandIndexVector;

	QVector <QVector <int> > selectedSupplyIndexVector;

	QVector< QPair <QVector <QVector <int> >, QVector <QVector <int> >> > selectedDSIndexVector;

	///
	bool m_bMouseOver;
	bool m_LeftDraw;
	bool m_RightDraw;
	bool m_bDraw;
	bool m_bDPIndexExist;
	bool m_bSPIndexExist;

	bool m_indexDPExist;
	bool m_indexSPExist;

	bool m_bPaintButton;
//control variables for drawing the graph, when the user double click the item in the table widget
	bool m_bFlag;

	int temp_index;


	QList <int> indexDPList;
	QList <int> indexSPList;

	QVector <QRect> demandButtonVector;
	QVector <QRect> supplyButtonVector;


	bool m_leftSelected;
	bool m_rightSelected;

	int flag;
	int cycle;
	int temp;
//	int tempD;

	int m_TimerStep;

	MyBtnList dPButtonList;
	MyBtnList sPButtonList;
	
	QList<QPair<int, int> > indexProfileSelList;

	QVector<int> dmdProfileSel;
	QVector<int> supProfileSel;

	QVector <QVector <int> > dmdProfileSelVector;
	QVector <QVector <int> > supProfileSelVector;

	QVector<QString> pMatchIdVector; //match id reference
	QVector<float> pMatchRateVector; //match rate
	QVector<int> pMatchRankVector; //ranking sequence based on match rate 
	QVector<float> pMatchICVector; //inequality coefficient
	QVector<float> pMatchCCVector; //correlation coefficient
	QVector<float> pMatchEDVector; //energy deficit
	QVector<float> pMatchESVector; //energy surplus
	QVector<float> pMatchTDVector; //total demand
	QVector<float> pMatchTSVector; //total supply


//	qMakePair

    ProfileMatchView( QWidget *parent=0 );
	ProfileMatchView(const ProfileMatchView &other);
	~ProfileMatchView();
	ProfileMatchView& operator=(const ProfileMatchView& other);

	void setupMatchIdVector(QVector<int> indexDPVector, QVector<int> indexSPVector);
	void setupMatchRateVector();
	void setupMatchRankVector();
	void setupMatchStatisticsVector(QVector<int> indexDPVector, QVector<int> indexSPVector); //set the vector for inequality coefficient
	void setupMatchCCVector(QVector<int> indexDPVector, QVector<int> indexSPVector); //set the vector for correlation coefficient
	void setupMatchEDVector(QVector<int> indexDPVector, QVector<int> indexSPVector); //set the vector for energy deficit
	void setupMatchESVector(QVector<int> indexDPVector, QVector<int> indexSPVector); //set the vector for energy surplus
	void setupMatchTDVector(QVector<int> indexDPVector); //set the vector for total demand
	void setupMatchTSVector(QVector<int> indexSPVector); //set the vector for total supply

//for auto search
	void setupMatchRankVector(int timestep);

	void bubbleSort(const QVector<QString>& MatchId, const QVector<float>& MatchRate, 
		QVector<QPair<QString, float> >& sortedMatchRate);//Bubble sort function 
//for auto search
	void bubbleSort(int timestep, const QVector<QString>& MatchId, const QVector<float>& MatchRate, 
		QVector<QPair<QString, float> >& sortedMatchRate);//Bubble sort function 

	void clearLayout(QLayout *layout);

//  QSize sizeHint() const;
 	void setTimerStep(int stp);
	int timerStep(); 
	void stopTimer();

	void InitialiseAllButtons();

	void drawDemandProfileButton(int idp, QList<QPair<QString, DemandDataList > >  &dp);
	void drawSupplyProfileButton(int isp, QList<QPair<QString, SupplyDataList > >  &sp);

//	bool operator!=(const QList<QPair<QString, DemandDataList > >  &other);

	int dsNumValue();
	int ssNumValue();

	QList<QPair<QString, DemandDataList > > demandProfileList();
	QList<QPair<QString, SupplyDataList > > supplyProfileList();

protected:
  void paintEvent( QPaintEvent* event);
//     void timerEvent(QTimerEvent *event);

    void DrawLine(const QRect& Rect1,const QRect& Rect2, const QPen& pPen);
	void DrawButton(const QRect& ButtonRect,const QBrush& pBrush);
	void setSupply(int n, bool Complete);
	void setDemand(int n, bool Complete);
	void PaintButtons(const QVector<int> &demandIndex, const QVector<int> &supplyIndex); 
	int GetSDCombinations(int i, int count, int combinations);

	
	int getDCombinations(int count, int combinations, int demand_combinations);
	int getCombinations(int supplycombinations, QVector<int>& supply_index_count, int count, int combinations);

//calculate match statistics
	double calculateInequalityCoefficient(const QVector<float>& Demand,const QVector<float>& Supply);//inequality coefficient
	double calculateSpearmansCorrelation(const QVector<float>& Demand,const QVector<float>& Supply); //correlation coefficient
	double calculateSurplusCurveAreas(const QVector<float>& Supply,const QVector<float>& Demand); //energy surplus
	double calculateDeficitCurveAreas(const QVector<float>& Supply,const QVector<float>& Demand); //energy deficit
	double calculateResidual(const QVector<float>& DemandData, const QVector<float>& SupplyData);// residual area, the rest of demand to be met by supply
	double calculateTotal(const QVector<float>& Data);
	double calculateMean(const QVector<float>& Data);

private:
	int mx, my;
	int m_indexCurrentDP;
	int m_indexCurrentSP;
	int m_CurrentDemand;
	int m_CurrentSupply;
    bool m_bDemandSelected;
	bool m_bSupplySelected;

    QTimer timer;

    int step;

//d pointer 
	ProfileMatchViewData *d;

	int m_iDemandProfile;
	int m_iSupplyProfile;

	QList<QPair<QString, DemandDataList > > m_pDemandProfileList;
	QList<QPair<QString, SupplyDataList > > m_pSupplyProfileList;

	int m_iDsValue; //which demand scenario;
	int m_iSsValue; //which supply scenario.

protected slots:
	void setDrawProfileMatch();
	void doDemandSearch();
	void setPaintButton();
	void doFullSearch();
	void slotTimeout();
	void resetProfileDraw(bool checked);

public slots:
//set the value of demand profiles for a specific demand scenario
	void setProfileValue(QList<QPair<QString, DemandDataList > > &value);
	void setSupplyProfileValue(QList<QPair<QString, SupplyDataList > > &value);
//set the value of supply profiles for a specific supply scenario
//	void setSSValue(int value);

//set the specific number of demand scenario
	void setDsNumValue(int value);
	void setSsNumValue(int value);

};

#endif 
