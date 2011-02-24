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
#ifndef OBSCAN_H
#define OBSCAN_H

#include "config.h"
#include <QMetaType>

#ifdef WINDOWS
#include <windef.h>
#include <winsock2.h>
#endif

#ifdef LINUX
#include <sys/time.h>
#define DWORD unsigned int
#define BYTE  unsigned char
#endif

// MSGTYPE bits of element MSGTYPE in structure TPCANMsg
#define MSGTYPE_STATUS        0x80     // used to mark a status TPCANMsg
#define MSGTYPE_EXTENDED      0x02     // declares a extended frame
#define MSGTYPE_RTR           0x01     // marks a remote frame
#define MSGTYPE_STANDARD      0x00     // marks a standard frame




struct _CANMsg
{
  DWORD ID;              // 11/29 bit code
  BYTE  MSGTYPE;         // bits of MSGTYPE_*
  BYTE  LEN;             // count of data bytes (0..8)
  BYTE  DATA[8];         // data bytes, up to 8
  timeval tv;
};

Q_DECLARE_METATYPE(_CANMsg)

#endif // CAN_H
