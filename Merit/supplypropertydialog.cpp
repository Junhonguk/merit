#include "supplypropertydialog.h"

#include <QLineEdit>

SupplyPropertyDialog::SupplyPropertyDialog( QWidget *parent ) : QDialog( parent )
{
//initialsations
	ui.setupUi( this );
}


//get the number of steps
const int SupplyPropertyDialog::hourSteps() const
{
	return ui.stepsPerHourComboBox->currentIndex();
}

//get the start day
const int SupplyPropertyDialog::startDay() const
{
	return ui.startDayComboBox->currentIndex();
}

//get the end day
const int SupplyPropertyDialog::endDay() const
{
	return ui.endDayComboBox->currentIndex();
}

//set the scaling factor
void SupplyPropertyDialog::setHourSteps( const QStringList &steps, const int &hoursteps)
{
	ui.stepsPerHourComboBox->clear();
	ui.stepsPerHourComboBox->addItems(steps);
	ui.stepsPerHourComboBox->setEditText(ui.stepsPerHourComboBox->itemText(hoursteps));
}
//currentIndex
//set the start day
void SupplyPropertyDialog::setStartDay( const QStringList &start, const int &startday )
{
   ui.startDayComboBox->clear();
   ui.startDayComboBox->addItems( start );
   ui.startDayComboBox->setEditText(ui.startDayComboBox->itemText(startday));
}

//set the end day
void SupplyPropertyDialog::setEndDay( const QStringList &end, const int &endday )
{
   ui.endDayComboBox->clear();
   ui.endDayComboBox->addItems( end );
   ui.endDayComboBox->setEditText(ui.endDayComboBox->itemText(endday));
}