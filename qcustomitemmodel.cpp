#include "qcustomitemmodel.h"

QCustomItemModel::QCustomItemModel()
{

}


 bool QCustomItemModel::setData(const QModelIndex &index,
                               const QVariant &value, int role)
 {
     if (index.isValid() && role == Qt::EditRole) {

         stringList.replace(index.row(), value.toString());
         emit dataChanged(index, index);
         return true;
     }
     return false;
 }
