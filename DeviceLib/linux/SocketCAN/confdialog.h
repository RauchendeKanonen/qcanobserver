#ifndef CONFDIALOG_H
#define CONFDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class ConfDialog;
}

class ConfDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ConfDialog)
public:
    explicit ConfDialog(QWidget *parent = 0);
    virtual ~ConfDialog();
    void getValues(QString *DevName, bool *logErrFrs, bool *LoopOwnMsgs);

    int setValues(QString DevName, bool logErrFrs, bool LoopOwnMsgs);

    void SetNetworks(QStringList lst);
protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ConfDialog *m_ui;

private slots:

};

#endif // CONFDIALOG_H
