
#ifndef DEMANDSCENARIOMODEL_H
#define DEMANDSCENARIOMODEL_H
#include <QAbstractItemModel>

class DemandScenarioModel : public QAbstractItemModel
{
  Q_OBJECT
  
public:
  DemandScenarioModel( QObject *parent = 0 );

  Qt::ItemFlags flags( const QModelIndex &index ) const;
  QVariant data( const QModelIndex &index, int role ) const;
  QVariant headerData( int section, Qt::Orientation orientation, 
	                   int role = Qt::DisplayRole ) const;
  int rowCount( const QModelIndex &parent = QModelIndex() ) const;
  int columnCount( const QModelIndex &parent = QModelIndex() ) const;
  
  QModelIndex index( int row, int column, 
	                 const QModelIndex &parent = QModelIndex() ) const;
  QModelIndex parent( const QModelIndex &index ) const;

  bool setData( const QModelIndex &index, const QVariant &value, int role );

private:
  QObject* m_root;
//  QString m_dsDescription;
//  QString m_dsName;

//  QList<QString> m_values;
    QList< QPair < QString, QList<float> > > m_SubDemandProfiles;
	QList <QString>  m_DemandScnearioName;

public:
//  QString dsDescription() const {return m_dsDescription;}
 // void setdsDescription(QString dsr) {m_dsDescription = dsr;}

 // QString dsName() const {return m_dsName;}
 // void setdsName(QString name) {m_dsName = name;}

};

#endif // DEMANDSCENARIOMODEL_H
