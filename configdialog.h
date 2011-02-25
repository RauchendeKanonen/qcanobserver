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
#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QtGui/QDialog>
#include <iostream>
#include <fstream>
using namespace std;
namespace Ui {
    class ConfigDialog;
}

typedef struct
{
    int MainMemByte;
    int GraphMemByte;
    int ObserverMemByte;
    int WriteToDisk;
}__config;

class ConfigDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ConfigDialog)
public:
            void informOthers(void);
    explicit ConfigDialog(QWidget *parent = 0);
    virtual ~ConfigDialog();
    ofstream& operator>>(ofstream& os);

    ifstream& operator<<(ifstream& is);

 signals:
         void configChanged(__config);


protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ConfigDialog *m_ui;
    void uiUpdate(void);
    int MainMemByte;
    int GraphMemByte;
    int ObserverMemByte;
    int WriteToDisk;

private slots:
    void on_buttonBox_accepted();
};

#endif // CONFIGDIALOG_H
