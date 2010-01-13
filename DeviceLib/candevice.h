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
#include <QObject>
#include <QString>
#ifndef CANDEVICE_H
#define CANDEVICE_H
#include"../config.h"
#include"../obscan.h"

#define ELIBNOTFOUND    -1
#define OPENSUCCESSFUL   1
#define OPENFAILED       0
#define OPENFAILEDONSOCK       -2
#define OPENFAILEDONBIND      -3
#define OPSUCCESS          2
#define OPFAIL              -4
#define OPENFAILONINIT  -5

#define CONFDATA_SIZEMAX 10*1024


//redefined HANDLE because of Problems with Qt
#define HANDLEI void*

#ifdef LINUX
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#endif

#ifdef LOOPBACK
#include <QMutex>
#endif

extern "C" int getDeviceFlags(void);
extern "C" void* createConfig(void *);

//!Interface for the CAN Devices
//!We need Classe derived from this Standart Type to implement
//!other CAN Hardware Interfaces. Maybe one can do this task also for
//!differences between Linux and Windows
class CANDevice// : public QObject
{
    //Q_OBJECT
public:

    CANDevice();
    ~CANDevice();
    virtual int CANDeviceOpen(void*ConfigBuf);
    virtual int CANDeviceInit(int BaudRate, int MsgType);
    //virtual int CANDeviceClose(void);
    virtual int CANDeviceRead(_CANMsg *Msg);
    virtual int CANDeviceWrite(_CANMsg Msg);
    virtual int CANSetFilter(int FromID, int ToID, int nCANMsgType);
    virtual int CANClearFilters(void);
private:

    bool    isConfigured;

    //needed by pcan
    HANDLEI  DevHandle;

#ifdef LINUX
    //Stuff needed by the SocketCAN Module
    //needs to be declared here because in this way it is thread save
    struct ifreq ifr;
    struct sockaddr_can addr;
    int opt, err;
    int sock;
#endif
#ifdef LOOPBACK
    QList <_CANMsg> MsgLst;
    QMutex Mutex;
#endif
};

#endif // CANDEVICE_H
