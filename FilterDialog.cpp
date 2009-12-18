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
#include "FilterDialog.h"
#include "ui_FilterDialog.h"

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::FilterDialog)
{
    m_ui->setupUi(this);
    m_ui->HWFilterText->setText(QwtText("Hardware\n Filter"));
    m_ui->DBFilterText->setText(QwtText("Database Output\n Filter"));
    m_ui->GraphicFilterText->setText(QwtText("Graphic Input\n Filter"));
    FilterDef = new Filterdef();

    QObject::connect(FilterDef, SIGNAL(acceptedFilter(int, int)),
                     this, SLOT(acceptedFilter(int, int)));

}


FilterDialog::~FilterDialog()
{
    delete m_ui;
    delete FilterDef;
}

void FilterDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//Slot
void FilterDialog::acceptedFilter(int from, int to)
{
    QString Str;
    Str.sprintf("0x%x - 0x%x", from, to);
    aktEditLst->addItem(Str);
    return;
}




void FilterDialog::on_AddInputFilter_clicked()
{
    aktEditLst = m_ui->lstInputFilter;
    FilterDef->setModal(true);
    FilterDef->exec();
}


void FilterDialog::on_AddDataBaseOutputFilter_clicked()
{
    aktEditLst = m_ui->lstDatabaseOutputFilter;
    FilterDef->setModal(true);
    FilterDef->exec();
}

void FilterDialog::on_AddGraphicInputFilter_clicked()
{
    aktEditLst = m_ui->lstGraphicInputFilter;
    FilterDef->setModal(true);
    FilterDef->exec();
}

void FilterDialog::on_DelInputFilter_clicked()
{
    qDeleteAll(m_ui->lstInputFilter->selectedItems());
}

void FilterDialog::on_DelDataVaseOutputFilter_clicked()
{
    qDeleteAll(m_ui->lstDatabaseOutputFilter->selectedItems());
}

void FilterDialog::on_DelGraphicInputFilter_clicked()
{
    qDeleteAll(m_ui->lstGraphicInputFilter->selectedItems());
}

//apply
void FilterDialog::on_pushButton_clicked()
{
    int cnt = m_ui->lstInputFilter->count();

    //delete all filters
    emit setFilter(HWFILTER, -1 , -1);

    if(cnt == 0)
    {
        emit setFilter(HWFILTER, 0 , 2048);
    }


    //re-add the remaining filters
    for( int i = 0 ; cnt > i ; i++ )
    {
        QVariant var = m_ui->lstInputFilter->item(i)->data(Qt::DisplayRole);
        QString Str = var.toString();

        int pos = Str.indexOf("-",0);

        QString FromStr = Str.left(pos-1);
        int from = FromStr.toInt(NULL, 16);

        QString ToStr = Str.right(Str.length() - pos -1);
        int to = ToStr.toInt(NULL, 16);

        emit setFilter(HWFILTER, from , to);
    }
}
