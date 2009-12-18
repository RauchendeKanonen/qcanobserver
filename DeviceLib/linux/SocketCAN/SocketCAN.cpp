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

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "../../candevice.h"

struct ifreq ifr;
struct sockaddr_can addr;
int family = PF_CAN, type = SOCK_RAW, proto = CAN_RAW;
int echo_gen = 0;
int opt, err;
static int sockfd;

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
    close(sockfd);
    DevHandle = NULL;
}

//!
int CANDevice::CANSetFilter(int FromID, int ToID, int nCANMsgType)
{
//    if(DevHandle)
//        CAN_MsgFilter(DevHandle, FromID, ToID, nCANMsgType);
//    return 0;
}

int CANDevice::CANClearFilters(void)
{

    return 0;
}

int CANDevice::CANDeviceOpen(QString Path)
{
    /* Create the socket */
    sockfd = socket( PF_CAN, SOCK_RAW, CAN_RAW );
    if(sockfd <= 0)
            return OPENFAILED;
    /* Locate the interface you wish to use */
    struct ifreq ifr;
    strcpy(ifr.ifr_name, Path.toStdString().c_str());
    int ret = ioctl(sockfd, SIOCGIFINDEX, &ifr); /* ifr.ifr_ifindex gets filled
                                  * with that device's index */

    /* Select that CAN interface, and bind the socket to it. */
    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    ret = bind( sockfd, (struct sockaddr*)&addr, sizeof(addr) );

    if(ret != 0)
    {
        close(sockfd);
        return OPENFAILED;
    }

    can_err_mask_t err_mask = CAN_ERR_MASK;//all possible errors

    ret = setsockopt(sockfd, SOL_CAN_RAW, CAN_RAW_ERR_FILTER,
               &err_mask, sizeof(err_mask));

    /*
    addr.can_family = family;
    strcpy(ifr.ifr_name, Path.toStdString().c_str());
    if ((sockfd = socket(family, type, proto)) < 0)
        return OPENFAILED;

    ioctl(sockfd, SIOCGIFINDEX, &ifr);
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(sockfd);
        return OPENFAILED;
    }*/
    return OPENSUCCESSFUL;
}

int CANDevice::CANDeviceInit(int BaudRate, int MsgType)
{
//    if(CAN_Init(DevHandle, BaudRate, MsgType))
        //return 1;

    return 0;
}

int CANDevice::CANDeviceRead(_CANMsg *Msg)
{
    /* Read a message back from the CAN bus */
    long timestamp;
    errno = 0;
    struct can_frame rx_frame;
    int bytes_read = read( sockfd, &rx_frame, sizeof(rx_frame) );
    int ret = ioctl(sockfd, SIOCGSTAMP, &Msg->tv);

    int err = errno;

    switch(errno)
    {
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
        break;
              case ENOENT:
        break;
    }

    memcpy(Msg->DATA, rx_frame.data, 8);
    Msg->ID = rx_frame.can_id;
    Msg->LEN = bytes_read;

    return 1;
}


int CANDevice::CANDeviceWrite(_CANMsg Msg)
{
//    TPCANMsg HWMsg;
//    HWMsg.ID = Msg.ID;
//    memcpy(HWMsg.DATA, Msg.DATA, 8);
//    HWMsg.LEN = Msg.LEN;
//    HWMsg.MSGTYPE = Msg.MSGTYPE;
//
//    if(CAN_Write(DevHandle, &HWMsg))
//        return 0;

    return 1;
}

