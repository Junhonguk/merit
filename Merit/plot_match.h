#ifndef _PLOTMATCH_H_
#define _PLOTMATCH_H_
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <QtCore/QVector>
class QwtPlotCurve;

class PlotMatch : public QwtPlot
{
public:
    PlotMatch( QWidget *parent = NULL);

	QwtPlotCurve *cMatch;

	QwtPlotCurve *c1Match;
	QwtPlotCurve *c2Match;
	QwtPlotCurve *c3Match;
	QwtPlotCurve *c4Match;
	QwtPlotCurve *c5Match;
	QwtPlotCurve *c6Match;
	QwtPlotCurve *c7Match;
	QwtPlotCurve *c8Match;
	QwtPlotCurve *c9Match;
	QwtPlotCurve *c10Match;

	int m_NoOfMatches;
	QVector <QString> m_MatchNames;

	~PlotMatch();
protected:
    virtual void resizeEvent( QResizeEvent * );

public:
//customised functions
	void drawMatchGraph(const QString& name, const QVector<float> &match);
	void drawMultipleMatchesGraph(const QVector <QString> &name, const QVector< QVector <float> > &match);
private:
    void populate();
    void updateGradient();

	void populateSingle();

	void populateOne(const QVector <QString> &name);
	void populateTwo(const QVector <QString> &name);
	void populateThree(const QVector <QString> &name);
	void populateFour(const QVector <QString> &name);
	void populateFive(const QVector <QString> &name);
	void populateSix(const QVector <QString> &name);
	void populateSeven(const QVector <QString> &name);
	void populateEight(const QVector <QString> &name);
	void populateNine(const QVector <QString> &name);
	void populateTen(const QVector <QString> &name);
};

#endif
