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


#ifndef WRITETHREAD_H
#define WRITETHREAD_H

#include "config.h"
#include <sys/time.h>
#include "errordialog.h"
#include <qthread.h>
#include "DeviceLib/candevice.h"
#include "messagebufferinterface.h"
#include "FilterDialog.h"
#include "obscan.h"
#include "sendmsgdialog.h"
#include <QEventLoop>

#ifdef LINUX
#include <dlfcn.h>
#endif

#ifdef WINDOWS
typedef CANDevice* (CALLBACK* _create)(void);
typedef void (CALLBACK* _destroy)(CANDevice*);
#endif


#include <iostream>



class WriteThread : public QThread
{
 Q_OBJECT
public:
    WriteThread();
    ~WriteThread();
    virtual void run();
    bool isConfigured();
signals:
    void ClearAll();

public slots:
    void addnewMessage(_CANMsg *, int);
    void setDev(void *ConfData, QString InterfaceLib, bool shareDevLib);
    void QuitThread();
    void sendCANMsg(_CANMsg Msg, int Period, _CANMsg TrigMsg, int SendType);
    void deleteCANMsg(_CANMsg Msg, int Period, _CANMsg TrigMsg, int SendType);
    void setDevLibInstance(CANDevice *);
private:
    QString Path;
    CANDevice *Dev;
    int QuitNow;
    bool DevLibInstIsShared;


    unsigned long usCounter;
    QList <_CANMsg> CANMsgFifo;

    QList <_CANMsg> CANMsgPeriodic;
    QList <unsigned long> NextTrg;
    QList <int>     Period;
    //Syncronisation
    int RemovePeriodic;         //index to remove

    QList <_CANMsg> CANMsgOnTrigLst;
    QList <_CANMsg> CANMsgTrigLst;

#ifdef WINDOWS
    _create create;
    _destroy destroy;
#endif
#ifdef LINUX
    CANDevice* (*create)();
    void (*destroy)(CANDevice*);
#endif
};

#endif // WRITETHREAD_H
