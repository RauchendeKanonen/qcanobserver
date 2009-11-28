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
#include "candevice.h"
#include <fcntl.h>    // O_RDWR

#include <libpcan.h>
#include <errno.h>

CANDevice::CANDevice()
{
}
//!
int CANDevice::CANSetFilter(int FromID, int ToID, int nCANMsgType)
{
    if(DevHandle)
        CAN_MsgFilter(DevHandle, FromID, ToID, nCANMsgType);
    return 0;
}

int CANDevice::CANClearFilters(void)
{
    if(DevHandle)
        CAN_ResetFilter(DevHandle);
    return 0;
}

int CANDevice::CANDeviceOpen(QString Path)
{
    if(DevHandle = LINUX_CAN_Open(Path.toStdString().c_str(), O_RDWR))
        return 1;

    return 0;
}

int CANDevice::CANDeviceInit(int BaudRate, int MsgType)
{
    if(CAN_Init(DevHandle, BaudRate, MsgType))
        return 1;

    return 0;
}

int CANDevice::CANDeviceRead(TPCANMsg *Msg)
{
    if(CAN_Read(DevHandle, Msg))
        return 0;

    return 1;
}

int CANDevice::CANDeviceWrite(TPCANMsg Msg)
{
    if(CAN_Write(DevHandle, &Msg))
        return 0;

    return 1;
}

