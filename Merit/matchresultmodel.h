 #ifndef MATCHRESULTMODEL_H
 #define MATCHRESULTMODEL_H

 #include <QAbstractItemModel>
 #include <QModelIndex>
 #include <QVariant>

 class DemandTreeItem;

 class MatchResultModel : public QAbstractItemModel
 {
     Q_OBJECT

 public:
     MatchResultModel(const QStringList &data, QObject *parent = 0);
     ~MatchResultModel();

     QVariant data(const QModelIndex &index, int role) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;
     QModelIndex parent(const QModelIndex &index) const;
     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;

 private:
	 void setupModelData(const QStringList &lines, DemandTreeItem *parent);
     DemandTreeItem *rootItem;
 };

 #endif