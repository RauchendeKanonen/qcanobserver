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

#include "observerdialog.h"
#include "ui_observerdialog.h"



ObserverDialog::ObserverDialog(QWidget *parent, CANSignalCollection *Collection) :
    QDialog(parent),
    m_ui(new Ui::ObserverDialog)
{
    pCollection = Collection;

    m_ui->setupUi(this);

    QStringList *list = new QStringList();
    list->append(QString("Name"));
    list->append(QString("Value"));
    list->append(QString("Unit"));
    TraceModel = new StringListModel(list);



    m_ui->tableView->setModel(TraceModel);

    m_ui->tableView->setColumnWidth(0, 120);
    m_ui->tableView->setColumnWidth(1, 80);
    m_ui->tableView->setColumnWidth(2, 80);
    m_ui->tableView->verticalHeader()->setDefaultSectionSize(15);


    Sel = new CANDataItemSelector(NULL, pCollection);
    connect(Sel, SIGNAL(addItemToDraw(CANSignal*, QColor)), this, SLOT(addItemToObserve(CANSignal*, QColor)));
    connect(Sel, SIGNAL(deleteItemToDraw(CANSignal*)), this, SLOT(deleteItemToObserve(CANSignal*)));

}

ObserverDialog::~ObserverDialog()
{
    delete m_ui;
    delete Sel;
}


void ObserverDialog::newMessage(CANMsgandTimeStruct *Msg, int Cnt)
{
    int i;
    for(i = 0 ; CANItems.count() >  i ; i++)
    {
        if(Msg->CANMsg.ID == CANItems.at(i)->Signal->Id)
        {
            SignalDataCollection *DataCol = CANItems.at(i)->Signal->getSignalDataCollection(Msg->CANMsg.DATA);

            if(DataCol)
            {
                QString Name = DataCol->Name;
                QString Unit = DataCol->Unit;
                float Value = DataCol->Value;

                QModelIndex index1 = TraceModel->index(0, 0, QModelIndex());

                TraceModel->insertRows(0, 1, (const QModelIndex &)index1);
                //get the valid index
                index1 = TraceModel->index(0, 0, QModelIndex());
                QVariant Col0(Name);
                TraceModel->setData(index1,Col0,Qt::EditRole, CANItems.at(i)->Color);


                index1 = TraceModel->index(0, 1, QModelIndex());

                QVariant Col1(Value);
                TraceModel->setData(index1,Col1,Qt::EditRole,  CANItems.at(i)->Color);

                index1 = TraceModel->index(0, 2, QModelIndex());
                QVariant Col2(Unit);
                TraceModel->setData(index1,Col2,Qt::EditRole, CANItems.at(i)->Color);
            }
        }
    }
}

void ObserverDialog::MainTimerSlot()
{
    TraceModel->Update();
}

void ObserverDialog::ClearAll()
{
    delete TraceModel;
    QStringList *list = new QStringList();
    list->append(QString("Name"));
    list->append(QString("Value"));
    list->append(QString("Unit"));
    TraceModel = new StringListModel(list);
    m_ui->tableView->setModel(TraceModel);
}


//!SLOT that addes an Item to observe
//!Takes a CalRule (specific for an ID) and the invoked Rule
void ObserverDialog::addItemToObserve(CANSignal* Signal, QColor Color)
{
    CANItems.append(new ObserveItems(Signal, new QColor(Color)));
}
void ObserverDialog::deleteItemToObserve(CANSignal* Signal)
{
    for( int i = 0 ; i < CANItems.count() ; i ++ )
    {
        if(CANItems.at(i)->Signal == Signal)
            CANItems.removeAt(i);
    }
}

void ObserverDialog::changeEvent(QEvent *e)
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

void ObserverDialog::on_pushButton_clicked()
{
    Sel->show();
}
