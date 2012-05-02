#include "sortonsecondmodel.h"

SortOnSecondModel::SortOnSecondModel( QObject *parent ) : QSortFilterProxyModel( parent )
{
}

bool SortOnSecondModel::lessThan( const QModelIndex &left, const QModelIndex &right ) const
{
  QString leftString = sourceModel()->data( left ).toString();
  QString rightString = sourceModel()->data( right ).toString();
  
  if( leftString.length() > 1 )
    leftString = leftString.mid( 1 );
  else
    leftString = "";

  if( rightString.length() > 1 )
    rightString = rightString.mid( 1 );
  else
    rightString = "";
    
  return leftString < rightString;
}
