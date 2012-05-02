#ifndef	WELCOME_H
#define WELCOME_H
#include <QtGui>

#include <QWidget>
#include "ui_welcome.h"
#include "merittab.h"

class Welcome : public QWidget, public Ui::Welcome
{
    Q_OBJECT
public:
	Welcome(QWidget *parent = 0);
    MeritTab *meritTab;
//	QTextBrowser*   textBrowser;

//	void setText(QString);

public slots:
  //  void setCurrentTab();

};

#endif
