#include "shiftdialog.h"

#include <QLineEdit>

ShiftDialog::ShiftDialog( QWidget *parent ) : QDialog( parent )
{
//initialsations
	m_bForward = false;
	m_bBackward = false;

	ui.setupUi( this );
}
//get the number of steps
const QString ShiftDialog::numberOfSteps() const
{
	return ui.stepsEdit->text();
}

//set the scaling factor
void ShiftDialog::setNumberOfSteps( const QString &steps )
{
	ui.stepsEdit->setText( steps );
}

void ShiftDialog::setForwardDirection(bool forward)
{
	m_bForward = forward;
}
void ShiftDialog::setBackwardDirection(bool backward)
{
	m_bBackward = backward;
}

bool ShiftDialog::forwardDirection()
{
	if(ui.forward->isChecked())
	{
		m_bForward = true;
	}
	else
	{
		m_bForward = true;
	}
	return m_bForward;
}
bool ShiftDialog::backwardDirection()
{
	if(ui.backward->isChecked())
	{
		m_bBackward = true;
	}
	else
	{
		m_bBackward =  false;
	}
	return m_bBackward;
}