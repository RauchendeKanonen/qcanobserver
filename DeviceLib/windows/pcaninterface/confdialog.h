#ifndef CONFDIALOG_H
#define CONFDIALOG_H

#include <QDialog>

namespace Ui {
    class ConfDialog;
}

class ConfDialog : public QDialog {
    Q_OBJECT
private:
    Ui::ConfDialog *ui;
    unsigned int BTR0BTR1;
    unsigned int MsgType;

public:
    ConfDialog(QWidget *parent = 0);
    ~ConfDialog();
    void getValues(int *Reg, int *MsgTypes);

    void setValues(int Reg, int MsgTypes);

protected:
    void changeEvent(QEvent *e);



private slots:
    void on_buttonBox_accepted();
    void on_checkBoxBTR0BTR1_toggled(bool checked);
    void on_checkBoxkbit_toggled(bool checked);
};

#endif // CONFDIALOG_H
