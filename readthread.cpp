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
#include "readthread.h"
#include <sys/time.h>
#include <QString>

#include "errordialog.h"

#ifdef WINDOWS
#include <windows.h>
#endif
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

#ifdef LINUX
    //Load the interface to the hardware
    void* handle = dlopen(InterfaceLib.toStdString().c_str(), RTLD_LAZY);

    if(!handle)
    {
        QString *Err = new QString();
        Err->sprintf("%s %s","Could not load Device Mapper: ", InterfaceLib);
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage(*Err);
        delete Err;
        ed->setModal(true);
        ed->show();
        Dev = NULL;
        //delete ed;
        return;
    }


    CANDevice* (*create)();
    void (*destroy)(CANDevice*);


    create = (CANDevice* (*)())dlsym(handle, "create_object");
    destroy = (void (*)(CANDevice*))dlsym(handle, "destroy_object");



    Path = PathArg;
#endif

#ifdef WINDOWS
    wchar_t Filename[256];
    memset(Filename, 0, 256*sizeof(wchar_t));
    InterfaceLib.toWCharArray(Filename);



    HINSTANCE__ *PCANDll;

    PCANDll = LoadLibrary(Filename);
    int ErrCode = GetLastError();
    if(!PCANDll)
    {
        QString *Err = new QString();
        Err->sprintf("%s %s,GetLastError() = (%d)","Could not load Device Mapper: ", InterfaceLib.toStdString().c_str(),ErrCode);
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage(*Err);
        delete Err;
        ed->setModal(true);
        ed->show();
        Dev = NULL;
        //delete ed;
        return;
    }

    typedef CANDevice* (CALLBACK* _create)(void);
    _create create;

    typedef void (CALLBACK* _destroy)(CANDevice*);
    _destroy destroy;

    create = (_create)GetProcAddress(PCANDll,"create_object");
    destroy = (_destroy)GetProcAddress(PCANDll,"destroy_object");
#endif

    if(!create || !destroy)
    {
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage("Could not find the expected symbols in the device mapper lib!");
        ed->setModal(true);
        ed->show();
        Dev = NULL;
        //delete ed;
        return;
    }

    Dev = (CANDevice*)create();

    int ret = Dev->CANDeviceOpen(PathArg);

    if(ELIBNOTFOUND == ret)
    {
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage("Could not find the vendor supplied library");
        ed->setModal(true);
        ed->show();
        destroy(Dev);
        Dev = NULL;
        //delete ed;
        return;
    }


    if(Dev->CANDeviceInit(BaudRate, MsgType))
    {
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage("Device could not be initialized");
        ed->setModal(true);
        ed->show();
        destroy(Dev);
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
    _CANMsg Msg;
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

void ReadThread::sendCANMsg(_CANMsg *Msg)
{
    Dev->CANDeviceWrite(*Msg);
}
