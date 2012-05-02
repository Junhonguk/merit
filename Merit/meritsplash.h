#ifndef MERITSPLASH_H
#define MERITSPLASH_H
#include <QSplashScreen>
//#include <qdialog.h>
#include <QPixmap.h>
//#include <qstring.h>
//#include <qprogressbar.h>
//#include <qlabel.h>

class MeritSplash : public QSplashScreen
{
  Q_OBJECT
public:
  MeritSplash(QWidget* parent, const QPixmap &p, Qt::WindowFlags f = 0 );
//  MeritSplash( const QPixmap &p, const char *title, const char *cpyr, bool pbar=false );

 // bool     isProgressSplash()   { return( isps );  }
//  int      progressValue()      { return( progr ); }

public slots:
//  void     setProgressValue( int );
//  void     setProgressText( const QString & );

private:
 // bool          isps;
 // int           progr;
 // QProgressBar *pbar;
 // QLabel       *plabel;
};
#endif // _SPLASH_H