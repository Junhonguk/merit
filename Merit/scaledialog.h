#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H

#include <QDialog>
#include "ui_scaledialog.h"

class ScaleDialog : public QDialog
{
public:
  ScaleDialog( QWidget *parent=0 );
  
  const QString scaleFactor() const;
  void setScaleFactor( const QString& );
  
private:
  Ui::scaleDialog ui;
};

#endif // SCALEDIALOG_H