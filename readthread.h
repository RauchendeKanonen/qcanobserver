//#include <QObject>

#ifndef READTHREAD_H
#define READTHREAD_H

#include <qthread.h>
#include "candevice.h"
#include "messagebufferinterface.h"
#include "FilterDialog.h"
//#include <libpcan.h>


class ReadThread : public QThread
{
 Q_OBJECT
public:
    ReadThread();
    virtual void run();
    MessageBufferInterface *MsgBuf;

signals:
    void ClearAll();

public slots:
    void setDev(QString PathArg, int BaudRate, int MsgType);
    void QuitThread();
    void setFilter(int Place, int from, int to);
private:
     QString Path;
     CANDevice *Dev;
     int QuitNow;
};

#endif // READTHREAD_H
