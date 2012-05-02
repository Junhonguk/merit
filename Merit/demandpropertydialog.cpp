#include "demandpropertydialog.h"

#include <QLineEdit>

DemandPropertyDialog::DemandPropertyDialog( QWidget *parent ) : QDialog( parent )
{
//initialsations
	ui.setupUi( this );
}


//get the number of steps
const int DemandPropertyDialog::hourSteps() const
{
	return ui.stepsPerHourComboBox->currentIndex();
}

//get the start day
const int DemandPropertyDialog::startDay() const
{
	return ui.startDayComboBox->currentIndex();
}

//get the end day
const int DemandPropertyDialog::endDay() const
{
	return ui.endDayComboBox->currentIndex();
}

//set the scaling factor
void DemandPropertyDialog::setHourSteps( const QStringList &steps, const int &hoursteps)
{
	ui.stepsPerHourComboBox->clear();
	ui.stepsPerHourComboBox->addItems(steps);
	ui.stepsPerHourComboBox->setEditText(ui.stepsPerHourComboBox->itemText(hoursteps));
}
//currentIndex
//set the start day
void DemandPropertyDialog::setStartDay( const QStringList &start, const int &startday )
{
   ui.startDayComboBox->clear();
   ui.startDayComboBox->addItems( start );
   ui.startDayComboBox->setEditText(ui.startDayComboBox->itemText(startday));
}

//set the end day
void DemandPropertyDialog::setEndDay( const QStringList &end, const int &endday )
{
   ui.endDayComboBox->clear();
   ui.endDayComboBox->addItems( end );
   ui.endDayComboBox->setEditText(ui.endDayComboBox->itemText(endday));
}