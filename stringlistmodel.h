/*
    QCANObserver - A CAN Sniffer
    Copyright (C) <2009>  <Netseal>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
