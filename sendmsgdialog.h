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
#ifndef SENDMSGDIALOG_H
#define SENDMSGDIALOG_H

#include <QtGui/QDialog>
#include "msgdefdialog.h"
#include "stringlistmodel.h"
#include "DeviceLib/candevice.h"
#include <QItemSelection>
#include "obscan.h"
#include <QCloseEvent>
#include "cansignal.h"
#include "cansignalcollection.h"
#include "signalselectordialog.h"
#define SEND_PERIODIC 1
#define SEND_SINGLE   2
#define SEND_TRIG     3


namespace Ui {
    class SendMsgDialog;
}

class SendMsgDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SendMsgDialog)
public:
    ofstream& operator>>(ofstream& os);
    ifstream& operator<<(ifstream& is);
    explicit SendMsgDialog(QWidget *parent = 0, CANSignalCollection *Collection = 0);
    ~SendMsgDialog();

protected:
    virtual void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    QList <SignalSelectorDialog*> *Sel;
    CANSignalCollection *pCollection;
    Ui::SendMsgDialog *m_ui;
    MsgDefDialog *SMsg;
    StringListModel *MsgModel;
    QModelIndex SelectedModelIndex;
    QModelIndex SelectedMsg;
    QColor black;
    QColor red;
    QColor green;

signals:
    void sendCANMsg(_CANMsg Msg, int aPeriod, _CANMsg TrigMsg, int SendType);
    void deleteCANMsg(_CANMsg Msg, int Period, _CANMsg TrigMsg, int SendType);
public slots:
    void newMsgAccepted(QString,
                        QString, QString , QString , QString,
                        QString, QString , QString , QString,
                        QString,
                        QString,
                        QString,
                        QString, QString , QString , QString ,
                        QString, QString , QString , QString);
   void selectionChanged(const QItemSelection &,const QItemSelection &);

private slots:
    void on_fromDbButton_clicked();
    void on_MsgtableView_pressed(QModelIndex index);
    void on_MsgtableView_activated(QModelIndex index);
    void on_DeleteIdButton_clicked();
    void on_NewIdButton_clicked();
    void on_SendStopButton_clicked();
    void on_SendButton_clicked();
    void on_MsgtableView_doubleClicked(QModelIndex index);
    void on_MsgtableView_clicked(QModelIndex index);
};

#endif // SENDMSGDIALOG_H
