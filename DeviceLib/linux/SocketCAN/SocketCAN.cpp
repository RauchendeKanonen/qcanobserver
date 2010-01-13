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





#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <libgen.h>
#include <getopt.h>
#include <time.h>
#include <sched.h>
#include <limits.h>
#include <errno.h>

#include <QMessageBox>

#include "../../candevice.h"
#include "confdialog.h"

typedef struct config
{
    char NetDevName[IFNAMSIZ];
    int logErrFrs;
    int LoopOwnMsgs;
};

extern "C" int getDeviceFlags(void)
{
    int Flags = 0;


    return Flags;
}

extern "C" void* createConfig(void *oldconf)
{
    struct config cfg;
    ConfDialog dlg;

    void *cmpbuf = malloc(CONFDATA_SIZEMAX);
    memset(cmpbuf, 0, CONFDATA_SIZEMAX);

    /* Create the socket */
    int locsock = socket( PF_CAN, SOCK_RAW, CAN_RAW );
    struct ifreq ifr;
    QStringList NetDevList;
    int ret = 0;


    //search all networkdevices
    for( ifr.ifr_ifindex = 1 ; ret == 0 ; ifr.ifr_ifindex++)
    {
        ret = ioctl(locsock, SIOCGIFNAME, &ifr);
        NetDevList.append(QString(ifr.ifr_ifrn.ifrn_name));
    }

    dlg.SetNetworks(NetDevList);

    if(memcmp(oldconf, cmpbuf, CONFDATA_SIZEMAX))
    {
        memcpy((void*)&cfg, oldconf, sizeof(cfg));
        dlg.setValues(QString(cfg.NetDevName),(bool)cfg.logErrFrs, (bool)cfg.LoopOwnMsgs);
    }
    free(cmpbuf);


    dlg.setModal(true);
    dlg.exec();

    QString NetDevName;

    bool LogErr;
    bool loopown;

    dlg.getValues( &NetDevName,&LogErr, &loopown);

    cfg.logErrFrs = 0;
    cfg.LoopOwnMsgs = 0;

    if(LogErr)
        cfg.logErrFrs = 1;

    if(loopown)
        cfg.LoopOwnMsgs = 1;


    memset(cfg.NetDevName, 0, sizeof(cfg.NetDevName));
    memcpy(cfg.NetDevName, NetDevName.toStdString().c_str(), NetDevName.count());

    memset(oldconf, 0, CONFDATA_SIZEMAX);
    memcpy(oldconf, (void*)(&cfg), sizeof(cfg));
    return oldconf;
}


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

CANDevice::CANDevice()
{
    DevHandle = NULL;
}

CANDevice::~CANDevice()
{
    close(sock);
    DevHandle = NULL;
}

//!
int CANDevice::CANSetFilter(int FromID, int ToID, int nCANMsgType)
{
    if((ToID - FromID) < 0)
        return OPFAIL;

    struct can_filter rfilter[1+(ToID - FromID)];

    int ID = FromID;

    for(int i = 0; i <= (ToID-FromID) ; i ++, ID ++)
    {
        rfilter[i].can_id   = ID;
        rfilter[i].can_mask = CAN_SFF_MASK;
    }

    errno = 0;
    setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, (1+(ToID - FromID))*sizeof(can_filter));

    FromID = errno;

    return OPSUCCESS;
}

int CANDevice::CANClearFilters(void)
{

    struct can_filter rfilter;

    for(int i = 0; i <= CAN_SFF_MASK; i ++ )
    {
        rfilter.can_id   = i;
        rfilter.can_mask = CAN_SFF_MASK;
        setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(can_filter));
    }

    return OPSUCCESS;
}

int CANDevice::CANDeviceOpen(void*ConfigBuf)
{
    struct config cfg;
    memcpy((void*)&cfg, ConfigBuf, sizeof(cfg));

    QString NetDev(cfg.NetDevName);
    /* Create the socket */
    sock = socket( PF_CAN, SOCK_RAW, CAN_RAW );
    if(sock <= 0)
            return OPENFAILEDONSOCK;

    //loop back send msg when opened a virtualcan netdev
    if(cfg.LoopOwnMsgs)
    {
        int recv_own_msgs = 1; /* 0 = disabled (default), 1 = enabled */

        setsockopt(sock, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS,
                   &recv_own_msgs, sizeof(recv_own_msgs));
    }
    /* Locate the interface you wish to use */
    struct ifreq ifr;
    strcpy(ifr.ifr_name, NetDev.toStdString().c_str());
    int ret = ioctl(sock, SIOCGIFINDEX, &ifr); /* ifr.ifr_ifindex gets filled
                                  * with that device's index */

    /* Select that CAN interface, and bind the socket to it. */
    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    ret = bind( sock, (struct sockaddr*)&addr, sizeof(addr) );

    if(ret != 0)
    {
        close(sock);
        return OPENFAILEDONBIND;
    }

    if(cfg.logErrFrs)
    {
        can_err_mask_t err_mask = CAN_ERR_MASK;//all possible errors

        ret = setsockopt(sock, SOL_CAN_RAW, CAN_RAW_ERR_FILTER,
                         &err_mask, sizeof(err_mask));
    }
    /*Listing 2. Manually setting the send and receive socket buffer sizes

    int ret, sock, sock_buf_size;

    sock = socket( AF_INET, SOCK_STREAM, 0 );
    sock_buf_size = BDP;

    ret = setsockopt( sock, SOL_SOCKET, SO_SNDBUF,
                   (char *)&sock_buf_size, sizeof(sock_buf_size) );

    ret = setsockopt( sock, SOL_SOCKET, SO_RCVBUF,
                   (char *)&sock_buf_size, sizeof(sock_buf_size) );

    */

    //try to read
    errno = 0;
    timeval tv;
    struct can_frame rx_frame;
    int bytes_read = read( sock, &rx_frame, sizeof(rx_frame) );
    ret = ioctl(sock, SIOCGSTAMP, &tv);

    int err = errno;
    switch(errno)
    {
    case ENOENT:
        QMessageBox::critical( 0,
                               QString( "Critical Error" ),
                               QString( "Cannot read from the Device (ENOENT)" ));
        break;
    case EAGAIN:
        break;
              case EBADF:
        break;
              case EFAULT:
        break;
              case EINTR:
        break;
              case EIO:
        break;
              case EISDIR:
        break;
    case ENETDOWN:
        QMessageBox::critical( 0,
                               QString( "Critical Error" ),
                               QString( "Net is down (ENETDOWN)" ));
        break;
    }

    return OPENSUCCESSFUL;
}

int CANDevice::CANDeviceInit(int BaudRate, int MsgType)
{
    return 0;
}

int CANDevice::CANDeviceRead(_CANMsg *Msg)
{
    /* Read a message back from the CAN bus */
    long timestamp;
    errno = 0;
    struct can_frame rx_frame;
    int bytes_read = read( sock, &rx_frame, sizeof(rx_frame) );
    int ret = ioctl(sock, SIOCGSTAMP, &Msg->tv);
    memcpy(Msg->DATA, rx_frame.data, 8);
    Msg->ID = rx_frame.can_id;
    Msg->LEN = bytes_read;

    return OPSUCCESS;
}


int CANDevice::CANDeviceWrite(_CANMsg Msg)
{
    struct can_frame tx_frame;
    tx_frame.can_dlc = 8;
    tx_frame.can_id = Msg.ID;
    memcpy(tx_frame.data, Msg.DATA, 8);
    int wrote_bytes = write( sock, &tx_frame, sizeof(tx_frame) );

    return 1;
}

