#include <QStyle>
#include <QStyleOptionButton>
#include <QPainter>
#include <QBrush>
#include <QLayoutItem>
//#include <QTimer>
//#include <QAbstractButton>
#include "profilematchview.h"



ProfileMatchView::ProfileMatchView(QWidget *parent)
    : QWidget(parent)  //: public QSplitter
{
	d = new ProfileMatchViewData;

	m_bMouseOver = false;
	m_LeftDraw = false;
	m_RightDraw = false;
	m_bDraw = false;
	m_bPaintButton = false;
	m_bFlag = false;

	m_indexCurrentDP = 0;
	m_indexCurrentSP = 0;
	m_TimerStep = 0;
	temp_index = 0;

	m_iDsValue = 0;
	m_iSsValue = 0;

//	flag = 0;
//	cycle = 0;
		
	m_indexDPExist = false;
	m_indexSPExist = false;

	m_bDPIndexExist = false;
	m_bSPIndexExist = false;		 

	dPButtonList.clear();
	sPButtonList.clear();
	indexProfileSelList.clear();
	demandButtonVector.clear();
	supplyButtonVector.clear();
	selectedSupplyIndexVector.clear();
	pMatchIdVector.clear();
	pMatchRateVector.clear();
	dmdProfileSelVector.clear();
	supProfileSelVector.clear();

	gridLayout = new QGridLayout(this);

	//for demand profile
	verticalLayout1 = new QVBoxLayout();
   

	formLayout1 = new QFormLayout();

	
//	formLayout1->addLayout(verticalLayout1);

	//construct myButtonList with 3 elements

	formLayout2 = new QFormLayout();


	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	horizontalLayout = new QHBoxLayout();

    horizontalLayout->addItem(horizontalSpacer2);
	horizontalLayout->addLayout(formLayout1);
    horizontalLayout->addItem(horizontalSpacer);
	horizontalLayout->addLayout(formLayout2);
    horizontalLayout->addItem(horizontalSpacer1);

	gridLayout->addLayout(horizontalLayout,0,0,Qt::AlignLeft);
	setLayout(gridLayout);

//	m_Timer = new QTimer(this);


//for supply scenario, it will delete later
//	connect(m_Timer, SIGNAL(timeout()), this, SLOT(setPaintButton()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));	
//	m_Timer->start(10);
//	connect();

}


ProfileMatchView::ProfileMatchView(const ProfileMatchView& other )
{
	d = new ProfileMatchViewData( *other.d );
}

ProfileMatchView::~ProfileMatchView()
{
	delete d;
}

ProfileMatchView& ProfileMatchView::operator=( const ProfileMatchView& other )
{
	*d = *other.d;
	return *this;
}
/*
QSize ProfileMatchView::sizeHint() const
{
  return QSize( fontMetrics().width( text() )+10, fontMetrics().height()+10 );
}
*/

void ProfileMatchView::setPaintButton()
{
	m_bPaintButton = true;
}
void ProfileMatchView::paintEvent( QPaintEvent* event)
{
	QPainterPath path;

	QPoint pt2;
	QPoint pt4;
	QPoint pt1,pt3;
//draw the line to show the combination
	QPainter painter(this);
	if(m_bDraw)
	{
		PaintButtons(dmdProfileSel, supProfileSel);
		m_bDraw = false;
	}
//draw the profile match view when the user double click the item in the table widget
	if(m_bFlag)
	{
		PaintButtons(dmdProfileSelVector[temp_index], supProfileSelVector[temp_index]);
		m_bFlag = false;
	}
/*
	for (int k = 0; k < indexProfileSelList.count(); k++)
	{
		QRect Rect1 = QRect(dPButtonList[indexProfileSelList[k].first]->x(), dPButtonList[indexProfileSelList[k].first]->y(),
			dPButtonList[indexProfileSelList[k].first]->width(),dPButtonList[indexProfileSelList[k].first]->height());
		QRect Rect2 = QRect(sPButtonList[indexProfileSelList[k].second]->x(), sPButtonList[indexProfileSelList[k].second]->y(),
			sPButtonList[indexProfileSelList[k].second]->width(),sPButtonList[indexProfileSelList[k].second]->height());
		int h = Rect1.height();
		

		pt1=Rect1.bottomRight();		
		pt2.rx()=pt1.rx();
		pt2.ry()=pt1.ry()-h/2;

		pt3=Rect2.topLeft();		
		pt4.rx()=pt3.rx();
		pt4.ry()=pt3.ry()+h/2;

		path.moveTo(pt2);
		path.lineTo(pt4);
		QPen pen(Qt::red, 5);
		pen.setStyle(Qt::SolidLine);
		painter.setPen (pen);

		painter.drawLine(pt2, pt4);
//		PaintButtons(indexProfileSelList[k].first, indexProfileSelList[temp].second);
	}
*/
	int adf = 0;
	adf = m_TimerStep;

	if (!selectedSupplyIndexVector.empty() && !	selectedDemandIndexVector.empty())
	{

		temp = m_TimerStep % 60;
//		tempD = step % 3;

		if (flag == 0)
		{
			if (temp != 0)
			{
				flag = 0;
				m_TimerStep = -1;
				return;
			}
			else
			{
				temp = 0;
				m_TimerStep = 0;
				flag++;
			}
		}
		else 
		{
			flag++;	
			if (m_TimerStep < 45)
			{
				if (m_TimerStep>=0 && m_TimerStep < selectedSupplyIndexVector.size())
				{
					PaintButtons(selectedDemandIndexVector[temp], selectedSupplyIndexVector[temp]);
				}
			}
			else //if the combination is greater than the maximum, stop the timer.
			{
				timer.stop();
			}
		}
	}
		
// enable auto drawing profile match view 	



//	flag = 0;
	
	
	
/*
	for (int j=0; j < temp; j++)
	{	
		PaintButtons(selectedSupplyIndexVector[temp]);	
//		temp = step;
//		j=selectedSupplyIndexVector.size();
	}
*/

}

//	gridLayout->addLayout(verticalLayout2,0,1,Qt::AlignLeft);
//	gridLayout->addItem(horizontalSpacer,0,1);

//	gridLayout->addItem(horizontalSpacer);

/*
	painter.setRenderHint(QPainter::Antialiasing, true);	
	QStyleOptionButton option;
	option.init( addButton );
	option.state |= QStyle::State_Raised;
	style()->drawControl( QStyle::CE_PushButtonBevel, &option, &painter, addButton);  
*/
/*
	
//	
//	QRect buttonRect=rect();
//	painter.setPen(Qt::black);
//	
	QRect buttonRect=rect();

	myButton = new MyButton;
	addButton = new QPushButton("Add");
//	matchView->addWidget(myButton);

	QPainter painter(addButton);
	painter.setRenderHint(QPainter::Antialiasing, true);	
	QStyleOptionButton option;
	option.init( addButton );
	option.state |= QStyle::State_Raised;
	style()->drawControl( QStyle::CE_PushButtonBevel, &option, &painter, addButton);   
	addButton->show();
*/
//	painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
//	painter.drawRoundRect(buttonRect);

//	MyButton::paintEvent(event);
//	painter.drawRect( 10,80,150,50);
//	painter.drawRoundRect(buttonRect,25,25);
//	painter.drawText(0,0,6,2, Qt::AlignRight | Qt::AlignVCenter,QString("Button"),&buttonRect);	

//	myButton = new MyButton();
//	myButton->paintEvent(event);
//   myButton = new MyButton;

/*
  QPainter painter( this );
  painter.setPen( Qt::black );
  painter.setFont( font() );
  painter.setBrush(Qt::red);

  QStyleOptionButton option;
  option.init( this );
  option.state |= QStyle::State_Raised;
    
  style()->drawControl( QStyle::CE_PushButtonBevel, &option, &painter, this );   
*/
 // painter.setFont( font() ); 
 // painter.setPen( Qt::black );
 // painter.setBrush(Qt::red);
 
 // painter.drawRect( 10,80,150,50);//, Qt::AlignCenter, "draw inside");//myButton->text() );
//}


//prepare the match id vector beforehand 
void ProfileMatchView::setupMatchIdVector(QVector<int> indexDPVector, QVector<int> indexSPVector)
{
	QString dTemp = " ";
	QString sTemp = " ";
	QString dsMatchString = " ";
//	dmdProfileSelVector.clear();

	for (int i=0; i<indexDPVector.size(); i++)
	{
		dTemp += "D" + QString::number(indexDPVector[i]);
	}

	for (int i=0; i<indexSPVector.size();i++)
	{
		sTemp +="S" + QString::number(indexSPVector[i]);
	}

	dsMatchString = dTemp +"/" +sTemp;

	if (!pMatchIdVector.empty())
	{
		if(!pMatchIdVector.contains(dsMatchString))
		{
			pMatchIdVector.append(dsMatchString);
			dmdProfileSelVector.append(indexDPVector);
			supProfileSelVector.append(indexSPVector);
		}
	}
	else
	{
		pMatchIdVector.append(dsMatchString);
		dmdProfileSelVector.append(indexDPVector);
		supProfileSelVector.append(indexSPVector);
	}	
//	pMatchRankVector(pMatchIdVector.size());
}

//prepare the match rate vector beforehand 
void ProfileMatchView::setupMatchRateVector()
{
	float temp = 0.0;

	for (int i=0; i<pMatchIdVector.size(); i++)
	{
		temp = (1 - pMatchICVector[i]) * 100;
		pMatchRateVector.append(temp);
	}
}

//prepare the match inequality coefficient vector beforehand 
void ProfileMatchView::setupMatchStatisticsVector(QVector<int> indexDPVector, QVector<int> indexSPVector)
{
	float tempIC = 0.0;
	float tempCC = 0.0;
	float tempTD = 0.0;
	float tempTS = 0.0;
	float tempED = 0.0;
	float tempES = 0.0;
	QVector<float> tempTotalDemand;
	QVector<float> tempTotalSupply;
	
	tempTotalDemand.clear();
	tempTotalSupply.clear();

//calculate the total demand and push back to the tempTotalDemand vector
	for (int i = 0; i <  m_pDemandProfileList[m_iDsValue].second[0].m_pDemandDataVector.size(); i++)
	{
		tempTD = 0.0;
		tempTS = 0.0;
		for (int j = 0; j < indexDPVector.size(); j++)
		{
			tempTD = + m_pDemandProfileList[m_iDsValue].second[indexDPVector[j]].m_pDemandDataVector[i];
		}
		tempTotalDemand.append(tempTD);
		for (int k = 0; k < indexSPVector.size(); k++)
		{
			tempTS = + m_pSupplyProfileList[m_iSsValue].second[indexSPVector[k]].m_pSupplyDataVector[i];
		}
		tempTotalSupply.append(tempTS);
	}


	for (int i=0; i<pMatchIdVector.size(); i++) //the total number of combinations
	{

		tempIC = calculateInequalityCoefficient(tempTotalDemand, tempTotalSupply); //inequality coefficient
		tempCC = calculateSpearmansCorrelation(tempTotalDemand, tempTotalSupply); //correlation coefficient
		tempED = calculateDeficitCurveAreas(tempTotalDemand, tempTotalSupply); //energy deficit
		tempES = calculateSurplusCurveAreas(tempTotalDemand, tempTotalSupply); //energy surplus
		tempTD = calculateTotal(tempTotalDemand); //total demand
		tempTS = calculateTotal(tempTotalSupply); //total supply
		
		pMatchICVector.append(tempIC);
		pMatchCCVector.append(tempCC);
		pMatchEDVector.append(tempED);
		pMatchESVector.append(tempES);
		pMatchTDVector.append(tempTD);
		pMatchTSVector.append(tempTS);
	}
}

//Theils Coefficient where 0=perfect match, 1=worst match
double ProfileMatchView::calculateInequalityCoefficient(const QVector<float>& Demand,const QVector<float>& Supply)
{
	double NumeratorTerm=0.0;
	double DemandSquared=0.0;
	double SupplySquared=0.0;
	double m_InequalityCoeff = 0.0;

	int TimeSteps = (int)Demand.size();
	for (int i=0;i<TimeSteps;++i)
	{
		NumeratorTerm += pow(Demand[i]-Supply[i],2);
		DemandSquared += pow(Demand[i],2);
		SupplySquared += pow(Supply[i],2);
	}
	double Numerator=pow(NumeratorTerm/TimeSteps,0.5);
	double DemandTerm=pow(DemandSquared/TimeSteps,0.5);
	double SupplyTerm=pow(SupplySquared/TimeSteps,0.5);

	m_InequalityCoeff=Numerator/(DemandTerm+SupplyTerm);

	return m_InequalityCoeff;
}


//for correlation coefficient
double ProfileMatchView::calculateSpearmansCorrelation(const QVector<float>& Demand,const QVector<float>& Supply)
{
	int TimeSteps = (int)Demand.size();
	double SupplyMean = 0.0;
	double DemandMean = 0.0;
	double m_CorrCoeff = 0.0;
	SupplyMean = calculateMean(Supply);
	DemandMean = calculateMean(Demand);
	double SSS,SSD,SSSD;
	SSS=SSD=SSSD=0;
	for (int i=0;i<TimeSteps;++i)
	{
		SSS+=pow(Supply[i]-SupplyMean,2);
		SSD+=pow(Demand[i]-DemandMean,2);
		SSSD+=(Supply[i]-SupplyMean)*(Demand[i]-DemandMean);
	}
	double Denom=pow(SSS*SSD,0.5);
	m_CorrCoeff=SSSD/Denom;
	if ((SSSD==0)&&(Denom==0)&&(SupplyMean<=1.00001f*DemandMean)&&
		(SupplyMean>=0.999999f*DemandMean))
	{
		m_CorrCoeff=1.0f;	
	}
	return m_CorrCoeff;
}

///energy deficit//not eough supply//
double ProfileMatchView::calculateDeficitCurveAreas(const QVector<float>& Supply,const QVector<float>& Demand)
{
	double SupplyArea=0;
	double DemandArea=0;
	double m_SharedArea=0;
	float SharedPoint1,SharedPoint2;

	float Deficit1, Deficit2; //not enough supply
	float DeficitArea = 0;//save the results of deficit area

	//integrating under curves using trapezium rule
	int TimeSteps = (int)Demand.size();
	for (int i=0;i<TimeSteps-1;++i)
	{
		SupplyArea += (Supply[i]+Supply[i+1])/2;
		DemandArea += (Demand[i]+Demand[i+1])/2;
		//find shared area
		if (Supply[i]>Demand[i])
		{
			SharedPoint1=Demand[i];
			Deficit1 = Demand[i] - Supply[i];
		}
		else
		{
			SharedPoint1=Supply[i];
			Deficit1 = Demand[i] - Supply[i];
		}

		if (Supply[i+1]>Demand[i+1])
		{
			SharedPoint2=Demand[i+1];
			Deficit2 = Demand[i+1] - Supply[i+1];
		}
		else 
		{
			SharedPoint2=Supply[i+1];
			Deficit2 =  Demand[i+1] - Supply[i+1];
		}
		m_SharedArea += (SharedPoint1+SharedPoint2)/2;

//calculate deficit area
		if (Deficit1 >= 0 && Deficit2 >=0)
		{
			DeficitArea += (Deficit1+Deficit2)/2.0;
		}
		else if (Deficit1 >= 0 && Deficit2 < 0)
		{
			DeficitArea += Deficit1/4.0;
		}
		else if (Deficit1 < 0 && Deficit2 >=0)
		{
			DeficitArea += Deficit2/4.0;
		}
		else
			DeficitArea += 0.0;
	}

	return DeficitArea;
}

///energy surplus//sufficient supply//
double ProfileMatchView::calculateSurplusCurveAreas(const QVector<float>& Supply,const QVector<float>& Demand)
{
	double SupplyArea=0;
	double DemandArea=0;
	double m_SharedArea=0;
	float SharedPoint1,SharedPoint2;

	float Surplus1, Surplus2; //enough supply
	float SurplusArea = 0; //save the results of surplus area

	//integrating under curves using trapezium rule
	int TimeSteps = (int)Demand.size();
	for (int i=0;i<TimeSteps-1;++i)
	{
		SupplyArea += (Supply[i]+Supply[i+1])/2;
		DemandArea += (Demand[i]+Demand[i+1])/2;
		//find shared area
		if (Supply[i]>Demand[i])
		{
			SharedPoint1=Demand[i];
			Surplus1 = Supply[i] - Demand[i];
		}
		else
		{
			SharedPoint1=Supply[i];
			Surplus1 = Supply[i] - Demand[i];
		}
		if (Supply[i+1]>Demand[i+1])
		{
			SharedPoint2=Demand[i+1];
			Surplus2 = Supply[i+1] - Demand[i+1];
		}
		else 
		{
			SharedPoint2=Supply[i+1];
			Surplus2 = Supply[i+1] - Demand[i+1];
		}
		m_SharedArea += (SharedPoint1+SharedPoint2)/2;

	//calculate the surplus area
		if (Surplus1 >= 0 && Surplus2 >=0)
		{
			SurplusArea += (Surplus1+Surplus2)/2.0;
		}
		else if (Surplus1 >= 0 && Surplus2 <0)
		{
			SurplusArea += (Surplus1)/4.0;
		}
		else if (Surplus1 < 0 && Surplus2 >=0)
		{
			SurplusArea += (Surplus2)/4.0;
		}
		else
			SurplusArea += 0.0;

	}
	return SurplusArea;
}

//residual area
double ProfileMatchView::calculateResidual(const QVector<float>& DemandData, const QVector<float>& SupplyData)
{
	double m_ResidualAverage=0.0;
	int TimeSteps = (int)DemandData.size();
	for (int i=0;i<TimeSteps;++i)
	{
		m_ResidualAverage += (DemandData[i]-SupplyData[i]);
	}

	m_ResidualAverage/=TimeSteps;
	return m_ResidualAverage;
}


double ProfileMatchView::calculateMean(const QVector<float>& Data)
{
	double Mean=0.0f;
	int TimeSteps = (int)Data.size();
	for (int i=0;i<TimeSteps;++i)
	{
		Mean+=Data[i];
	}
	Mean/=TimeSteps;
	return Mean;
}
double ProfileMatchView::calculateTotal(const QVector<float>& Data)
{
	double Mean=0.0f;
	int TimeSteps = (int)Data.size();
	for (int i=0;i<TimeSteps;++i)
	{
		Mean+=Data[i];
	}
	return Mean;
}
////////////////////////////////////
void ProfileMatchView::bubbleSort(const QVector<QString>& MatchId, const QVector<float>& MatchRate, QVector<QPair<QString, float> >& sortedMatchRate)//Bubble sort function 
{
    int i,j;
	QPair <QString, float> temp;
//construct the vector for sortedMatchRate
	for (i = 0; i < MatchId.size(); i++)
	{
		sortedMatchRate.append(qMakePair(MatchId[i], MatchRate[i]));
	}
//	sortedMatchRate = originalMatchRate;
    for(i=0;i<sortedMatchRate.size();i++)
    {
        for(j=0;j<i;j++)
        {
            if(sortedMatchRate[i].second>sortedMatchRate[j].second)
            {
                temp=sortedMatchRate[i]; //swap 
                sortedMatchRate[i]=sortedMatchRate[j];
                sortedMatchRate[j]=temp;
            }
        }
    }
//	sortedMatchRateVector = matchRate;	
}

void ProfileMatchView::bubbleSort(int timestep, const QVector<QString>& MatchId, const QVector<float>& MatchRate, QVector<QPair<QString, float> >& sortedMatchRate)//Bubble sort function 
{
    int i,j;
	QPair <QString, float> temp;
//construct the vector for sortedMatchRate
	for (i = 0; i < timestep; i++)
	{
		sortedMatchRate.append(qMakePair(MatchId[i], MatchRate[i]));
	}
//	sortedMatchRate = originalMatchRate;
    for(i=0;i<sortedMatchRate.size();i++)
    {
        for(j=0;j<i;j++)
        {
            if(sortedMatchRate[i].second>sortedMatchRate[j].second)
            {
                temp=sortedMatchRate[i]; //swap 
                sortedMatchRate[i]=sortedMatchRate[j];
                sortedMatchRate[j]=temp;
            }
        }
    }
//	sortedMatchRateVector = matchRate;	
}
///////////////////////////////////
//prepare the match ranking vector beforehand based on the match index 
void ProfileMatchView::setupMatchRankVector()
{
	int temp = 0;
	QVector< QPair <QString, float> > pSortedMatchRateVector;
	QVector< QPair <QString, float> > pOriginalMatchRateVector;

	pSortedMatchRateVector.clear();
	pOriginalMatchRateVector.clear();

	pMatchRankVector.clear();
	pMatchRankVector.reserve(pMatchIdVector.size());
	pMatchRankVector.resize(pMatchIdVector.size());

	for (int i=0; i<pMatchIdVector.size(); i++)
	{
		pOriginalMatchRateVector.append(qMakePair(pMatchIdVector[i], pMatchRateVector[i]));
	}

	bubbleSort(pMatchIdVector, pMatchRateVector, pSortedMatchRateVector);

	for (int i=0; i<pMatchIdVector.size(); i++)
	{
		pMatchRankVector[i] = pSortedMatchRateVector.indexOf(pOriginalMatchRateVector[i])+1;
	}
}

void ProfileMatchView::setupMatchRankVector(int timestep)
{
	int temp = 0;
	QVector< QPair <QString, float> > pSortedMatchRateVector;
	QVector< QPair <QString, float> > pOriginalMatchRateVector;

	pSortedMatchRateVector.clear();
	pOriginalMatchRateVector.clear();

	pMatchRankVector.clear();
	pMatchRankVector.reserve(timestep);
	pMatchRankVector.resize(timestep);

	for (int i=0; i<timestep; i++)
	{
		pOriginalMatchRateVector.append(qMakePair(pMatchIdVector[i], pMatchRateVector[i]));
	}

	bubbleSort(timestep, pMatchIdVector, pMatchRateVector, pSortedMatchRateVector);

	for (int i=0; i<timestep; i++)
	{
		pMatchRankVector[i] = pSortedMatchRateVector.indexOf(pOriginalMatchRateVector[i])+1;
	}
}

void ProfileMatchView::setDrawProfileMatch()
{
	dmdProfileSel.clear();
	supProfileSel.clear();
	m_LeftDraw = false;
	m_RightDraw = false;

//for demand buttons
//check the mouse press area is within the defined button range 
	for (int i = 0; i < dPButtonList.count(); i++)
	{
		if(dPButtonList[i]->isChecked()) //if it is checked
		{
			dmdProfileSel.append(i);
			m_LeftDraw = true;
		}
	}

//for supply buttons
//check the mouse press area is within the defined button range 
	for (int i = 0; i < sPButtonList.count(); i++)
	{
		if(sPButtonList[i]->isChecked()) //if it is checked
		{
			supProfileSel.append(i);
			m_RightDraw = true;
		}
	}


	if (m_LeftDraw && m_RightDraw)
	{
//		
		m_bDraw = true;
	}
	else
	{
		m_bDraw = false;
	}

	update();
}


void ProfileMatchView::resetProfileDraw(bool checked)
{
	if (!checked)
	{
//set all the demand scenario buttons to unchecked
		for (int i = 0; i < dPButtonList.count(); i++)
		{
			if(dPButtonList[i]->isChecked())
			{
				dPButtonList[i]->setChecked(false);
			}
		}

//set all the supply scenario buttons to unchecked 
		for (int i = 0; i < sPButtonList.count(); i++)
		{
			if(sPButtonList[i]->isChecked()) //if it is checked
			{
				sPButtonList[i]->setChecked(false);
			}
		}
		update();
	}
}


/////////////////////////////////////////////////////////
//searches every match for each demand exclusivly
void ProfileMatchView::doDemandSearch()
{
	int nDemands=dPButtonList.count();
	int count=-1;
	selectedSupplyIndexVector.clear();
//	flag = 0;
    step = 0;
	temp = 0;
    timer.start(500);
//	m_Timer->start(1000);
	for (int i=0;i<nDemands;i++)
	{
		int combinations = 1;
		do{
			count = GetSDCombinations(i, count, combinations);
			++combinations;
		}while (count > 0);		
	}
//	update();
}


//get the total number of the combinations between supply and demand
int ProfileMatchView::GetSDCombinations(int i, int count, int combinations)
{
	int combinations_check = count;
	int inc_index = combinations-1;
	int start;

	QVector<int> index_count;
	//set up the counting index vector
	for (int j=0; j<combinations; ++j)
	{
		index_count.push_back(j);
	}

//nRE - represents the number of supply profiles
	int nRE = (int)sPButtonList.count();

	for (int a=0; a<nRE; ++a)
	{
//		m_Timer->start(1000);
		if (index_count[combinations-1]<nRE)
		{
			do{
				m_CurrentDemand=i;
				setDemand(i,TRUE); //this is the function to paint the demand button
//				TRACE("demand:%d\n", i);

				//set the relevant supply checked...
				for (int k=0; k<combinations; ++k)
				{
					setSupply(index_count[k],TRUE); //this is the function to paint the supply button
//					TRACE("%d,", index_count[k]);								
				}
		    //make pair the supply and demand combinations and save to a defined vector
				selectedSupplyIndexVector.push_back(index_count);

//				TRACE("\n");
/////////////////////////////////////////////////
//leave this function will implement the match function later
//				GetMatch(true);
///////////////////////////////////////////////////////////
				count++;
				++index_count[combinations-1];
			//////////////	
				int demo = index_count[combinations-1];
				///////////

				if (index_count[combinations-1]==nRE)
				{
					if (index_count[inc_index]!=nRE)
					{
						start = index_count[inc_index];
						++start;
						if (start+combinations-inc_index > nRE)
						{
							--inc_index;
							start = index_count[inc_index];
							++start;
							if (start+combinations-inc_index > nRE)
							{
								--inc_index;
								start = index_count[inc_index];
								++start;
							}
						}
						if (inc_index+combinations<=nRE)
						{
							for (int j=inc_index; j<inc_index+combinations; ++j)
							{
								if (j<combinations)
								{
									index_count[j]= start+j-inc_index;
								}
							}
						}
					}
					else
					{
						--inc_index;
						if (inc_index>=0 && inc_index <= combinations)
						{
							++index_count[inc_index];
							start = index_count[inc_index];
							for (int j=inc_index; j<inc_index+combinations; ++j)
							{
								if (j<combinations)
								{
									index_count[j]= start+j-inc_index;
								}
							}
						}
						else
						{
							++inc_index;
							++index_count[inc_index];
							start = index_count[inc_index];
							for (int j=inc_index; j<inc_index+combinations; ++j)
							{
								if (j<combinations)
								{
									index_count[j]= start+j-inc_index;
								}
							}
						}
					}
				}
				if (inc_index == 0)
				{
					inc_index = combinations-1;
				}
			}while(nRE>index_count[combinations-1]);
		}
//		m_Timer->stop(); //emit timeout signal
		
//		QTimer::singleShot(1000, this, SLOT(update()));
//		m_bPaintButton = true;
//		repaint(this->dPButtonL);
//		repaint(this->sPButtonList);
	}
	if (combinations_check == count)
	{
		count = 0;
	}
	return count;
}

void ProfileMatchView::setDemand(int n, bool Complete)
{
	if (m_CurrentDemand==n)
	{
		//doubleclicked demand to clear
		InitialiseAllButtons();//clear the state of buttons
	}
	dPButtonList[n]->setChecked(true); 
	m_CurrentDemand=n;
	if (Complete)
	{
	  m_bPaintButton = true;
//	  update();
	}

}


void ProfileMatchView::setSupply(int n, bool Complete)
{
	if (m_CurrentSupply==n)
	{//double clicked supply to clear
		InitialiseAllButtons();//clear highlights
	}
	sPButtonList[n]->setChecked(true); 
	m_CurrentSupply=n;
	if (Complete)
	{
	  m_bPaintButton = true;
//	  update();
	}
}


void ProfileMatchView::InitialiseAllButtons()
{
	for (int i=0;i<dPButtonList.count();++i)
	{
		dPButtonList[i]->setChecked(false);
	}
	for (int i=0;i<sPButtonList.count();++i)
	{
		sPButtonList[i]->setChecked(false);
	}	
//leave the color for the moment
/*			
	++m_ColorIndex;
	if (m_ColorIndex>64)
	{
		m_ColorIndex=0;
	}
*/
////////////////////////////
	m_CurrentDemand=-1;
	m_CurrentSupply=-1;

//redraw the updated widgets
	update();
}

void ProfileMatchView::PaintButtons(const QVector<int> &demandIndex, const QVector<int> &supplyIndex) 
{
//	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QPen pen(Qt::red, 3);
	QBrush brush(Qt::yellow);

	//run through all the buttons and highlight those selected.
	for (int i=0;i<dPButtonList.count();++i)
	{
		QRect Rect1 = QRect(dPButtonList[i]->x(), dPButtonList[i]->y(),
			dPButtonList[i]->width(),dPButtonList[i]->height());
		for (int q = 0; q < demandIndex.size(); q++)
		{
			if (i == demandIndex[q])
			{
				m_bDemandSelected = true;
				DrawButton(Rect1,brush);
				int nSupplies=0;
				//lines between demand & supply
				for (int j=0;j<sPButtonList.count();++j)
				{
					QRect Rect2 = QRect(sPButtonList[j]->x(), sPButtonList[j]->y(),
						sPButtonList[j]->width(),sPButtonList[j]->height());
					for (int q = 0; q < supplyIndex.size(); q++)
					{
						if (j == supplyIndex[q])
						{
							++nSupplies;
							DrawLine(Rect1,Rect2,pen);
						}
					}
				}
			}
		}
	}

// run through all the supply button and highlight those selected
	for (int i=0;i<sPButtonList.count();++i)
	{
		QRect Rect2 = QRect(sPButtonList[i]->x(), sPButtonList[i]->y(),
			sPButtonList[i]->width(),sPButtonList[i]->height());
		for (int q = 0; q < supplyIndex.size(); q++)
		{
			if (i == supplyIndex[q])
			{
				m_bSupplySelected = true;
				DrawButton(Rect2,brush);
			}
		}
	}

//	QApplication::restoreOverrideCursor(); 
}


void ProfileMatchView::DrawLine(const QRect& Rect1,const QRect& Rect2, const QPen& pPen)
{
	QPainterPath path;
	QPainter paint(this);

	paint.begin(this); //start painting

	//Sort Left to right order
	QPoint pt1,pt3;

	int h = Rect1.height();//all buttons the same height
	if (Rect1.bottomRight().x() < Rect2.bottomRight().x())
	{
		pt1=Rect1.bottomRight();
		pt3=Rect2.topLeft();
	}
	else
	{
		pt1=Rect2.bottomRight();
		pt3=Rect1.topLeft();
	}

	QPoint pt2;
	pt2.setX(pt1.x());
	pt2.setY(pt1.y()-h/2);
	QPoint pt4;
	pt4.setX(pt3.x());
	pt4.setY(pt3.y()+h/2);

	path.moveTo(pt2);
	path.lineTo(pt4);

	paint.setPen(pPen);
	paint.drawPath(path);

	paint.end(); //release the paint device
}

void ProfileMatchView::DrawButton(const QRect& ButtonRect, const QBrush& pBrush) 
{
	QRect DrawRect = ButtonRect;
	
	DrawRect.adjust(-1,-1,1,1); //draw a inflated rectangle

	QPainter paint(this);

	paint.begin(this); //start painting
	paint.setBrush(pBrush);

	paint.drawRect(DrawRect);
	paint.end(); //release paint device resource

}
/*
 void ProfileMatchView::timerEvent(QTimerEvent *event)
 {
     if (event->timerId() == timer.timerId())
	 {
         ++step;
         update();
//		 repaint(this->rect());
     } 
	 else 
	 {
         QWidget::timerEvent(event);
     }
 }

*/
//////////////////////////////////////////////////////
/////////////////Do Full Search//////////////////////
////////////////////////////////////////////////////

void ProfileMatchView::doFullSearch()
{
	int nDemands = dPButtonList.count();
	int nSupplys= sPButtonList.count();

	int count=-1;
//	flag = 0;
    step = 0;
	temp = 0;
//	tempD = 0;
    timer.start(500);
	selectedSupplyIndexVector.clear();
	selectedDemandIndexVector.clear();


//	InitialiseAllButtons();
	m_bDraw = false;

//	repaint();
	for (int j=0;j<nSupplys;j++)
	{
		int combinations = 1;
		do{
			count = getDCombinations(count, combinations, j+1);
			++combinations;
		}while (count > 0);
	}

//	InitialiseAllButtons();
}

/////
int ProfileMatchView::getDCombinations(int count, int combinations, int demand_combinations)
{
	int combinations_check = count;
	int inc_index = combinations-1;
	int start;

	QVector<int> index_count;
	//set up the counting index vector
	for (int j=0; j<combinations; ++j)
	{
		index_count.push_back(j);
	}
	int nSupplys = (int)sPButtonList.count();
	for (int a=0; a<nSupplys; ++a)
	{
		if (index_count[combinations-1]<nSupplys)
		{
			int demand_count=0;
			do{
				getCombinations(combinations, index_count, demand_count, demand_combinations);
				count++;
				++index_count[combinations-1];
				if (index_count[combinations-1]==nSupplys)
				{
					if (index_count[inc_index]!=nSupplys)
					{
						start = index_count[inc_index];
						++start;
						if (start+combinations-inc_index > nSupplys)
						{
							--inc_index;
							start = index_count[inc_index];
							++start;
							if (start+combinations-inc_index > nSupplys)
							{
								--inc_index;
								start = index_count[inc_index];
								++start;
							}
						}
						if (inc_index+combinations<=nSupplys)
						{
							for (int j=inc_index; j<inc_index+combinations; ++j)
							{
								if (j<combinations)
								{
									index_count[j]= start+j-inc_index;
								}
							}
						}
					}
					else
					{
						--inc_index;
						if (inc_index>=0 && inc_index <= combinations)
						{
							++index_count[inc_index];
							start = index_count[inc_index];
							for (int j=inc_index; j<inc_index+combinations; ++j)
							{
								if (j<combinations)
								{
									index_count[j]= start+j-inc_index;
								}
							}
						}
						else
						{
							++inc_index;
							++index_count[inc_index];
							start = index_count[inc_index];
							for (int j=inc_index; j<inc_index+combinations; ++j)
							{
								if (j<combinations)
								{
									index_count[j]= start+j-inc_index;
								}
							}
						}
					}
				}
				if (inc_index == 0)
				{
					inc_index = combinations-1;
				}
			}while(nSupplys>index_count[combinations-1]);
		}
	}
	if (combinations_check == count)
	{
		count = 0;
	}
	return count;
}

////////////////////////////////////////////////////////////////////////
//get the combinations
//input parameters:
//////////supplycombinations - the type of supply combination
//////////supply_index_count - a vector storing the index of supplies
//////////combinations - the type of demand combination
///////// count - the number of the combinations

//return parameters:
//////////count - the total number of the combinations
/////////////////////////////////////////////////////////////////////

int ProfileMatchView::getCombinations(int supplycombinations, QVector<int>& supply_index_count, int count, int combinations)
{
	int combinations_check = count;
	int inc_index = combinations-1;
	int start;
	QVector<int> index_count;

//set up the counting index vector
	for (int j=0; j<combinations; ++j)
	{
		index_count.push_back(j);
	}

//the total number of the available demand profiles
	int nDemands = (int)dPButtonList.count();
	for (int a=0; a<nDemands-1; ++a)
	{
		if (index_count[combinations-1]<nDemands)
		{
			do{
				m_CurrentSupply=supply_index_count[0]; //index of the current supply
				for (int k=0; k<supplycombinations; ++k) //set the supplies checked for the current combination
				{
					setSupply(supply_index_count[k],TRUE);
				}
//save the index of supply
				selectedSupplyIndexVector.push_back(supply_index_count);

				for (int k=0; k<combinations; ++k) //set the demand checked for the current demand combination
				{
					setDemand(index_count[k],TRUE);
				}
//save the index of the demand
				selectedDemandIndexVector.push_back(index_count);
////////////////////////////////////////////////////////////////////////////
//set up match vector for search function
				setupMatchIdVector(index_count,supply_index_count);
				setupMatchRateVector();
				setupMatchRankVector();
///////////////////////////////////////////////////////////////////////////
////////////////////////////////
//implement this function later, when the data is available
//				GetMatch(true);
/////////////////////////////////

				count++;
				++index_count[combinations-1];

				if (index_count[combinations-1]==nDemands)
				{
					if (index_count[inc_index]!=nDemands)
					{
						start = index_count[inc_index];
						++start;
						if (start+combinations-inc_index > nDemands)
						{
							--inc_index;
							start = index_count[inc_index];
							++start;
							if (start+combinations-inc_index > nDemands)
							{
								--inc_index;
								start = index_count[inc_index];
								++start;
							}
						}
						if (inc_index+combinations<=nDemands)
						{
							for (int j=inc_index; j<inc_index+combinations; ++j)
							{
								if (j<combinations)
								{
									index_count[j]= start+j-inc_index;
								}
							}
						}
					}
					else
					{
						--inc_index;
						if (inc_index>=0 && inc_index <= combinations)
						{
							++index_count[inc_index];
							start = index_count[inc_index];
							for (int j=inc_index; j<inc_index+combinations; ++j)
							{
								if (j<combinations)
								{
									index_count[j]= start+j-inc_index;
								}
							}
						}
						else
						{
							++inc_index;
							++index_count[inc_index];
							start = index_count[inc_index];
							for (int j=inc_index; j<inc_index+combinations; ++j)
							{
								if (j<combinations)
								{
									index_count[j]= start+j-inc_index;
								}
							}
						}
					}
				}
				if (inc_index == 0)
				{
					inc_index = combinations-1;
				}
			}while(nDemands>index_count[combinations-1]);
		}
	}
	if (combinations_check == count)
	{
		count = 0;
	}
	return count;
}

void ProfileMatchView::slotTimeout()
{
	m_TimerStep++;
	setTimerStep(m_TimerStep);
	update();
}

void ProfileMatchView::setTimerStep(int stp)
{
	m_TimerStep = stp;
}

int ProfileMatchView::timerStep()
{	
	return m_TimerStep;
}

void ProfileMatchView::stopTimer()
{
	timer.stop();
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////

//for demand profile
void ProfileMatchView::setProfileValue(QList<QPair<QString, DemandDataList > > &value) 
{
	if (value != m_pDemandProfileList) 
	{
		m_pDemandProfileList = value;
//		emit valueChanged(value);
	}
//	m_iDemandScenario = m_pDemandProfileList.count();
//	drawDemandScenarioButton(m_iDemandScenario);

	drawDemandProfileButton(m_iDsValue, m_pDemandProfileList);

}

void ProfileMatchView::drawDemandProfileButton(int ids, QList<QPair<QString, DemandDataList > >  &dp)
{
	QString temp2 = "";
	dPButtonList.clear();
//	for (int j =0; j < formLayout1->count(); j++)
//		formLayout1->takeAt(j);

    QLayoutItem *item;
    while((item = formLayout1->takeAt(0))) 
	{
        if (item->layout()) 
		{
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) 
		{
            delete item->widget();
        }
        delete item;
    }

	for (int i =0; i < dp[ids].second.count(); i++)
	{
		temp2 = dp[ids].second[i].demandName();

		testButton = new MyButton;
		testButton->setText(temp2);
		testButton->setCheckable(true);
		dPButtonList.append(testButton);
		connect(dPButtonList[i], SIGNAL(clicked()), this, SLOT(setDrawProfileMatch()));
		connect(dPButtonList[i], SIGNAL(toggled(bool)), this, SLOT(resetProfileDraw(bool)));
	}

	demandButtonVector.resize(dPButtonList.count());
	for (int j = 0; j < dPButtonList.count(); j++)
		formLayout1->addRow(dPButtonList[j]);
}

void ProfileMatchView::setSupplyProfileValue(QList<QPair<QString, SupplyDataList > > &value)
{
	if (value != m_pSupplyProfileList) 
	{
		m_pSupplyProfileList = value;
	}

//for supply scenario
	drawSupplyProfileButton(m_iSsValue, m_pSupplyProfileList);
}
void ProfileMatchView::drawSupplyProfileButton(int iss, QList<QPair<QString, SupplyDataList > >  &sp)
{
	QString temp2 = "";
	sPButtonList.clear();
//	for (int j =0; j < formLayout2->count(); j++)
//		formLayout2->takeAt(j);

    QLayoutItem *item;
    while((item = formLayout2->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

	//
	for (int i= 0; i < sp[iss].second.count(); i++)
	{
		temp2 = sp[iss].second[i].supplyName();

		testButton = new MyButton;
		testButton->setText(temp2);
		testButton->setCheckable(true);
		sPButtonList.append(testButton);
		connect(sPButtonList[i], SIGNAL(clicked()), this, SLOT(setDrawProfileMatch()));
		connect(sPButtonList[i], SIGNAL(toggled(bool)), this, SLOT(resetProfileDraw(bool)));
	}
	
	supplyButtonVector.resize(sPButtonList.count());
	for (int j = 0; j < sPButtonList.count(); j++)
		formLayout2->addRow(sPButtonList[j]);
}


void ProfileMatchView::setDsNumValue(int value)
{
	if (value != m_iDsValue) 
	{
		m_iDsValue = value;
	}
//	drawDemandProfileButton(m_iDsValue, m_pDemandProfileList);
}

void ProfileMatchView::setSsNumValue(int value)
{
	if (value != m_iSsValue) 
	{
		m_iSsValue = value;
	}
//	drawSupplyProfileButton(m_iSsValue, m_pDemandProfileList);
}

int ProfileMatchView::dsNumValue()
{
	return m_iDsValue;
}

int ProfileMatchView::ssNumValue()
{
	return m_iSsValue;
}

QList<QPair<QString, DemandDataList > > ProfileMatchView::demandProfileList()
{
	return m_pDemandProfileList;
}

QList<QPair<QString, SupplyDataList > > ProfileMatchView::supplyProfileList()
{
	return m_pSupplyProfileList;
}
//clear the formlayout
void ProfileMatchView::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0))) 
	{
        if (item->layout()) 
		{
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) 
		{
            delete item->widget();
        }
        delete item;
    }
}