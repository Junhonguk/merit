#ifndef SUPPLYPROPERTYDIALOG_H
#define SUPPLYPROPERTYDIALOG_H

#include <QDialog>
#include <QtGui>
#include "ui_supplypropertydialog.h"

class SupplyPropertyDialog : public QDialog
{
public:
	SupplyPropertyDialog( QWidget *parent=0 );
  
	const int hourSteps() const; //return the current index in the hour steps combobox
	const int startDay() const;//return the current index in the start day combobox
	const int endDay() const; //return the current index in the end day combobox

	void setHourSteps( const QStringList &steps, const int &hoursteps );  
	//set the start day
	void setStartDay( const QStringList &start, const int &startday );
	//set the end day
	void setEndDay( const QStringList &end, const int &endday);

private:
    Ui::supplyPropertyDialog ui;

};

#endif // SUPPLYPROPERTYDIALOG_H