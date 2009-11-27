
#ifndef DEVDIALOG_H
#define DEVDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class DevDialog;
}

class DevDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(DevDialog)
public:
    explicit DevDialog(QWidget *parent = 0);
    virtual ~DevDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DevDialog *m_ui;

signals:
    void setDev(QString PathArg, int BaudRate, int MsgType);

private slots:
    void on_buttonBox1_accepted();
};

#endif // DEVDIALOG_H
