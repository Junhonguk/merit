/*
 * Copyright (c) 2006-2007, Johan Thelin
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, 
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,  
 *       this list of conditions and the following disclaimer in the documentation 
 *       and/or other materials provided with the distribution.
 *     * Neither the name of APress nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef DRAWMATCHVIEW_H
#define DRAWMATCHVIEW_H

#include <QtGui>
#include <QWidget>
#include "mybutton.h"
#include <QAbstractButton>
#include <QPushButton>
#include <QMap>

struct DrawMatchViewData
{
	bool m_bDraw;
};


//class DrawMatchViewData;

class DrawMatchView : public QWidget
{
  Q_OBJECT

public:

	DrawMatchView(QWidget *parent=0 );
	DrawMatchView(const DrawMatchView &other);

	~DrawMatchView();
	DrawMatchView& operator=(const DrawMatchView& other);

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




	bool m_bMouseOver;
	bool m_LeftDraw;
	bool m_RightDraw;
	bool m_bDraw;
	bool m_bDemandIndexExist;
	bool m_bSupplyIndexExist;

	bool m_indexDemandExist;
	bool m_indexSupplyExist;

	QList <int> indexDemandList;
	QList <int> indexSupplyList;

	bool m_leftSelected;
	bool m_rightSelected;

	MyBtnList myButtonList;
	MyBtnList mySupplyButtonList;

	QList<QPair<int, int> > indexScnSelList;

	int indexDmdSelScn;
	int indexSupSelScn;

//	qMakePair


	void pairScenarioSelection(int indexDemand, int indexSupply);
//  QSize sizeHint() const;
    bool enableProfileDraw();
	void setProfileDraw(bool bDraw);
//	void drawDemandScenarioButton(int ds);
	void drawDemandScenarioButton(QMap<QString, QString>  &ds);
	void drawSupplyScenarioButton(QMap<QString, QString>  &ss);

	void clearLayout(QLayout *layout);

protected:
  void paintEvent( QPaintEvent* event);
//  void mousePressEvent( QMouseEvent* event);

private:
	int mx, my;
	int m_indexCurrentDemand;
	int m_indexCurrentSupply;

//d pointer 
	DrawMatchViewData *d;

signals:
	void emitProfileDrawSignal();
	void dsValueChanged(int);
	void ssValueChanged(int);
	void drawSignal();

protected slots:
	void setDraw();
	void setDemandScenarioDraw();
	void setSupplyScenarioDraw();
//	void resetScenarioDraw(bool checked);

public slots:
//	void setValue(int value);
	void setValue(QMap<QString, QString>& value); //for demand scenario
	void setSSValue(QMap<QString, QString>& value); //for supply scenario
private:
	int m_iDemandScenario; //total number of demand scenario
	QMap<QString, QString> m_DemandScenarioMap;
	QMap<QString, QString> m_SupplyScenarioMap;
	int m_iSupplyScenario; //total number of supply scenario

};

#endif 
