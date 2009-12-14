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
#include <QObject>
#ifndef CANDEVICE_H
#define CANDEVICE_H

#include"../can.h"

#define ELIBNOTFOUND    -1
#define OPENSUCCESSFUL   1
#define OPENFAILED       0



//redefined HANDLE because of Problems with Qt
#define HANDLEI void*


//!Interface for the CAN Devices
//!We need Classe derived from this Standart Type to implement
//!other CAN Hardware Interfaces. Maybe one can do this task also for
//!differences between Linux and Windows
class CANDevice// : public QObject
{
    //Q_OBJECT
public:

    CANDevice();
    virtual int CANDeviceOpen(QString Path);
    virtual int CANDeviceInit(int BaudRate, int MsgType);
    //virtual int CANDeviceClose(void);
    virtual int CANDeviceRead(_CANMsg *Msg);
    virtual int CANDeviceWrite(_CANMsg Msg);
    virtual int CANSetFilter(int FromID, int ToID, int nCANMsgType);
    virtual int CANClearFilters(void);
private:
    HANDLEI  DevHandle;
    bool    isConfigured;
};

#endif // CANDEVICE_H
