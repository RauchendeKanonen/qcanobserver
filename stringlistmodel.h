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
#include <QColor>
#include <QFlags>
#include <QFlag>
class ColoredVariant : public QVariant
{
    public:
    ColoredVariant(QString &Str,QColor color):QVariant(Str)
    {
        Color = color;
    }
    void setColor(QColor color)
    {
        Color = color;
    }

    QColor* getColor(void)
    {
        return new QColor(Color);
    }

    QColor Color;
};

//!Model for viewing Msgs in the MainView
//!Implemented because the StandardModel consumed high cputime
//!When ading Messages to it
class StringListModel : public QAbstractListModel
{
     Q_OBJECT
 private:
    QStringList *ColumnNames;
    QList <QStringList*> *stringList;
    QList <QColor*> ColorList;
    QList <QModelIndex> FlagIdxLst;
    QList <int> FlagLst;

    QColor black;



 public:
     StringListModel(QStringList *columnnames)
         : QAbstractListModel(0)
     {
         black = QColor(Qt::black);
         stringList = new QList<QStringList*>();
         for(int i = 0; i < columnnames->count() ; i++ )
         {
             stringList->append(new QStringList());
         }
        ColumnNames = new QStringList(*columnnames);
        beginInsertColumns(QModelIndex(), 0, 3);
        endInsertColumns();
     }
     ~StringListModel()
     {
         for(int i = 0; i < stringList->count() ; i++ )
         {
             delete stringList->at(i);
         }

         delete stringList;
         delete ColumnNames;
     }
    bool setData(const QModelIndex &index,
                               const QVariant &value, int role);
    bool setData(const QModelIndex &index,
				  const void *value, int role);


     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent) const;
     QVariant data(const QModelIndex &index, int role) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     bool setData(const QModelIndex &index,
                               const QVariant &value, int role, QColor *color);
     bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
     bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
     void setflags(QModelIndex index, int Flags);
     bool setColor(const QModelIndex &index, QColor *color);
};

#endif // STRINGLISTMODEL_H
