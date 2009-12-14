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
#include "messagebufferinterface.h"
#include "errordialog.h"
#include <stdio.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

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
int MessageBufferInterface::AddMessage(_CANMsg *Msg, struct timeval *tv)
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


int MessageBufferInterface::GetMessage(_CANMsg *Msg, int idx)
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

#ifdef LINUX
    if((OUTFILE = open(Filename,O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU|S_IRWXG|S_IRWXO))<=0)
      return -1;
#endif

#ifdef WINDOWS
    if((OUTFILE = open(Filename,O_CREAT | O_WRONLY |O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE))<=0)
      return -1;
#endif

    int flength = write(OUTFILE, CANMsgandTime, LengthOfBuf);
    close(OUTFILE);
    if(LengthOfBuf != flength)
    {
        QString *Err = new QString();
        Err->sprintf("Could not Save the Capture File propperly!");
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage(*Err);
        delete Err;
        ed->setModal(true);
        ed->show();
        //delete ed;
    }
    return 0;
}
//!Loads Messages from Filename, adds them to the Buffer via AddMessage (TPCANMsg *Msg, timeval *tv)
int MessageBufferInterface::Load(char *Filename)
{
    int INFILE;
    int length, t;
    CANMsgandTimeStruct Msg;
    int i;

#ifdef LINUX
    if(-1==(INFILE = open(Filename,O_RDONLY)))
      return -1;

    for(length=0,t=1;t>0;)
    {
        t = read(INFILE,((char*)&Msg),sizeof(Msg));

        if( t == sizeof(Msg) && (t > 0))
            AddMessage(&Msg);

        else if( t != sizeof(Msg) && (t != 0))
        {
            QString *Err = new QString();
            Err->sprintf("Could not Load the File properly! The file seems to be malformed!");
            ErrorDialog *ed = new ErrorDialog;
            ed->SetErrorMessage(*Err);
            delete Err;
            ed->setModal(true);
            ed->show();
            return 0;
        }
    }
#endif

#ifdef WINDOWS
    if(-1==(INFILE = _open(Filename,O_RDONLY|_O_BINARY)))
      return -1;

    for(length=0,t=1;t>0;)
    {
        t = _read(INFILE,((char*)&Msg),sizeof(Msg));
        length += t;

        if( t == sizeof(Msg) && (t > 0))
            AddMessage(&Msg);

        else if( t != sizeof(Msg) && (t != 0))
        {
            QString *Err = new QString();
            Err->sprintf("Could not Load the File properly! The file seems to be malformed!");
            ErrorDialog *ed = new ErrorDialog;
            ed->SetErrorMessage(*Err);
            delete Err;
            ed->setModal(true);
            ed->show();
            return 0;
        }

    }
#endif
    return 0;
}

//!Resets the internal Messagecounter to zero. But does not free the mem
int MessageBufferInterface::ClearAll()
{
    MsgIndex = 0;
    return 1;
}
