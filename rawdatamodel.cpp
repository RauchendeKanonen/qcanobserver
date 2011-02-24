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
#include "config.h"
#include "rawdatamodel.h"
#include <QColor>


int RawDataModel::rowCount(const QModelIndex &parent) const
{
    if(parent.row()==-1 || parent.column()==-1)
    {
	int cnt = DataList.count();
        return cnt;
    }
    return DataList.count();;
}

int RawDataModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant RawDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= DataList.count())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
	_CANMsg CANMsg = DataList.at(index.row());
	QString Field;

	if(index.column() == 2)
	{
	    Field.sprintf("%f", (float)CANMsg.tv.tv_sec + (float)CANMsg.tv.tv_usec/1000000.0);
	    return QVariant(Field);
	}
	if(index.column() == 1)
	{
	    Field.sprintf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", CANMsg.DATA[0], CANMsg.DATA[1], CANMsg.DATA[2], CANMsg.DATA[3], CANMsg.DATA[4]
								    , CANMsg.DATA[5], CANMsg.DATA[6], CANMsg.DATA[7]);
	    return QVariant(Field);
	}
	if(index.column() == 0)
	{
	    Field.sprintf("0x%04x", (unsigned int)CANMsg.ID);
	    return QVariant(Field);
	}
    }
    else
        return QVariant();
}

QVariant RawDataModel::headerData(int section, Qt::Orientation orientation,
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

Qt::ItemFlags RawDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;


    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}


bool RawDataModel::setData(const QModelIndex &index,
			      _CANMsg *Msg, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
	DataList.replace(index.row(), *Msg);
	return true;
    }

    return false;
}

bool RawDataModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), position, position+rows-1);
    _CANMsg Msg;
    for (int row = 0; row < rows; ++row)
    {
	DataList.insert(position, Msg);
    }

    endInsertRows();
    return true;
}

bool RawDataModel::Update()
{
    return true;
}

bool RawDataModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row)
    {
        DataList.removeAt(position);
    }
    endRemoveRows();
    return true;
}
