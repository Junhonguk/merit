#include <QStyle>
#include <QStyleOptionButton>
#include <QPainter>
//#include <QAbstractButton>
#include "drawmatchview.h"

DrawMatchView::DrawMatchView(QWidget *parent )  //: public QWidget
{
	m_iDemandScenario = 0;


	d = new DrawMatchViewData;

	m_bMouseOver = false;
	m_LeftDraw = false;
	m_RightDraw = false;
	m_bDraw = false;
	m_indexCurrentDemand = 0;
	m_indexCurrentSupply = 0;

	m_indexDemandExist = false;
	m_indexSupplyExist = false;

	m_bDemandIndexExist = false;
	m_bSupplyIndexExist = false;		 
	gridLayout = new QGridLayout(this);
	//for demand scenario
	verticalLayout1 = new QVBoxLayout();

//	myButtonList.clear();

	indexScnSelList.clear();
	indexDmdSelScn = -1;
	indexSupSelScn = -1;

//for demand scenario layout
	formLayout1 = new QFormLayout();
//for supply scenario layout
	formLayout2 = new QFormLayout();

	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);
//	horizontalSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	horizontalLayout = new QHBoxLayout();

 //   horizontalLayout->addItem(horizontalSpacer1);
	horizontalLayout->addLayout(formLayout1);
    horizontalLayout->addItem(horizontalSpacer);
	horizontalLayout->addLayout(formLayout2);
    horizontalLayout->addItem(horizontalSpacer2);

	gridLayout->addLayout(horizontalLayout,0,0,Qt::AlignLeft);
	setLayout(gridLayout);

	connect(this, SIGNAL(drawSignal()), this, SLOT(setDraw()));
//	connect();
}
/*
QSize DrawMatchView::sizeHint() const
{
  return QSize( fontMetrics().width( text() )+10, fontMetrics().height()+10 );
}
*/

DrawMatchView::DrawMatchView(const DrawMatchView& other )
{
	d = new DrawMatchViewData( *other.d );
}

DrawMatchView::~DrawMatchView()
{
	delete d;
}

DrawMatchView& DrawMatchView::operator=( const DrawMatchView& other )
{
	*d = *other.d;
	return *this;
}

void DrawMatchView::paintEvent( QPaintEvent* event)
{
	QPainterPath path;

	QPoint pt2;
	QPoint pt4;
	QPoint pt1,pt3;

//draw the line to show the combination
	QPainter painter(this);

	if (indexDmdSelScn!=-1 && indexSupSelScn!= -1)
	{
		QRect Rect1 = QRect(myButtonList[indexDmdSelScn]->x(), myButtonList[indexDmdSelScn]->y(),
			myButtonList[indexDmdSelScn]->width(),myButtonList[indexDmdSelScn]->height());
		QRect Rect2 = QRect(mySupplyButtonList[indexSupSelScn]->x(), mySupplyButtonList[indexSupSelScn]->y(),
			mySupplyButtonList[indexSupSelScn]->width(),mySupplyButtonList[indexSupSelScn]->height());
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
	}

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



void DrawMatchView::pairScenarioSelection(int indexDemand, int indexSupply)
{
	indexScnSelList.append(qMakePair(indexDemand, indexSupply));
}

void DrawMatchView::setDraw()
{
//////////////////////////////////////////
	if (m_LeftDraw && m_RightDraw)
	{
		pairScenarioSelection(m_indexCurrentDemand, m_indexCurrentSupply);
		m_bDraw = true;
//		setProfileDraw(true);
		emit dsValueChanged(m_indexCurrentDemand); //set the demand scenario
		emit ssValueChanged(m_indexCurrentSupply); //set the supply scenario
		emit emitProfileDrawSignal();

	}
	else
	{
		m_bDraw = false;
//		d->m_bDraw = m_bDraw;
		setProfileDraw(false);
//		emit emitProfileDraw(false);
	}
	update();
}

bool DrawMatchView::enableProfileDraw()
{
	return d->m_bDraw;
}

void DrawMatchView::setProfileDraw(bool bDraw)
{
	d->m_bDraw = bDraw;
}

// for demand scenario
void DrawMatchView::setValue(QMap<QString, QString>& value)
{
	if (value != m_DemandScenarioMap) 
	{
		m_DemandScenarioMap = value;
	}

	drawDemandScenarioButton(m_DemandScenarioMap);
}
//for supply scenario
void DrawMatchView::setSSValue(QMap<QString, QString>& value)
{
	if (value != m_SupplyScenarioMap) 
	{
		m_SupplyScenarioMap = value;
	}
//for supply scenario, it will delete later
	drawSupplyScenarioButton(m_SupplyScenarioMap);
}

void DrawMatchView::drawDemandScenarioButton(QMap<QString, QString>  &ds)
{
	QString temp = "";
	QString temp2 = "";
	myButtonList.clear();
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

	//construct myButtonList with 3 elements
	for (int i= 0; i < ds.count(); i++)
	{
		temp = QString("ds %1").arg(i+1);
		temp2 = QString("%1").arg(ds[temp]);
		testButton = new MyButton;
		testButton->setText(temp2);
		testButton->setCheckable(true);
		testButton->setMouseTracking(true);
		myButtonList.append(testButton);

//		connect(myButtonList[i], SIGNAL(clicked()), this, SLOT(set()));
		connect(myButtonList[i], SIGNAL(clicked()), this, SLOT(setDemandScenarioDraw()));
//		connect(this, SIGNAL(drawSignal()), this, SLOT(setDraw()));
//		connect(myButtonList[i], SIGNAL(toggled(bool)), this, SLOT(resetScenarioDraw(bool))); 
	}
	

//create a form layout to accomodate the demand scenario buttons

	for (int j = 0; j < myButtonList.count(); j++)
    	formLayout1->addRow(myButtonList[j]);

}

void DrawMatchView::drawSupplyScenarioButton(QMap<QString, QString>  &ss)
{
	QString temp = "";
	QString temp2 = "";
	mySupplyButtonList.clear();
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

	//construct myButtonList with 3 elements
	for (int i= 0; i < ss.count(); i++)
	{
///////////////////////
		temp = QString("ss %1").arg(i+1);
		temp2 = QString("%1").arg(ss[temp]);
		testButton = new MyButton;
		testButton->setText(temp2);
		testButton->setCheckable(true);
		testButton->setMouseTracking(true);
		mySupplyButtonList.append(testButton);
		connect(mySupplyButtonList[i], SIGNAL(clicked()), this, SLOT(setSupplyScenarioDraw()));
//		connect(mySupplyButtonList[i], SIGNAL(toggled(bool)), this, SLOT(resetScenarioDraw(bool))); 
	}
	

//create a form layout to accomodate the demand scenario buttons
	for (int j = 0; j < mySupplyButtonList.count(); j++)
    	formLayout2->addRow(mySupplyButtonList[j]);

}

//clear the formlayout
void DrawMatchView::clearLayout(QLayout *layout)
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
/*
void DrawMatchView::resetScenarioDraw(bool checked)
{
	if (!checked)
	{
//set all the demand scenario buttons to unchecked
		for (int i = 0; i < myButtonList.count(); i++)
		{
			if(myButtonList[i]->isChecked())
			{
				myButtonList[i]->setChecked(false);
			}
		}

//set all the supply scenario buttons to unchecked 
		for (int i = 0; i < mySupplyButtonList.count(); i++)
		{
			if(mySupplyButtonList[i]->isChecked()) //if it is checked
			{
				mySupplyButtonList[i]->setChecked(false);
			}
		}
		update();
	}
}

*/
//set demand scenario draw
void DrawMatchView::setDemandScenarioDraw()
{
//demand scenario button
//check the mouse press area is within the defined button range 
	for (int i = 0; i < myButtonList.count(); i++)
	{
		if(myButtonList[i]->isChecked()) //if it is checked
		{
	//check the if the index is within the senario selection list
			if(indexDmdSelScn!= i)
			{
				if (indexDmdSelScn != -1)
					myButtonList[indexDmdSelScn]->setChecked(false);
				m_indexCurrentDemand = i;
				m_LeftDraw = true;
				myButtonList[i]->setChecked(true);
				break;
			}
			else
			{
				myButtonList[i]->setChecked(false);
				m_LeftDraw = false;
			}
		}	
		else //if it is not checked
		{
//make the button uncheckable for rest of the buttons
			if (i == indexDmdSelScn)
			{
				myButtonList[i]->setChecked(false);
				for (int j = 0; j < mySupplyButtonList.count(); j++)
				{
					if(mySupplyButtonList[j]->isChecked()) //if it is checked
					{
						mySupplyButtonList[j]->setChecked(false);
					}
				}
				m_RightDraw = false;
				indexSupSelScn = -1;
				update();
			}

			m_LeftDraw = false;
		}
	}


	//demand draw
	if (m_LeftDraw)
	{
		indexDmdSelScn = m_indexCurrentDemand;
		emit drawSignal();
	}
	else
	{
		indexDmdSelScn = -1;
	}
}

//set supply scenario draw
void DrawMatchView::setSupplyScenarioDraw()
{
//for supply button
//check the mouse press area is within the defined button range 
	for (int i = 0; i < mySupplyButtonList.count(); i++)
	{
		if(mySupplyButtonList[i]->isChecked()) //if it is checked
		{

	//check the if the index is within the senario selection list
			if(indexSupSelScn!= i)
			{
				if (indexSupSelScn != -1)
					mySupplyButtonList[indexSupSelScn]->setChecked(false);
				m_indexCurrentSupply = i;
				m_RightDraw = true;
				mySupplyButtonList[i]->setChecked(true);
				break;
			}
			else
			{
				mySupplyButtonList[i]->setChecked(false);
				m_RightDraw = false;
			}
		}
		else //if it is not checked
		{
			if (i == indexSupSelScn)
			{
				mySupplyButtonList[i]->setChecked(false);
				for (int j = 0; j < myButtonList.count(); j++)
				{
					if(myButtonList[j]->isChecked()) //if it is checked
					{
						myButtonList[j]->setChecked(false);
					}
				}
				m_LeftDraw = false;
				indexDmdSelScn = -1;
				update();
			}
			m_RightDraw = false;
		}
	}

	//supply draw
	if (m_RightDraw)
	{
		indexSupSelScn = m_indexCurrentSupply;
		emit drawSignal();
	}
	else
	{
		indexSupSelScn = -1;
	}
}