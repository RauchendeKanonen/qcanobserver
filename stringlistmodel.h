#ifndef STRINGLISTMODEL_H
#define STRINGLISTMODEL_H

#include <QAbstractListModel>
#include <qstringlist.h>

//!Model for viewing Msgs in the MainView
//!Implemented because the StandardModel consumed high cputime
//!When ading Messages to it
class StringListModel : public QAbstractListModel
{
     Q_OBJECT
 private:
    QStringList stringList[3];
    QModelIndex akt_index;
    QModelIndex updated_index;
    int akt_position;
    int updated_position;

 public:
     StringListModel()
         : QAbstractListModel(0)
     {
        stringList[0] = QStringList();
        stringList[1] = QStringList();
        stringList[2] = QStringList();
        beginInsertColumns(QModelIndex(), 0, 3);
        endInsertColumns();
        akt_position = 0;
        updated_position = 0;
        updated_index = this->index(0, 0, QModelIndex());
     }
     
     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent) const;
     QVariant data(const QModelIndex &index, int role) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     bool setData(const QModelIndex &index,
                               const QVariant &value, int role);
     bool Update(void);
     bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
     bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

};

#endif // STRINGLISTMODEL_H
