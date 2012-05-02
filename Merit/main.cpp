#include "merit.h"
#include "connection.h"
#include <QtGui/QApplication>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	if (!createConnection())
      return 1;
	Merit w;
	w.show();
	return a.exec();
}
