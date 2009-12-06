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

#include <qthread.h>
#include "DeviceLib/candevice.h"
#include "messagebufferinterface.h"
#include "FilterDialog.h"
//#include <libpcan.h>
#include <dlfcn.h>
#include <iostream>



class ReadThread : public QThread
{
 Q_OBJECT
public:
    ReadThread();
    virtual void run();
    MessageBufferInterface *MsgBuf;
    bool isConfigured();
signals:
    void ClearAll();

public slots:
    void setDev(QString PathArg, int BaudRate, int MsgType, QString InterfaceLib);
    void QuitThread();
    void setFilter(int Place, int from, int to);
    void sendCANMsg(TPCANMsg *);
private:
     QString Path;
     CANDevice *Dev;
     int QuitNow;
};

#endif // READTHREAD_H
