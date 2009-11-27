#include "messagebufferinterface.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

MessageBufferInterface::MessageBufferInterface(int size)
{
    tv_1.tv_sec = 0;
    tv_1.tv_usec = 0;
    MsgIndex = 0;

    MsgBufsize = size;
    CANMsgandTime = (CANMsgandTimeStruct*)malloc(sizeof(CANMsgandTimeStruct)*size);
}

//!Adds a Message to the internal increasing Buffer.
//!Emits newMessage(CANMsgandTimeStruct* CANMsgandTime, int NumOfMsgs);
int MessageBufferInterface::AddMessage(TPCANMsg *Msg, struct timeval *tv)
{
    if(MsgIndex >= MsgBufsize)
    {
        CANMsgandTime = (CANMsgandTimeStruct*)realloc((CANMsgandTimeStruct*)CANMsgandTime, (MsgBufsize + 1000000) * sizeof(CANMsgandTimeStruct));
        MsgBufsize = (MsgBufsize + 1000000);
    }


    (CANMsgandTime+MsgIndex)->CANMsg = *Msg;
    (CANMsgandTime+MsgIndex)->timev = *tv;

    MsgIndex++;

    emit newMessage(CANMsgandTime+MsgIndex-1, MsgIndex);
    return 1;


    return 0;
}

int MessageBufferInterface::AddMessage(CANMsgandTimeStruct *MsgandTime)
{
    if(MsgIndex >= MsgBufsize)
    {
        CANMsgandTime = (CANMsgandTimeStruct*)realloc((CANMsgandTimeStruct*)CANMsgandTime, (MsgBufsize + 1000000) * sizeof(CANMsgandTimeStruct));
        MsgBufsize = (MsgBufsize + 1000000);
    }


    (CANMsgandTime+MsgIndex)->CANMsg = MsgandTime->CANMsg;
    (CANMsgandTime+MsgIndex)->timev = MsgandTime->timev;

    MsgIndex++;



    emit newMessage(CANMsgandTime+MsgIndex-1, MsgIndex);
    return 1;
}


int MessageBufferInterface::GetMessage(TPCANMsg *Msg, int idx)
{
    if(MsgIndex > 0 && MsgIndex <= idx)
    {
        Msg = pTPCANMsg+idx;
        return 1;
    }

    return 0;
}
//!Saves the internal Buffer to a File
int MessageBufferInterface::Save(char *Filename)
{
    int LengthOfBuf = MsgIndex * sizeof(CANMsgandTimeStruct);
    int OUTFILE;



    if((OUTFILE = open(Filename,O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU|S_IRWXG|S_IRWXO))<=0)
      return -1;

    int length = write(OUTFILE, CANMsgandTime, LengthOfBuf);

    return 0;
}
//!Loads Messages from Filename, adds them to the Buffer via AddMessage (TPCANMsg *Msg, timeval *tv)
int MessageBufferInterface::Load(char *Filename)
{
    int INFILE;
    int length, t;
    CANMsgandTimeStruct Msg;


    if(-1==(INFILE = open(Filename,O_RDONLY)))
      return -1;

    for(length=0,t=1;t!=0;)
    {

        t = read(INFILE,((char*)&Msg),sizeof(Msg));
        AddMessage(&Msg);
    }


    return 0;
}

//!Resets the internal Messagecounter to zero. But does not free the mem
int MessageBufferInterface::ClearAll()
{
    MsgIndex = 0;
    return 1;
}
