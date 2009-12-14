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
#include <libpcan.h>
#undef HANDLE
#include "../../candevice.h"

#include <fcntl.h>    // O_RDWR


#include <errno.h>

extern "C" CANDevice* create_object();

extern "C" void destroy_object( CANDevice* object );

extern "C" CANDevice* create_object()
{
  return new CANDevice;
}

extern "C" void destroy_object( CANDevice* object )
{
  delete object;
}




CANDevice::CANDevice()
{
    DevHandle = NULL;
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
        return OPENSUCCESSFUL;

    return OPENFAILED;
}

int CANDevice::CANDeviceInit(int BaudRate, int MsgType)
{
    if(CAN_Init(DevHandle, BaudRate, MsgType))
        return 1;

    return 0;
}

int CANDevice::CANDeviceRead(_CANMsg *Msg)
{
    TPCANMsg HWMsg;

    if(CAN_Read(DevHandle, &HWMsg))
    {
        return 0;
    }

    Msg->ID = HWMsg.ID;
    memcpy(Msg->DATA, HWMsg.DATA, 8);
    Msg->LEN = HWMsg.LEN;
    Msg->MSGTYPE = HWMsg.MSGTYPE;

    return 1;
}


int CANDevice::CANDeviceWrite(_CANMsg Msg)
{
    TPCANMsg HWMsg;
    HWMsg.ID = Msg.ID;
    memcpy(HWMsg.DATA, Msg.DATA, 8);
    HWMsg.LEN = Msg.LEN;
    HWMsg.MSGTYPE = Msg.MSGTYPE;

    if(CAN_Write(DevHandle, &HWMsg))
        return 0;

    return 1;
}

