
#ifndef SORTONSECONDMODEL_H
#define SORTONSECONDMODEL_H

#include <QSortFilterProxyModel>

class SortOnSecondModel : public QSortFilterProxyModel
{
public:
  SortOnSecondModel( QObject *parent = 0 );
  
protected:
  bool lessThan( const QModelIndex &left, const QModelIndex &right ) const;
}; 

#endif // SORTONSECONDMODEL_H