#include "stringlistmodel.h"


 int StringListModel::rowCount(const QModelIndex &parent) const
 {
     return stringList[0].count();
 }

 int StringListModel::columnCount(const QModelIndex &parent) const
 {
     return 3;
 }

 QVariant StringListModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (index.row() >= stringList[0].size())
         return QVariant();

     if (role == Qt::DisplayRole)
         return stringList[index.column()].at(index.row());
     else
         return QVariant();
 }

 QVariant StringListModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
 {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal)
         return QString("Column %1").arg(section);
     else
         return QString("Row %1").arg(section);
 }

 Qt::ItemFlags StringListModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return Qt::ItemIsEnabled;

     return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
 }

  bool StringListModel::setData(const QModelIndex &index,
                               const QVariant &value, int role)
 {
     if (index.isValid() && role == Qt::EditRole) {

         stringList[index.column()].replace(index.row(), value.toString());
         akt_index = index;
         return true;
     }
     return false;
 }
 bool StringListModel::insertRows(int position, int rows, const QModelIndex &parent)
 {
     //beginInsertRows(QModelIndex(), position, position+rows-1);
     for (int row = 0; row < rows; ++row) {
         stringList[0].insert(position, "");
         stringList[1].insert(position, "");
         stringList[2].insert(position, "");
     }
    akt_position += rows;
     //endInsertRows();
     return true;
 }
 
 bool StringListModel::Update()
 {
    beginInsertRows(QModelIndex(), updated_position, akt_position);
    endInsertRows();
    updated_position = akt_position;
    emit dataChanged(updated_index, akt_index);
    updated_index = akt_index;
    return true;
 }

 bool StringListModel::removeRows(int position, int rows, const QModelIndex &parent)
 {
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row = 0; row < rows; ++row) {
        stringList[0].removeAt(position);
        stringList[1].removeAt(position);
        stringList[2].removeAt(position);
     }

     endRemoveRows();
     return true;
 }
