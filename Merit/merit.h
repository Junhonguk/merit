#ifndef MERIT_H
#define MERIT_H

#include <QtGui/QMainWindow>
#include "ui_merit.h"
//#include "welcome.h"

class MeritTab;
class Merit : public QMainWindow
{
	Q_OBJECT

public:
	Merit(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Merit();

private:
	Ui::MeritClass ui;
//	Welcome *welcome;

	void createActions();

    MeritTab *meritTab;
private slots:
	void aboutMerit();
    
};

#endif // MERIT_H
