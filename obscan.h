#ifndef OBSCAN_H
#define OBSCAN_H

#include "config.h"


#ifdef WINDOWS
#include <windef.h>
#endif

#ifdef LINUX
#define DWORD unsigned int
#define BYTE  unsigned char
#endif

// MSGTYPE bits of element MSGTYPE in structure TPCANMsg
#define MSGTYPE_STATUS        0x80     // used to mark a status TPCANMsg
#define MSGTYPE_EXTENDED      0x02     // declares a extended frame
#define MSGTYPE_RTR           0x01     // marks a remote frame
#define MSGTYPE_STANDARD      0x00     // marks a standard frame




typedef struct
{
  DWORD ID;              // 11/29 bit code
  BYTE  MSGTYPE;         // bits of MSGTYPE_*
  BYTE  LEN;             // count of data bytes (0..8)
  BYTE  DATA[8];         // data bytes, up to 8
  timeval tv;
} _CANMsg;              // for PCAN_WRITE_MSG



#endif // CAN_H
