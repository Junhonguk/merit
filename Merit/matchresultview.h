
#ifndef MATCHRESULTVIEW_H
#define MATCHRESULTVIEW_H

#include <QtGui>
#include <QTableView>


class MatchResultView : public QTableView
{
  Q_OBJECT
  
public:
	MatchResultView( QWidget *parent = 0 );
	~MatchResultView();


	bool eventFilter(QObject* o, QEvent* e);

protected:
	bool m_toggleCornerSelection;
//	void paintEvent( QPaintEvent* event);
signals:
	void emitSetModelSignal();
    void emitSetProxyModelSignal();
protected slots:
	void handleCornerButtonClicked();
	void sortMatchResults();
	void disableSortMatchResults();
};

#endif // MATCHRESULTVIEW_H
