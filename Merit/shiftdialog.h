#ifndef SHIFTDIALOG_H
#define SHIFTDIALOG_H

#include <QDialog>
#include <QtGui>
#include "ui_shiftdialog.h"

class ShiftDialog : public QDialog
{
public:
	ShiftDialog( QWidget *parent=0 );
  
	const QString numberOfSteps() const;
	void setNumberOfSteps( const QString &steps );  

	void setForwardDirection(bool forward);
	void setBackwardDirection(bool backward);

	bool forwardDirection();
	bool backwardDirection();

private:
    Ui::shiftDialog ui;

    bool m_bForward;
	bool m_bBackward;

};

#endif // SHIFTDIALOG_H