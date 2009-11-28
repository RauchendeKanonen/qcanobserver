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
