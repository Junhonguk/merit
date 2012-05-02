#ifndef DEMANDPROPERTYDIALOG_H
#define DEMANDPROPERTYDIALOG_H

#include <QDialog>
#include <QtGui>
#include "ui_demandpropertydialog.h"

class DemandPropertyDialog : public QDialog
{
public:
	DemandPropertyDialog( QWidget *parent=0 );
  
	const int hourSteps() const; //return the current index in the hour steps combobox
	const int startDay() const;//return the current index in the start day combobox
	const int endDay() const; //return the current index in the end day combobox

	void setHourSteps( const QStringList &steps, const int &hoursteps );  
	//set the start day
	void setStartDay( const QStringList &start, const int &startday );
	//set the end day
	void setEndDay( const QStringList &end, const int &endday);

private:
    Ui::demandPropertyDialog ui;

};

#endif // DEMANDPROPERTYDIALOG_H