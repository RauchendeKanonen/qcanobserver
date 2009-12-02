#ifndef SENDMSGDIALOG_H
#define SENDMSGDIALOG_H

#include <QtGui/QDialog>
#include "msgdefdialog.h"
#include "stringlistmodel.h"
#include "candevice.h"
#include <QItemSelection>

namespace Ui {
    class SendMsgDialog;
}

class SendMsgDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SendMsgDialog)
public:
    explicit SendMsgDialog(QWidget *parent = 0);
    virtual ~SendMsgDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SendMsgDialog *m_ui;
    MsgDefDialog *SMsg;
    StringListModel *MsgModel;
    QModelIndex SelectedModelIndex;
    QModelIndexList SelectedMsgs;

signals:
    void sendCANMsg(TPCANMsg *);

public slots:
        void newMsgAccepted(QString , QString , QString , QString , QString
                   , QString , QString , QString , QString );
        void selectionChanged(const QItemSelection &,const QItemSelection &);

private slots:
    void on_delete_2_clicked();
    void on_SendButton_clicked();
    void on_MsgtableView_entered(QModelIndex index);
    void on_MsgtableView_doubleClicked(QModelIndex index);
    void on_MsgtableView_clicked(QModelIndex index);
    void on_pushButton_3_clicked();
};

#endif // SENDMSGDIALOG_H
