#include "matchresultview.h"


MatchResultView::MatchResultView( QWidget *parent ) : QTableView( parent )
{
    // Take the click signal off of the corner widget and replace it with our own
    // This enables toggling the selection when the corner button is clicked.
    // This is more ergonomic than the default behavior of only selecting all of the cells.

    m_toggleCornerSelection = false;
    QAbstractButton *btn = findChild<QAbstractButton *>();
    if (btn != NULL) 
	{
		btn->disconnect(SIGNAL(clicked()));
		connect(btn, SIGNAL(clicked()), this, SLOT(handleCornerButtonClicked()));
/*
		MyIcons *iconList = MyIcons::getIcons(); // our class that handles all of our icons
		QIcon selectIcon = iconList->getIconSet(MyIcons::SELECT_ALL);
		btn->setIcon(selectIcon);
*/
		btn->setText("S");
		btn->setToolTip("Toggle ranking results on match rate.");
		btn->installEventFilter(this);
	     
		// adjust the width of the vertical header to match the preferred corner button width
		// (unfortunately QAbstractButton doesn't implement any size hinting functionality)
		QStyleOptionHeader opt;
		opt.text = btn->text();
//		opt.icon = btn->icon();
		QSize s = (btn->style()->sizeFromContents(QStyle::CT_HeaderSection, &opt, QSize(), btn).
		expandedTo(QApplication::globalStrut()));
		if (s.isValid()) 
		{
			verticalHeader()->setMinimumWidth(s.width());
		}
    }

}

 MatchResultView::~MatchResultView()
{

}
//-----------------------------------------------------------------------------
// Event filter used to draw an icon in the top-left corner of the table widget
//-----------------------------------------------------------------------------
 
bool MatchResultView::eventFilter(QObject* o, QEvent* e)
{

	if (e->type() == QEvent::Paint) 
	{
		QAbstractButton* btn = qobject_cast<QAbstractButton*>(o);
		if (btn) 
		{
			// paint by hand (borrowed from QTableCornerButton)
			QStyleOptionHeader opt;
			opt.init(btn);
			QStyle::State styleState = QStyle::State_None;

			btn->setCheckable(true);
			if (btn->isEnabled())
				styleState |= QStyle::State_Enabled;
			if (btn->isActiveWindow())
				styleState |= QStyle::State_Active;
			if (btn->isDown())
				styleState |= QStyle::State_Sunken;

			if (btn->isChecked())
				styleState |= QStyle::State_On;

			opt.state = styleState;
			opt.rect = btn->rect();
			opt.text = btn->text(); // this line is the only difference to QTableCornerButton
//			opt.icon = btn->icon(); // this line is the only difference to QTableCornerButton
			opt.position = QStyleOptionHeader::OnlyOneSection;
			QStylePainter painter(btn);
			painter.drawControl(QStyle::CE_Header, opt);
			return true; // eat event
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Handle clicking the upper-left corner button. This action toggles the selection of all of the cells
// off and on.
//-----------------------------------------------------------------------------
 
void MatchResultView::handleCornerButtonClicked()
{
	if (m_toggleCornerSelection) 
	{
//no sorting
		disableSortMatchResults();

	} 
	else 
	{
//		selectAll();
//sort the table
		sortMatchResults();
	}
	m_toggleCornerSelection = !m_toggleCornerSelection;

}


/*
void MatchResultView::paintEvent( QPaintEvent* event)
{

}
*/

void MatchResultView::sortMatchResults()
{
//	setSortingEnabled(true);
	sortByColumn(1, Qt::DescendingOrder);
	emit emitSetProxyModelSignal();	
}

void MatchResultView::disableSortMatchResults()
{

//	sortByColumn(0, Qt::DescendingOrder);
	setSortingEnabled(false);
	sortByColumn(-1);
	emit emitSetModelSignal();
}