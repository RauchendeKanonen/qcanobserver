#include "readthread.h"
#include <sys/time.h>

#include "errordialog.h"
ReadThread::ReadThread()
{
        MsgBuf = new MessageBufferInterface(1000);
        QObject::connect(this, SIGNAL(ClearAll()),
                     MsgBuf,SLOT(ClearAll()));
}


//!SLOT for setting the device
//!PathArg is the path to the Device eg. /dev/pcanusb
//!For Baudrate see the Datasheet of the PCAN devices
//!MsgType has to be
void ReadThread::setDev(QString PathArg, int BaudRate, int MsgType)
{
    Dev = new CANDevice;
    Path = PathArg;

    if(!Dev->CANDeviceOpen(PathArg) || Dev->CANDeviceInit(BaudRate, MsgType))
    {
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage("Device could not be opened");
        ed->setModal(true);
        ed->show();
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
        Dev->CANSetFilter(from, to, MSGTYPE_STANDARD);
    }

    //clear all
    if(Place == HWFILTER && from == -1 && to == -1)
    {
        Dev->CANClearFilters();
    }
 }

void ReadThread::run()
{
    TPCANMsg Msg;
    struct timeval tv, starttime,dt;
    QuitNow = 0;
    gettimeofday( &starttime, NULL);

    while(1)
    {
        
        if(Dev->CANDeviceRead(&Msg))
        {
            gettimeofday( &tv, NULL);
            dt.tv_sec = tv.tv_sec - starttime.tv_sec;
            dt.tv_usec = tv.tv_usec;
            if(!MsgBuf->AddMessage(&Msg, &dt))
            {
                ErrorDialog *ed = new ErrorDialog;
                ed->SetErrorMessage("Messagebuffer overrun");
                ed->setModal(true);
                ed->show();
            }
        }
        if(QuitNow)
        {
            return;
        }
    }
}

void ReadThread::QuitThread()
{
    QuitNow = 1;
}
