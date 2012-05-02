 /*
     MatchResultModel.cpp

     Provides a simple table model to show how to create and use hierarchical
     models.
 */

 #include <QtGui>
 #include "matchresultmodel.h"

 MatchResultModel::MatchResultModel(const QStringList &data, QObject *parent)
     : QAbstractItemModel(parent)
 {
     QList<QVariant> rootData;
     rootData << "Scenario(s)";
     rootItem = new DemandTreeItem(rootData);

     setupModelData(data, rootItem);
 }

 MatchResultModel::~MatchResultModel()
 {
     delete rootItem;
 }

 int MatchResultModel::columnCount(const QModelIndex &parent) const
 {
     if (parent.isValid())
         return static_cast<DemandTreeItem*>(parent.internalPointer())->columnCount();
     else
         return rootItem->columnCount();
 }

 QVariant MatchResultModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role != Qt::DisplayRole)
         return QVariant();

     DemandTreeItem *item = static_cast<DemandTreeItem*>(index.internalPointer());

     return item->data(index.column());
 }

 Qt::ItemFlags MatchResultModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;
	 if (hasChildren(index))
		 return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	 else
		 return Qt::ItemIsEnabled | Qt::ItemIsSelectable;		
 }

 QVariant MatchResultModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
         return rootItem->data(section);

     return QVariant();
 }

 QModelIndex MatchResultModel::index(int row, int column, const QModelIndex &parent)
             const
 {
     if (!hasIndex(row, column, parent))
         return QModelIndex();

     DemandTreeItem *parentItem;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<DemandTreeItem*>(parent.internalPointer());

     DemandTreeItem *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }

 QModelIndex MatchResultModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     DemandTreeItem *childItem = static_cast<DemandTreeItem*>(index.internalPointer());
     DemandTreeItem *parentItem = childItem->parent();

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, parentItem);
 }

 int MatchResultModel::rowCount(const QModelIndex &parent) const
 {
     DemandTreeItem *parentItem;
     if (parent.column() > 0)
         return 0;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<DemandTreeItem*>(parent.internalPointer());

     return parentItem->childCount();
 }

 void MatchResultModel::setupModelData(const QStringList &lines, DemandTreeItem *parent)
 {
     QList<DemandTreeItem*> parents;
     QList<int> indentations;
     parents << parent;
     indentations << 0;

     int number = 0;

     while (number < lines.count())
	 {
         int position = 0;
         while (position < lines[number].length())
		 {
             if (lines[number].mid(position, 1) != " ")
                 break;
             position++;
         }

         QString lineData = lines[number].mid(position).trimmed();

         if (!lineData.isEmpty()) 
		 {
             // Read the column data from the rest of the line.
             QStringList columnStrings = lineData.split(",", QString::SkipEmptyParts);
             QList<QVariant> columnData;
             for (int column = 0; column < columnStrings.count(); ++column)
                 columnData << columnStrings[column];

             if (position > indentations.last()) 
			 {
                 // The last child of the current parent is now the new parent
                 // unless the current parent has no children.

                 if (parents.last()->childCount() > 0) 
				 {
                     parents << parents.last()->child(parents.last()->childCount()-1);
                     indentations << position;
                 }
             } 
			 else 
			 {
                 while (position < indentations.last() && parents.count() > 0) 
				 {
                     parents.pop_back();
                     indentations.pop_back();
                 }
             }

             // Append a new item to the current parent's list of children.
             parents.last()->appendChild(new DemandTreeItem(columnData, parents.last()));
         }
         number++;
     }
 }

/*
  bool DemandScenarioTreeModel::setData((const QModelIndex & index, const QVariant & value, int role)
  {
     if (role == Qt::EditRole)
     {
         //save value from editor to member m_gridData
         m_gridData[index.row()][index.column()] = value.toString();
         //for presentation purposes only: build and emit a joined string
         QString result;
         for(int row= 0; row < ROWS; row++)
         {
             for(int col= 0; col < COLS; col++)
             {
                 result += m_gridData[row][col] + " ";
             }
         }
         emit editCompleted( result );
     }
     return true;
  }
*/