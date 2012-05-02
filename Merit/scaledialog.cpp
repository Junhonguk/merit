#include "scaledialog.h"

#include <QLineEdit>

ScaleDialog::ScaleDialog( QWidget *parent ) : QDialog( parent )
{
  ui.setupUi( this );
}
//get the scaling factor
const QString ScaleDialog::scaleFactor() const
{
  return ui.scaleEdit->text();
}

//set the scaling factor
void ScaleDialog::setScaleFactor( const QString &scale )
{
  ui.scaleEdit->setText( scale );
}

