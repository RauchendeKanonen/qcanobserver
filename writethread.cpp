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
#include "writethread.h"
#include <sys/time.h>
#include <QString>



#include "errordialog.h"

#ifdef WINDOWS
#include <windows.h>
#endif




WriteThread::WriteThread()
{
    RemovePeriodic = -1;
    setTerminationEnabled(true);
    WaitForMsg = new QWaitCondition();
    WaitMutex  = new QMutex();
}

WriteThread::~WriteThread()
{


    if(Dev && !DevLibInstIsShared)
        destroy(Dev);
}

//!SLOT for setting the device
//!PathArg is the path to the Device eg. /dev/pcanusb
//!For Baudrate see the Datasheet of the PCAN devices
//!MsgType has to be
void WriteThread::setDev(void *ConfData, QString InterfaceLib, bool shareDevLib)
{
    //we will get the instance form the read thread per signal
    DevLibInstIsShared = shareDevLib;
    if(shareDevLib)
        return;


#ifdef LINUX
    //Load the interface to the hardware
    void* handle = dlopen(InterfaceLib.toStdString().c_str(), RTLD_LAZY);



    if(!handle)
    {
        QString *ErrStr = new QString(" ");
        ErrStr->sprintf("%s %s","Could not load Device Mapper: ", InterfaceLib);
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage(*ErrStr);
        delete ErrStr;
        ed->setModal(true);
        ed->exec();
        delete ed;
        return;
    }





    create = (CANDevice* (*)())dlsym(handle, "create_object");
    destroy = (void (*)(CANDevice*))dlsym(handle, "destroy_object");


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
        ed->exec();
        Dev = NULL;
        delete ed;
        return;
    }

    create = (_create)GetProcAddress(PCANDll,"create_object");
    destroy = (_destroy)GetProcAddress(PCANDll,"destroy_object");
#endif

    if(!create || !destroy)
    {
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage("Could not find the expected symbols in the device mapper lib!");
        ed->setModal(true);
        ed->exec();
        Dev = NULL;
        delete ed;
        return;
    }

    Dev = (CANDevice*)create();

    int ret = Dev->CANDeviceOpen(ConfData);

    if(ELIBNOTFOUND == ret)
    {
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage("Could not find the vendor supplied library");
        ed->setModal(true);
        ed->exec();
        destroy(Dev);
        Dev = NULL;
        delete ed;
        return;
    }

    if(OPENFAILEDONSOCK == ret)
    {
        QString *Err;
        Err = new QString("Device could not be opened! Failure while creating the socket!");
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage(*Err);
        ed->setModal(true);
        ed->exec();
        destroy(Dev);
        Dev = NULL;
        delete ed;
        return;
    }

    if(OPENFAILEDONBIND == ret)
    {
        QString *Err;
        Err = new QString("Device could not be opened! Failure submitted by bind();!");
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage(*Err);
        ed->setModal(true);
        ed->exec();
        destroy(Dev);
        Dev = NULL;
        delete ed;
        return;
    }

    if(OPENSUCCESSFUL != ret)
    {
        QString *Err;
        Err = new QString("Device could not be opened!");
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage(*Err);
        ed->setModal(true);
        ed->exec();
        destroy(Dev);
        Dev = NULL;
        delete ed;
        return;
    }


    if(OPENFAILONINIT == ret)
    {
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage("Device could not be initialized");
        ed->setModal(true);
        ed->exec();
        destroy(Dev);
        Dev = NULL;
        delete ed;
        return;
    }
    
}

bool WriteThread::isConfigured()
{
   if(Dev)
       return true;

   return false;
}
void WriteThread::run()
{
    struct timeval tv;
    unsigned long sleeptime;
    struct timeval starttime;
    gettimeofday(&starttime,NULL);
    unsigned long ProcessThreadEvts = 1;
    unsigned long newNextTrg;
    unsigned long dbg;


    QuitNow = 0;
    QEventLoop *loop = new QEventLoop();

    if(Dev == NULL)
    {
        delete loop;
        return;
    }

    while(1)
    {

        if(QuitNow)
        {
            delete loop;
            return;
        }
        //We shall remove a msg
        if(RemovePeriodic >= 0)
        {
            CANMsgPeriodic.removeAt(RemovePeriodic);
            Period.removeAt(RemovePeriodic);
            NextTrg.removeAt(RemovePeriodic);
            RemovePeriodic = -1;
        }

        int cnt = Period.count();

        gettimeofday(&tv,NULL);

        usCounter = (tv.tv_sec - starttime.tv_sec) * 1000000;
        usCounter += (tv.tv_usec - starttime.tv_usec);

        if((tv.tv_sec % 2) == 0 && (tv.tv_sec != ProcessThreadEvts))
        {
            ProcessThreadEvts = 1;
        }


        if(ProcessThreadEvts == 1)
        {
            ProcessThreadEvts = tv.tv_sec;
            loop->processEvents(QEventLoop::ExcludeUserInputEvents);
        }

        sleeptime = 1000000;       //1s

        for(int s = 0 ; s < cnt ; s ++ )
        {

            if(NextTrg.at(s) <= usCounter)
            {
                Dev->CANDeviceWrite(CANMsgPeriodic.at(s));

                newNextTrg = NextTrg.at(s) + Period.at(s);
                NextTrg.replace(s, newNextTrg);
            }

            if(((unsigned long)( NextTrg.at(s) - usCounter ) < (unsigned long)sleeptime) && (sleeptime != 0))
                sleeptime = (NextTrg.at(s)-usCounter);

            if( newNextTrg <= usCounter )
                sleeptime = 0;
        }


        if(sleeptime > 50 && !CANMsgFifo.count())
        {
            //sleep more than ms and stay wakable
            if(sleeptime > 5000)        //more than 5 ms
            {
                WaitMutex->lock();
                WaitForMsg->wait(WaitMutex, sleeptime/1000);         //in ms
                WaitMutex->unlock();
            }

            //Sleep only some us
            else
                usleep(sleeptime - 25);

        }



        if(CANMsgFifo.count())
        {
            Dev->CANDeviceWrite(CANMsgFifo.at(0));
            CANMsgFifo.removeAt(0);
        }

    }
}

void WriteThread::QuitThread()
{
    //QuitNow = 1;
}

void WriteThread::sendCANMsg(_CANMsg Msg, int aPeriod, _CANMsg TrigMsg, int SendType)
{
    if(SendType == SEND_SINGLE)
    {
        WaitForMsg->wakeAll();
        CANMsgFifo.append(Msg);
        return;
    }
    if(SendType == SEND_TRIG)
    {
        WaitForMsg->wakeAll();
        CANMsgTrigLst.append(TrigMsg);
        CANMsgOnTrigLst.append(Msg);
        return;
    }

    if(SendType == SEND_PERIODIC)
    {
        WaitForMsg->wakeAll();
        CANMsgPeriodic.append(Msg);
        Period.append(aPeriod);
        //now
        NextTrg.append(usCounter);
    }
}

void WriteThread::deleteCANMsg(_CANMsg Msg, int aPeriod, _CANMsg TrigMsg, int SendType)
{
    if(SendType == SEND_PERIODIC)
    {
        for(int c = 0 ; CANMsgPeriodic.count() > c ; c++ )
        {
            if(Msg.ID == CANMsgPeriodic.at(c).ID)
            {
                if(aPeriod == Period.at(c))
                {
                    if(memcmp(Msg.DATA, CANMsgPeriodic.at(c).DATA, 8) == 0)
                    {
                        //We shall remove a msg
                        RemovePeriodic = c;
                    }
                }
            }
        }
    }
}

void WriteThread::addnewMessage(_CANMsg *Msg, int Cnt)
{
    int cnt = CANMsgTrigLst.count();

    for(int f = 0; f < cnt ; f ++ )
    {
        if(CANMsgTrigLst.at(f).ID == Msg->ID)
        {
            if(memcmp(CANMsgTrigLst.at(f).DATA,Msg->DATA, 8) == 0 )
                Dev->CANDeviceWrite(CANMsgOnTrigLst.at(f));

        }
    }
}

//Got the CANDevice Instance from readthread
//using now the same one
void WriteThread::setDevLibInstance(CANDevice *ExtDev)
{
    if(Dev && !DevLibInstIsShared)
        destroy(Dev);

    Dev = ExtDev;
}
