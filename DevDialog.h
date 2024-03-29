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
#ifndef DEVDIALOG_H
#define DEVDIALOG_H

#include <QtGui/QDialog>
#include <iostream>
#include <fstream>

using namespace std;

#define RTR_FR 1
#define ERROR_FR 2
#define TIMESTAMP 4



namespace Ui {
    class DevDialog;
}


class DevDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(DevDialog)
public:
    explicit DevDialog(QWidget *parent = 0);
    virtual ~DevDialog();
    ofstream& operator>>(ofstream& os);
    ifstream& operator<<(ifstream& is);

protected:
    virtual void changeEvent(QEvent *e);
    QString *CANLibFilePath;
    void *confBuffer;

private:
    Ui::DevDialog *m_ui;
QString *dbg;

signals:
    void setDev(void *ConfData, QString InterfaceLib, bool ShareDevInterfaceLib);

private slots:
    void on_comboBoxLibSelector_activated(int index);
    void on_comboBoxLibSelector_currentIndexChanged(int index);
    void on_buttonBox1_accepted();
};

#endif // DEVDIALOG_H
