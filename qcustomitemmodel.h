#ifndef QCUSTOMITEMMODEL_H
#define QCUSTOMITEMMODEL_H

#include <QStandardItemModel>

class StringListModel : public QAbstractListModel
{


 public:
     StringListModel(const QStringList &strings, QObject *parent = 0)
         : QAbstractListModel(parent), stringList(strings) {}

     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     QVariant data(const QModelIndex &index, int role) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;

 private:
     QStringList stringList;

};

#endif // QCUSTOMITEMMODEL_H
