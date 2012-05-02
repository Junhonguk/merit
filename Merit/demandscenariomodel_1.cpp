
#include <QtCore/QMetaObject>
#include "demandscenariomodel.h"
#include "demandtreeitem.h"

DemandScenarioModel::DemandScenarioModel( QObject *parent ) : QAbstractItemModel( parent )
{
//	QObject* root;
	m_root = new QObject();
	m_root->setObjectName( "root" );
	for( int r=0; r<1; r++ )
	{
		QObject *child;
		QObject *ds = new QObject( m_root );
		ds->setObjectName(QString("ds %0").arg(r));

		child = new QObject( ds );
		child->setObjectName( "No demand profile selected yet" );
	}
	m_DemandScnearioName.clear();
	m_DemandScnearioName.append("ds 0");

	m_SubDemandProfiles.clear();

 //   m_root = root;
//	m_dsName = "";
//	m_dsDescription = "";
}
Qt::ItemFlags DemandScenarioModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant DemandScenarioModel::data( const QModelIndex &index, int role) const
{
  if( !index.isValid() )
    return QVariant();
    
  if( role == Qt::DisplayRole )
  {
    switch( index.column() )
    {
    case 0:
      return static_cast<QObject*>( index.internalPointer() )->objectName();
      
    case 1:
//		return dsDescription();
//      return static_cast<QObject*>( index.internalPointer() )->metaObject()->className();
      
    default:
      break;
    }
  }
  else if( role == Qt::ToolTipRole )
  {
    switch( index.column() )
    {
    case 0:
      return QString( "The name of the demand scenario." );
      
    case 1:
      return QString( "The description of the demand scenario." );
      
    default:
      break;
    }
  }
/*
  else if( role == Qt::EditRole )
  {
    switch( index.column() )
    {
    case 0:
      return static_cast<QObject*>( index.internalPointer() )->objectName();
      
    case 1:
		return dsDescription();
//      return static_cast<QObject*>( index.internalPointer() )->metaObject()->className();
      
    default:
      break;
    }
  }
*/
  return QVariant();
}

QVariant DemandScenarioModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
  if( role != Qt::DisplayRole || orientation != Qt::Horizontal )
    return QVariant();
    
  switch( section )
  {
  case 0:
    return QString( "Demand scenario" );
    
  case 1:
    return QString( "Description" );
    
  default:
    return QVariant();
  }
}

int DemandScenarioModel::rowCount(const QModelIndex &parent ) const
{
  QObject *parentObject;
  
  if( !parent.isValid() )
    parentObject = m_root;
  else
    parentObject = static_cast<QObject*>( parent.internalPointer() );
    
  return parentObject->children().count();
}
//specify the column of tree view in the demand scenario section
int DemandScenarioModel::columnCount(const QModelIndex &parent ) const
{
  return 1;
}

QModelIndex DemandScenarioModel::index(int row, int column, const QModelIndex &parent ) const
{
  QObject *parentObject;
  
  if( !parent.isValid() )
    parentObject = m_root;
  else
    parentObject = static_cast<QObject*>( parent.internalPointer() );
  
  if( row < parentObject->children().count() )
    return createIndex( row, column, parentObject->children().at( row ) );
  else
    return QModelIndex();
}

QModelIndex DemandScenarioModel::parent(const QModelIndex &index) const
{
  if( !index.isValid() )
    return QModelIndex();
    
  QObject *indexObject = static_cast<QObject*>( index.internalPointer() );
  QObject *parentObject = indexObject->parent();
  
  if( parentObject == m_root )
    return QModelIndex();
    
  QObject *grandParentObject = parentObject->parent();
  
  return createIndex( grandParentObject->children().indexOf( parentObject ), 0, parentObject );
}

bool DemandScenarioModel::setData( const QModelIndex &index, const QVariant &value, int role )
{

  if( role != Qt::EditRole || index.column() != 0 )//|| index.row() > m_root->count() )
    return false;
    
//  if( value.toString() == m_root->objectName())//index.row() ) )
//    return false;

 // m_root->parent()->setObjectName(value.toString());

	index.model()->setData(index, value.toString());


  emit dataChanged( index, index );
  return true;
}
