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
#ifndef FILTERDEF_H
#define FILTERDEF_H

#include <QtGui/QDialog>

namespace Ui {
    class Filterdef;
}

class Filterdef : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(Filterdef)
public:
    explicit Filterdef(QWidget *parent = 0);
    virtual ~Filterdef();


protected:
    virtual void changeEvent(QEvent *e);



private:
    Ui::Filterdef *m_ui;

signals:
    void acceptedFilter(int from, int to);

private slots:
    void on_buttonBox_accepted();
};

#endif // FILTERDEF_H
