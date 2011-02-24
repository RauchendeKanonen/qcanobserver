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
#include "specialeventdialog.h"
#include "ui_specialeventdialog.h"

SpecialEventDialog::SpecialEventDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SpecialEventDialog)
{
    pparent = parent;

    m_ui->setupUi(this);

    QStringList *list = new QStringList();
    list->append(QString("Id"));
    list->append(QString("Class"));
    list->append(QString("Status"));

    TraceModel = new StringListModel(list);
    delete list;

    black = QColor(Qt::black);

    m_ui->tableView->setModel(TraceModel);

    m_ui->tableView->setColumnWidth(0, 50);
    m_ui->tableView->setColumnWidth(1, 80);
    m_ui->tableView->setColumnWidth(1, 80);
    m_ui->tableView->verticalHeader()->setDefaultSectionSize(15);
}

void SpecialEventDialog::ClearAll()
{
    delete TraceModel;
    QStringList *list = new QStringList();
    list->append(QString("Name"));
    list->append(QString("Value"));
    list->append(QString("Unit"));

    TraceModel = new StringListModel(list);
    delete list;
    m_ui->tableView->setModel(TraceModel);
}

void SpecialEventDialog::newSpecialMessage(_CANMsg CANMsg)
{
#ifdef LINUX
    QString IDString;
    IDString.sprintf("0x%04x", CANMsg.ID);

    QModelIndex IDIndex = TraceModel->index(0, 0, QModelIndex());
    TraceModel->insertRows(0, 1, (const QModelIndex &)IDIndex);

    IDIndex = TraceModel->index(0, 0, QModelIndex());
    QVariant IDCol(IDString);
    TraceModel->setData(IDIndex,IDCol,Qt::EditRole, &black);


    QModelIndex ClassIndex = TraceModel->index(0, 1, QModelIndex());
    QModelIndex StatusIndex = TraceModel->index(0, 2, QModelIndex());
    //Extracting information about the error
    //Statuserror is saved in the id if it is the id of an errorframe

    if(CANMsg.ID & CAN_ERR_TX_TIMEOUT)
    {
        QVariant ClassCol(QString("TX Timeout"));
        TraceModel->setData(ClassIndex,ClassCol,Qt::EditRole, &black);
    }

    if(CANMsg.ID & CAN_ERR_LOSTARB)
    {
        QVariant ClassCol(QString("Lost Arbitration"));
        TraceModel->setData(ClassIndex,ClassCol,Qt::EditRole, &black);
    }

    if(CANMsg.ID & CAN_ERR_CRTL)
    {
        QVariant ClassCol(QString("Controller Error"));
        TraceModel->setData(ClassIndex,ClassCol,Qt::EditRole, &black);

        switch(CANMsg.DATA[1])
        {
        case CAN_ERR_CRTL_UNSPEC:
            TraceModel->setData(StatusIndex,QVariant (QString("Unspecified")),Qt::EditRole, &black);
        break;
        case CAN_ERR_CRTL_RX_OVERFLOW:
            TraceModel->setData(StatusIndex,QVariant(QString("RX Overflow")),Qt::EditRole, &black);
        break;
        case CAN_ERR_CRTL_TX_OVERFLOW:
            TraceModel->setData(StatusIndex,QVariant (QString("TX Overflow")),Qt::EditRole, &black);
        break;
        case CAN_ERR_CRTL_RX_WARNING:
            TraceModel->setData(StatusIndex,QVariant (QString("RX Warning Level reached")),Qt::EditRole, &black);
        break;
        case CAN_ERR_CRTL_TX_WARNING:
            TraceModel->setData(StatusIndex,QVariant (QString("TX Warning Level reached")),Qt::EditRole, &black);
        break;
        case CAN_ERR_CRTL_RX_PASSIVE:
            TraceModel->setData(StatusIndex,QVariant (QString("reached RX passive Status")),Qt::EditRole, &black);
        break;
        case CAN_ERR_CRTL_TX_PASSIVE:
            TraceModel->setData(StatusIndex,QVariant (QString("reached TX passive Status")),Qt::EditRole, &black);
        break;
        }

    }

    if(CANMsg.ID & CAN_ERR_PROT)
    {
        QVariant ClassCol(QString("Protocol Violation"));
        TraceModel->setData(ClassIndex,ClassCol,Qt::EditRole, &black);

        switch(CANMsg.DATA[2])
        {
        case CAN_ERR_PROT_UNSPEC:
            TraceModel->setData(StatusIndex,QVariant (QString("Unspecified")),Qt::EditRole, &black);
        break;
        case CAN_ERR_PROT_BIT:
            TraceModel->setData(StatusIndex,QVariant(QString("single bit error")),Qt::EditRole, &black);
        break;
        case CAN_ERR_PROT_FORM:
            TraceModel->setData(StatusIndex,QVariant (QString("frame format error")),Qt::EditRole, &black);
        break;
        case CAN_ERR_PROT_STUFF:
            TraceModel->setData(StatusIndex,QVariant (QString("bit stuffing error")),Qt::EditRole, &black);
        break;
        case CAN_ERR_PROT_BIT0:
            TraceModel->setData(StatusIndex,QVariant (QString("unable to send dominant bit")),Qt::EditRole, &black);
        break;
        case CAN_ERR_PROT_BIT1:
            TraceModel->setData(StatusIndex,QVariant (QString("unable to send recessive bit")),Qt::EditRole, &black);
        break;
        case CAN_ERR_PROT_OVERLOAD:
            TraceModel->setData(StatusIndex,QVariant (QString("bus overload")),Qt::EditRole, &black);
        break;
        case CAN_ERR_PROT_ACTIVE:
            TraceModel->setData(StatusIndex,QVariant (QString("active error announcement")),Qt::EditRole, &black);
        break;
        case CAN_ERR_PROT_TX:
            TraceModel->setData(StatusIndex,QVariant (QString("error occured on transmission")),Qt::EditRole, &black);
        break;
        }
    }

    if(CANMsg.ID & CAN_ERR_TRX)
    {
        QVariant ClassCol(QString("Transeiver Status"));
        TraceModel->setData(ClassIndex,ClassCol,Qt::EditRole, &black);
        /* error status of CAN-transceiver / data[4] */

        switch(CANMsg.DATA[4])
        {
        case CAN_ERR_PROT_UNSPEC:
            TraceModel->setData(StatusIndex,QVariant (QString("Unspecified")),Qt::EditRole, &black);
        break;
        case CAN_ERR_TRX_CANH_NO_WIRE:
            TraceModel->setData(StatusIndex,QVariant(QString("CANH no wire")),Qt::EditRole, &black);
        break;
        case CAN_ERR_TRX_CANH_SHORT_TO_BAT:
            TraceModel->setData(StatusIndex,QVariant (QString("CANH short to BAT")),Qt::EditRole, &black);
        break;
        case CAN_ERR_TRX_CANH_SHORT_TO_VCC:
            TraceModel->setData(StatusIndex,QVariant (QString("CANH short to VCC")),Qt::EditRole, &black);
        break;
        case CAN_ERR_TRX_CANH_SHORT_TO_GND:
            TraceModel->setData(StatusIndex,QVariant (QString("CANH short to GND")),Qt::EditRole, &black);
        break;
        case CAN_ERR_TRX_CANL_NO_WIRE:
            TraceModel->setData(StatusIndex,QVariant(QString("CANL no wire")),Qt::EditRole, &black);
        break;
        case CAN_ERR_TRX_CANL_SHORT_TO_BAT:
            TraceModel->setData(StatusIndex,QVariant (QString("CANL short to BAT")),Qt::EditRole, &black);
        break;
        case CAN_ERR_TRX_CANL_SHORT_TO_VCC:
            TraceModel->setData(StatusIndex,QVariant (QString("CANL short to VCC")),Qt::EditRole, &black);
        break;
        case CAN_ERR_TRX_CANL_SHORT_TO_GND:
            TraceModel->setData(StatusIndex,QVariant (QString("CANL short to GND")),Qt::EditRole, &black);
        break;
        case CAN_ERR_TRX_CANL_SHORT_TO_CANH:
            TraceModel->setData(StatusIndex,QVariant (QString("CANL short to CANH")),Qt::EditRole, &black);
        break;
        }
    }

    if(CANMsg.ID & CAN_ERR_ACK)
    {
        QVariant ClassCol(QString("Recived no ACK on TX"));
        TraceModel->setData(ClassIndex,ClassCol,Qt::EditRole, &black);
    }

    if(CANMsg.ID & CAN_ERR_BUSOFF)
    {
        QVariant ClassCol(QString("Bus off"));
        TraceModel->setData(ClassIndex,ClassCol,Qt::EditRole, &black);
    }

    if(CANMsg.ID & CAN_ERR_BUSERROR)
    {
        QVariant ClassCol(QString("Bus Error"));
        TraceModel->setData(ClassIndex,ClassCol,Qt::EditRole, &black);
    }

    if(CANMsg.ID & CAN_ERR_RESTARTED)
    {
        QVariant ClassCol(QString("Controller Restarted"));
        TraceModel->setData(ClassIndex,ClassCol,Qt::EditRole, &black);
    }
#endif
}

void SpecialEventDialog::MainTimerSlot()
{

}


SpecialEventDialog::~SpecialEventDialog()
{
    delete m_ui;
}

void SpecialEventDialog::changeEvent(QEvent *e)
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

void SpecialEventDialog::closeEvent(QCloseEvent *e)
{
    e->ignore();
}
