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
#include "readthread.h"
#include <sys/time.h>

#include "errordialog.h"
ReadThread::ReadThread()
{
        MsgBuf = new MessageBufferInterface(15000);
        QObject::connect(this, SIGNAL(ClearAll()),
                     MsgBuf,SLOT(ClearAll()));
        Dev = NULL;
}


//!SLOT for setting the device
//!PathArg is the path to the Device eg. /dev/pcanusb
//!For Baudrate see the Datasheet of the PCAN devices
//!MsgType has to be
void ReadThread::setDev(QString PathArg, int BaudRate, int MsgType, QString InterfaceLib)
{

    //Load the interface to the hardware
    void* handle = dlopen(InterfaceLib.toStdString().c_str(), RTLD_LAZY);
    CANDevice* (*create)();
    void (*destroy)(CANDevice*);

    char *i = dlerror();
    create = (CANDevice* (*)())dlsym(handle, "create_object");
    destroy = (void (*)(CANDevice*))dlsym(handle, "destroy_object");

    Dev = (CANDevice*)create();

    Path = PathArg;

    if(!Dev->CANDeviceOpen(PathArg) || Dev->CANDeviceInit(BaudRate, MsgType))
    {
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage("Device could not be opened");
        ed->setModal(true);
        ed->show();
        delete Dev;
        Dev = NULL;
        //delete ed;
        return;
    }
    
}
//!SLOT for setting the Hardware Filter Place == HWFILTER makes this function acting
//!sets the Messages from int from to int to free
//!from=-1 and to=-1 clears all filters
 void ReadThread::setFilter(int Place, int from, int to)
 {
    if(Place == HWFILTER)
    {
        if(Dev)
            Dev->CANSetFilter(from, to, MSGTYPE_STANDARD);
    }

    //clear all
    if(Place == HWFILTER && from == -1 && to == -1)
    {
        if(Dev)
           Dev->CANClearFilters();
    }
 }
bool ReadThread::isConfigured()
{
   if(Dev)
       return true;

   return false;
}
void ReadThread::run()
{
    TPCANMsg Msg;
    struct timeval tv, starttime,dt;
    QuitNow = 0;
    gettimeofday( &starttime, NULL);

    if(Dev == NULL)
    {
        return;
    }

    while(1)
    {
        if(QuitNow)
        {
            return;
        }
        if(Dev->CANDeviceRead(&Msg))
        {
            gettimeofday( &tv, NULL);
            dt.tv_sec = tv.tv_sec - starttime.tv_sec;
            dt.tv_usec = tv.tv_usec;
            MsgBuf->AddMessage(&Msg, &dt);

        }

    }
}

void ReadThread::QuitThread()
{
    QuitNow = 1;
}

void ReadThread::sendCANMsg(TPCANMsg *Msg)
{
    Dev->CANDeviceWrite(*Msg);
}
