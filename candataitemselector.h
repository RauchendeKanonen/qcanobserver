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
#include <QtGui/QDialog>

#include "cansignalcollection.h"

namespace Ui {
    class CANDataItemSelector;
}

class CANDataItemSelector : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(CANDataItemSelector)
public:
    explicit CANDataItemSelector(QWidget *parent = 0, CANSignalCollection *Collection = 0);
    virtual ~CANDataItemSelector();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::CANDataItemSelector *m_ui;
    CANSignalCollection *pCollection;
signals:
    void addItemToDraw(CANSignal*, QColor);
    void deleteItemToDraw(CANSignal*);
private slots:
    void on_DeleteItem_clicked();
    void on_AddItem_clicked();
    void on_pushButton_clicked();
    void on_ComboItemSelector_currentIndexChanged(int index);
    void on_ComboItemSelector_highlighted(int index);
};

#endif // CANDATAITEMSELECTOR_H
