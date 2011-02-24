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
#ifndef SPECIALEVENTDIALOG_H
#define SPECIALEVENTDIALOG_H

#include <QtGui/QDialog>
#include "stringlistmodel.h"
#include "obscan.h"
#include <QCloseEvent>
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
    void closeEvent(QCloseEvent *e);
public slots:
    void newSpecialMessage(_CANMsg CANMsg);
    void MainTimerSlot();
    void ClearAll();
private:
    Ui::SpecialEventDialog *m_ui;
    StringListModel *TraceModel;
    QColor black;
    QWidget *pparent;
};

#endif // SPECIALEVENTDIALOG_H
