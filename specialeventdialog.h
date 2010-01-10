#ifndef SPECIALEVENTDIALOG_H
#define SPECIALEVENTDIALOG_H

#include <QtGui/QDialog>
#include "stringlistmodel.h"
#include "obscan.h"

#ifdef LINUX
#include<linux/can/error.h>
#endif
namespace Ui {
    class SpecialEventDialog;
}

class SpecialEventDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SpecialEventDialog)
public:
    explicit SpecialEventDialog(QWidget *parent = 0);
    virtual ~SpecialEventDialog();

protected:
    virtual void changeEvent(QEvent *e);

public slots:
    void newSpecialMessage(_CANMsg *CANMsg);
    void MainTimerSlot();

private:
    Ui::SpecialEventDialog *m_ui;
    StringListModel *TraceModel;
    QColor black;
};

#endif // SPECIALEVENTDIALOG_H
