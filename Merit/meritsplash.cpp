#include "meritsplash.h"
#include <QPixmap>

MeritSplash::MeritSplash(QWidget* parent, const QPixmap &p, Qt::WindowFlags f = 0 )
{

	QPixmap pixmap(":/Resources/splash.png");
    show();
}

