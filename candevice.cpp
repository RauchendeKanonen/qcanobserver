#include "candevice.h"
#include <fcntl.h>    // O_RDWR

#include <libpcan.h>
#include <errno.h>

CANDevice::CANDevice()
{
}
//!
int CANDevice::CANSetFilter(int FromID, int ToID, int nCANMsgType)
{
    if(DevHandle)
        CAN_MsgFilter(DevHandle, FromID, ToID, nCANMsgType);
    return 0;
}

int CANDevice::CANClearFilters(void)
{
    if(DevHandle)
        CAN_ResetFilter(DevHandle);
    return 0;
}

int CANDevice::CANDeviceOpen(QString Path)
{
    if(DevHandle = LINUX_CAN_Open(Path.toStdString().c_str(), O_RDWR))
        return 1;

    return 0;
}

int CANDevice::CANDeviceInit(int BaudRate, int MsgType)
{
    if(CAN_Init(DevHandle, BaudRate, MsgType))
        return 1;

    return 0;
}

int CANDevice::CANDeviceRead(TPCANMsg *Msg)
{
    if(CAN_Read(DevHandle, Msg))
        return 0;

    return 1;
}

int CANDevice::CANDeviceWrite(TPCANMsg Msg)
{
    if(CAN_Write(DevHandle, &Msg))
        return 0;

    return 1;
}

