#ifndef MSGDEFDIALOG_H
#define MSGDEFDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class MsgDefDialog;
}

class MsgDefDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(MsgDefDialog)
public:
    explicit MsgDefDialog(QWidget *parent = 0, QString ID = 0 );
    virtual ~MsgDefDialog();

protected:
    virtual void changeEvent(QEvent *e);

signals:
    void newMsgAccepted(QString,
                        QString, QString , QString , QString,
                        QString, QString , QString , QString,
                        QString,
                        QString,
                        QString,
                        QString, QString , QString , QString ,
                        QString, QString , QString , QString);
private:
    Ui::MsgDefDialog *m_ui;

private slots:
    void on_checkBoxTrigger_toggled(bool checked);
    void on_checkBox_toggled(bool checked);
    void on_buttonBox_accepted();
};

#endif // MSGDEFDIALOG_H
