#ifndef SENDMSGDIALOG_H
#define SENDMSGDIALOG_H

#include <QtGui/QDialog>
#include "msgdefdialog.h"
#include "stringlistmodel.h"
#include "DeviceLib/candevice.h"
#include <QItemSelection>
#include "obscan.h"
#include <QCloseEvent>
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
    explicit SendMsgDialog(QWidget *parent = 0);
    ~SendMsgDialog();

protected:
    virtual void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private:
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
