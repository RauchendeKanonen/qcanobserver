
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

#include <windows.h>

#include "Pcan_usb.h"

#include "../../candevice.h"
#include "../../../obcan.h"
#include <fcntl.h>    // O_RDWR
#include <QDir>

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

typedef DWORD (CALLBACK* __CAN_MsgFilter)(int, int, int);
__CAN_MsgFilter _CAN_MsgFilter;

typedef DWORD (CALLBACK* __CAN_ResetFilter)(void);
__CAN_ResetFilter _CAN_ResetFilter;

typedef DWORD (CALLBACK* __CAN_Init)(int, int);
__CAN_Init _CAN_Init;

typedef DWORD (CALLBACK* __CAN_Read)(TPCANMsg*);
__CAN_Read _CAN_Read;

typedef DWORD (CALLBACK* __CAN_Write)(TPCANMsg*);
__CAN_Write _CAN_Write;

CANDevice::CANDevice()
{
    DevHandle = NULL;
}

//!
int CANDevice::CANSetFilter(int FromID, int ToID, int nCANMsgType)
{
    if(DevHandle)
        _CAN_MsgFilter(FromID, ToID, nCANMsgType);
    return 0;
}

int CANDevice::CANClearFilters(void)
{
    if(DevHandle)
        _CAN_ResetFilter();
    return 0;
}

int CANDevice::CANDeviceOpen(QString Path)
{
    //for a local searchpath

    QDir vendordir = QDir();
    vendordir.cd(QString("VendorLibs"));


    QString DllFilename("Pcan_usb.dll");
    QString PathtoLib = vendordir.absoluteFilePath(DllFilename);

    wchar_t Filename[256];
    memset(Filename, 0, 256*2);


    DllFilename.toWCharArray(Filename);

    HINSTANCE__ *PCANDll;

    PCANDll = LoadLibrary(Filename);

    //retry in local path (VendorLibs)
    if(!PCANDll)
    {
        PCANDll = LoadLibrary(PathtoLib.toStdWString().c_str());
    }


    //could not load corresponding driver dll
    if(!PCANDll)
    {

        MessageBox(0, QString("Could not find Pcan_usb.dll in the directory VendorLib nor in the global search path.").toStdWString().c_str(), QString("pcaninterface.dll").toStdWString().c_str(), MB_OK | MB_ICONINFORMATION);

        return ELIBNOTFOUND;
    }
    _CAN_MsgFilter = (__CAN_MsgFilter)GetProcAddress(PCANDll,"CAN_MsgFilter");
    _CAN_ResetFilter = (__CAN_ResetFilter)GetProcAddress(PCANDll,"CAN_ResetFilter");
    _CAN_Init = (__CAN_Init)GetProcAddress(PCANDll,"CAN_Init");
    _CAN_Read = (__CAN_Read)GetProcAddress(PCANDll,"CAN_Read");
    _CAN_Write = (__CAN_Write)GetProcAddress(PCANDll,"CAN_Write");
    _CAN_Write = (__CAN_Write)GetProcAddress(PCANDll,"CAN_Write");

     return OPENSUCCESSFUL;
}

int CANDevice::CANDeviceInit(int BaudRate, int MsgType)
{
    if(_CAN_Init(BaudRate, MsgType))
    {
        DevHandle = (void*)1;  //show that we are configured
        return 1;
    }

    return 0;
}

int CANDevice::CANDeviceRead(_CANMsg *Msg)
{
    TPCANMsg HWMsg;

    if(_CAN_Read(&HWMsg))
    {
        return 0;
    }
    Msg->TimeStamp = -1;
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

    if(_CAN_Write(&HWMsg))
        return 0;

    return 1;
}
