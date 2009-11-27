#include <QObject>
#ifndef CANDEVICE_H
#define CANDEVICE_H

#include<pcan.h>


//redefined HANDLE because of Problems with Qt
#define HANDLEI void*


//!Interface for the CAN Devices
//!We need Classe derived from this Standart Type to implement
//!other CAN Hardware Interfaces. Maybe one can do this task also for
//!differences between Linux and Windows
class CANDevice// : public QObject
{
    //Q_OBJECT
public:
    CANDevice();
    int CANDeviceOpen(QString Path);
    int CANDeviceInit(int BaudRate, int MsgType);
    int CANDeviceClose(void);
    int CANDeviceRead(TPCANMsg *Msg);
    int CANDeviceWrite(TPCANMsg Msg);
    int CANSetFilter(int FromID, int ToID, int nCANMsgType);
    int CANClearFilters(void);
private:
    HANDLEI  DevHandle;
};

#endif // CANDEVICE_H
