#include <QtGui>
#include <QStandardItemModel>
#include "demandprocess.h"


class QStandardItemModel;
class QTableView;

using namespace std;

DemandProcess::DemandProcess(QWidget *parent)
    : QWidget(parent)
{
	m_ItemName = " ";

    setupUi(this);
	processDemandList.clear();

	connect(listProcessDemandWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(setListItemName(QListWidgetItem *)));
}



DemandProcess::~DemandProcess()
{

}


void DemandProcess::updateView()
{

}

void DemandProcess::AllocateMemory(int n)
{
	//allocate 8760 to the vectors for storing demand data
//	demandData.clear();
 //   demandData.reserve(n);
  //  demandData.resize(n);
}

void DemandProcess::setListItemName(QListWidgetItem *Item)
{
	m_ItemName = Item->text();
}