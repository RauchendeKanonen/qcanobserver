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
#ifndef CANDATAITEMSELECTOR_H
#define CANDATAITEMSELECTOR_H
#include "processdatabase.h"
#include "errordialog.h"

#include <QtGui/QDialog>
#include <iostream>
#include <fstream>


#include "cansignalcollection.h"

using namespace std;

#define SELECTORMODE 1
#define SINGLEITEM   2

namespace Ui {
    class SignalSelectorDialog;
}

class SignalSelectorDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SignalSelectorDialog)
        public:
            explicit SignalSelectorDialog(QWidget *parent = 0, CANSignalCollection *Collection = 0);
    virtual ~SignalSelectorDialog();
    ofstream& operator>>(ofstream& os);
    ifstream&  operator<<(ifstream& is);

public:
    void switchMode(int Mode);
    QList <CANSignal*> getSelectedSignals(void);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SignalSelectorDialog *m_ui;
    CANSignalCollection *pCollection;
signals:
    void addItemToDraw(CANSignal*, QColor);
    void deleteItemToDraw(CANSignal*);
    void saveSignalToFile(QString, CANSignal*);

private slots:
    void on_pushButtonSaveSignal_clicked();
    void on_DeleteItem_clicked();
    void on_AddItem_clicked();
    void on_ComboItemSelector_currentIndexChanged(int index);
    void on_ComboItemSelector_highlighted(int index);
};

#endif // CANDATAITEMSELECTOR_H
