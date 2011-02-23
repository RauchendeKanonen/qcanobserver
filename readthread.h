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
//#include <QObject>

#ifndef READTHREAD_H
#define READTHREAD_H

#include "config.h"

#include "errordialog.h"
#include <qthread.h>
#include "DeviceLib/candevice.h"
#include "messagebufferinterface.h"
#include "FilterDialog.h"
#include "obscan.h"

#include <QEventLoop>

#ifdef LINUX
#include <dlfcn.h>
#endif

#include <iostream>

#ifdef WINDOWS
typedef CANDevice* (CALLBACK* _create)(void);
typedef void (CALLBACK* _destroy)(CANDevice*);
#endif

class ReadThread : public QThread
{
    Q_OBJECT
public:
    ReadThread();
    ~ReadThread();
    virtual void run();
    MessageBufferInterface *MsgBuf;
    bool isConfigured();
signals:
    void ClearAll();
    void setDevLibInstance(CANDevice *);
    void DevIsConfigured(bool isIt);
    void NoMem();
    void configChanged(__config);
public slots:
    void setDev(void *ConfData, QString InterfaceLib, bool shareDevLib);
    void QuitThread();
    void setFilter(int Place, int from, int to);

private:
    QString Path;
    CANDevice *Dev;
    int QuitNow;

#ifdef WINDOWS
    _create create;
    _destroy destroy;
#endif
#ifdef LINUX
     CANDevice* (*create)();
     void (*destroy)(CANDevice*);
#endif
};

#endif // READTHREAD_H
