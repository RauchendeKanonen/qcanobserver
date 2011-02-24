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
