/*


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

#ifndef RAWDATAMODEL_H
#define RAWDATAMODEL_H
#include "obscan.h"
#include <QAbstractListModel>
#include <qstringlist.h>
#include <QColor>
#include <QFlags>
#include <QFlag>
#include "stringlistmodel.h"

//!Model for viewing Msgs in the MainView
//!Implemented because the StandardModel consumed high cputime
//!When ading Messages to it
class RawDataModel : public QAbstractListModel
{
     Q_OBJECT
 private:
    QStringList *ColumnNames;
    QList <_CANMsg> DataList;



 public:
     RawDataModel(QStringList *columnnames)
         : QAbstractListModel(0)
     {
        ColumnNames = new QStringList(*columnnames);
        beginInsertColumns(QModelIndex(), 0, 3);
        endInsertColumns();
     }
     ~RawDataModel()
     {
         delete ColumnNames;
     }

    bool setData(const QModelIndex &index,
				  _CANMsg *Msg, int role);

     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent) const;
     QVariant data(const QModelIndex &index, int role) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     bool Update(void);
     bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
     bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
     void setflags(QModelIndex index, int Flags);
     bool setColor(const QModelIndex &index, QColor *color);
};

#endif // STRINGLISTMODEL_H
