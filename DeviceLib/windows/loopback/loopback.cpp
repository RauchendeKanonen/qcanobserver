
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


#include "../../../wingettimeofday.h"

#define LOOPBACK
#include "../../candevice.h"

#include "../../../obscan.h"
#include <fcntl.h>    // O_RDWR
#include <QDir>

#include <errno.h>

extern "C" CANDevice* create_object();

extern "C" void destroy_object( CANDevice* object );

extern "C" int getDeviceFlags(void)
{
    int Flags = 0;
    return Flags;
}

extern "C" void* createConfig(void *oldconf)
{
    return oldconf;
}


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

CANDevice::~CANDevice()
{

}


//!
int CANDevice::CANSetFilter(int FromID, int ToID, int nCANMsgType)
{
    return OPSUCCESS;
}

int CANDevice::CANClearFilters(void)
{
    return OPSUCCESS;
}

int CANDevice::CANDeviceOpen(void *buf)
{
     return OPENSUCCESSFUL;
}

int CANDevice::CANDeviceInit(int BaudRate, int MsgType)
{

    DevHandle = (void*)1;  //show that we are configured
    return OPSUCCESS;
}

int CANDevice::CANDeviceRead(_CANMsg *Msg)
{
    Mutex.lock();
    if(MsgLst.count())
    {
        *Msg = MsgLst.at(0);
        MsgLst.removeAt(0);
        Mutex.unlock();
        return OPSUCCESS;
    }
    Mutex.unlock();
    return OPFAIL;
}

int CANDevice::CANDeviceWrite(_CANMsg Msg)
{
    Mutex.lock();
    if(MsgLst.count() < 100000)
    {
        gettimeofday(&Msg.tv,NULL);
        MsgLst.append(Msg);
        Mutex.unlock();
        return OPSUCCESS;
    }
    Mutex.unlock();
    return OPFAIL;
}
