#ifndef	SUPPLY_H
#define SUPPLY_H
#include <QtGui>

#include <QWidget>
#include "ui_supply.h"
#include "merittab.h"

class Supply : public QWidget, public Ui::Supply
{
    Q_OBJECT
public:
	Supply(QWidget *parent = 0);
    MeritTab *meritTab;

public slots:
  //  void setCurrentTab();

};

#endif
