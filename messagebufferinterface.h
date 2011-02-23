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

#include "obscan.h"
#include "configdialog.h"
#ifdef WINDOWS
#include <winsock2.h>
#endif
#define TEMPSTORE 1
#define NOSTORE 2



//!Storrage Class for all the CAN Messages.
//!From here a Qt Signal is emitted if a new Message arrives.
//!The Message is stored inside of a increasing Buffer.
class MessageBufferInterface : public QObject
{
  Q_OBJECT
public:
    MessageBufferInterface(void);
    int AddMessage(_CANMsg *Msg);
    ~MessageBufferInterface();
    int Save(char *Filename);
    int Load(char *Filename);
    void updateConfig(void);
private:
    int TMPFILE;

    int Mode;
    struct timeval tv_1;
    int Stop;

signals:
    void newMessage(_CANMsg , int);
    void newSpecialMessage(_CANMsg CANMsg);

public slots:
    void configChanged(__config cfg);
    int ClearAll();

};

#endif // MESSAGEBUFFERINTERFACE_H
