#ifndef DEBUGTERMINAL_H
#define DEBUGTERMINAL_H

#include <QtGui/QDialog>
#include "cansignalcollection.h"
#include "signalselectordialog.h"
#include "messagebufferinterface.h"
#include <QVector>
#include "qconsole.h"

namespace Ui {
    class DebugTerminal;
}

class DebugTerminal : public QDialog {
    Q_OBJECT
public:
    DebugTerminal(QWidget *parent=0, CANSignalCollection *Collection=0);
    ~DebugTerminal();
    ifstream& operator<<(ifstream& is);
    ofstream& operator>>(ofstream& os);
protected:
    void changeEvent(QEvent *e);

private:
    QList <CANSignal> Signals;
    QList <QColor>      Colors;
    Ui::DebugTerminal *m_ui;
    SignalSelectorDialog *Sel;
    QWidget *pparent;
    QConsole *Console;
public slots:
    void addItem(CANSignal* Signal, QColor Color);
    void deleteItem(CANSignal* Signal);
    void MainTimerSlot(void);
    void newMessage(_CANMsg , int);
    void ClearAll();
    void StopCapture();

private slots:
    void on_AddItem_clicked();
};

#endif // DEBUGTERMINAL_H
