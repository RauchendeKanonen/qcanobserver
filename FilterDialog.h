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
#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QtGui/QDialog>
#include "filterdef.h"
#include <QListWidget>

#define HWFILTER 1
#define BEHINDDBFILTER 2
#define GRAPHICWINFILTER 3


namespace Ui {
    class FilterDialog;
}

class FilterDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(FilterDialog)
public:
    explicit FilterDialog(QWidget *parent = 0);
    virtual ~FilterDialog();

protected:
    virtual void changeEvent(QEvent *e);

public slots:
    void acceptedFilter(int from, int to);

signals:
    void setFilter(int Place, int from, int to);

private:
    QListWidget *aktEditLst;
    Ui::FilterDialog *m_ui;
    Filterdef *FilterDef;
private slots:
    void on_pushButton_clicked();
    void on_DelGraphicInputFilter_clicked();
    void on_DelDataVaseOutputFilter_clicked();
    void on_AddGraphicInputFilter_clicked();
    void on_AddDataBaseOutputFilter_clicked();
    void on_DelInputFilter_clicked();
    void on_AddInputFilter_clicked();
};

#endif // FILTERDIALOG_H
