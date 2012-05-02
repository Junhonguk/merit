#ifndef MERITTAB_H
#define MERITTAB_H

#include <QTabWidget>
#include <QString>
#include <QMap>

//Class Welcome;

class MeritTab : public QTabWidget
{
    Q_OBJECT

public:
    MeritTab(QWidget *parent = 0);

//    void readFlightCurves(WeatherPlot *plotter, const QString &fileName);
//    Welcome *welcomeWidget;
//    Ui::Welcome uiwel;
    

public slots:
//    void setCurrentTab();

signals:
//    void modified();

private slots:
//    void somethingChanged();

private:
	void createActions();
};

#endif
