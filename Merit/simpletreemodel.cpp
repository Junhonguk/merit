 /*
     DemandScenarioTreeModel.cpp

     Provides a simple tree model to show how to create and use hierarchical
     models.
 */

 #include <QtGui>

 #include "demandtreeitem.h"
 #include "simpletreemodel.h"

 DemandScenarioTreeModel::DemandScenarioTreeModel(const QString &data, QObject *parent)
     : QAbstractItemModel(parent)
 {
     QList<QVariant> rootData;
     rootData << "Title" << "Summary";
     rootItem = new DemandTreeItem(rootData);
     setupModelData(data.split(QString("\n")), rootItem);
 }

 DemandScenarioTreeModel::~DemandScenarioTreeModel()
 {
     delete rootItem;
 }

 int DemandScenarioTreeModel::columnCount(const QModelIndex &parent) const
 {
     if (parent.isValid())
         return static_cast<DemandTreeItem*>(parent.internalPointer())->columnCount();
     else
         return rootItem->columnCount();
 }

 QVariant DemandScenarioTreeModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role != Qt::DisplayRole)
         return QVariant();

     DemandTreeItem *item = static_cast<DemandTreeItem*>(index.internalPointer());

     return item->data(index.column());
 }

 Qt::ItemFlags DemandScenarioTreeModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
 }

 QVariant DemandScenarioTreeModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
         return rootItem->data(section);

     return QVariant();
 }

 QModelIndex DemandScenarioTreeModel::index(int row, int column, const QModelIndex &parent)
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

 QModelIndex DemandScenarioTreeModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     DemandTreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
     DemandTreeItem *parentItem = childItem->parent();

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, parentItem);
 }

 int DemandScenarioTreeModel::rowCount(const QModelIndex &parent) const
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

 void DemandScenarioTreeModel::setupModelData(const QStringList &lines, DemandTreeItem *parent)
 {
     QList<DemandTreeItem*> parents;
     QList<int> indentations;
     parents << parent;
     indentations << 0;

     int number = 0;

     while (number < lines.count()) {
         int position = 0;
         while (position < lines[number].length()) {
             if (lines[number].mid(position, 1) != " ")
                 break;
             position++;
         }

         QString lineData = lines[number].mid(position).trimmed();

         if (!lineData.isEmpty()) {
             // Read the column data from the rest of the line.
             QStringList columnStrings = lineData.split(",", QString::SkipEmptyParts);
             QList<QVariant> columnData;
             for (int column = 0; column < columnStrings.count(); ++column)
                 columnData << columnStrings[column];

             if (position > indentations.last()) {
                 // The last child of the current parent is now the new parent
                 // unless the current parent has no children.

                 if (parents.last()->childCount() > 0) {
                     parents << parents.last()->child(parents.last()->childCount()-1);
                     indentations << position;
                 }
             } else {
                 while (position < indentations.last() && parents.count() > 0) {
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