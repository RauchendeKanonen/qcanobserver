#include <QObject>
#ifndef MESSAGEBUFFERINTERFACE_H
#define MESSAGEBUFFERINTERFACE_H

#include <pcan.h>

    typedef struct CANMsgandTimeStruct
    {
        TPCANMsg CANMsg;
        struct timeval timev;
    };


//!Storrage Class for all the CAN Messages.
//!From here a Qt Signal is emitted if a new Message arrives.
//!The Message is stored inside of a increasing Buffer.
class MessageBufferInterface : public QObject
{
  Q_OBJECT
public:
    int GetMessage(TPCANMsg *Msg, int idx);
    MessageBufferInterface(int size);
    int AddMessage(TPCANMsg *Msg, timeval *tv);

    int Save(char *Filename);
    int Load(char *Filename);
    int AddMessage(CANMsgandTimeStruct *MsgandTime);

private:
    TPCANMsg *pTPCANMsg;
    int MsgIndex;
    int MsgBufsize;
    CANMsgandTimeStruct *CANMsgandTime;
    struct timeval tv_1;


signals:
    void newMessage(CANMsgandTimeStruct *, int);

public slots:
    int ClearAll();



};

#endif // MESSAGEBUFFERINTERFACE_H
