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
#include "config.h"
#include "stringlistmodel.h"
#include <QColor>


int StringListModel::rowCount(const QModelIndex &parent) const
{
    if(parent.row()==-1 || parent.column()==-1)
    {
        int cnt = stringList->at(0)->count();
        return cnt;
    }
    return 0;
}

int StringListModel::columnCount(const QModelIndex &parent) const
{
    return stringList->count();
}

QVariant StringListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::ForegroundRole)
        return *ColorList.at(index.row());

    if (index.row() >= stringList->at(index.column())->size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return stringList->at(index.column())->at(index.row());
    else
        return QVariant();
}

QVariant StringListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        return ColumnNames->at(section);
    }

    else
        return QString("");
}

Qt::ItemFlags StringListModel::flags(const QModelIndex &index) const
{
    for(int i = 0 ; i < FlagIdxLst.count() ; i ++ )
    {
        if(FlagIdxLst.at(i).row() == index.row())
        {
            if(FlagLst.at(i) & Qt::ItemIsEditable)
                return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

            else
                return QAbstractItemModel::flags(index) & ~Qt::ItemIsEditable;
        }
    }


    if (!index.isValid())
        return Qt::ItemIsEnabled;


    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void StringListModel::setflags(QModelIndex index, int Flags)
{
    for(int i = 0 ; i < FlagIdxLst.count() ; i ++ )
    {
        if(FlagIdxLst.at(i).row() == index.row())
        {
            FlagLst.replace(i, Flags);
            return ;
        }
    }
    FlagIdxLst.append(index);
    FlagLst.append(Flags);
}


bool StringListModel::setData(const QModelIndex &index,
                              const QVariant &value, int role, QColor *color)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        ColorList.replace(index.row(), color);

        stringList->at(index.column())->replace(index.row(), value.toString());
        return true;
    }
    return false;
}

bool StringListModel::setColor(const QModelIndex &index, QColor *color)
{
    ColorList.replace(index.row(), color);
    return true;
}

bool StringListModel::setData(const QModelIndex &index,
                              const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        ColorList.replace(index.row(), &black);
        stringList->at(index.column())->replace(index.row(), value.toString());
        return true;
    }
    return false;
}

bool StringListModel::insertRows(int position, int rows, const QModelIndex &parent)
{

    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row)
    {
        ColorList.insert(position, &black);
        for( int i = 0; i < stringList->count() ; i++ )
        {
            stringList->at(i)->insert(position, "");
        }
    }
    endInsertRows();

    return true;
}


bool StringListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row)
    {
        if(stringList->at(0)->count() < position)
            return true;
        stringList->at(0)->removeAt(position);
        stringList->at(1)->removeAt(position);
        stringList->at(2)->removeAt(position);
        stringList->at(3)->removeAt(position);
    }
    endRemoveRows();

    return true;
}
