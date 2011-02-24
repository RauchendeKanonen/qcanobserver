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
#include "sendmsgdialog.h"
#include "ui_sendmsgdialog.h"



SendMsgDialog::SendMsgDialog(QWidget *parent, CANSignalCollection *Collection ) :
        QDialog(parent),
        m_ui(new Ui::SendMsgDialog)
{
    m_ui->setupUi(this);

    pCollection = Collection;

    black = QColor(Qt::black);
    red = QColor(Qt::red);
    green = QColor(Qt::green);

    Sel = NULL;

    if(pCollection)
        Sel = new QList <SignalSelectorDialog*>;

    QStringList *list = new QStringList();
    list->append(QString("ID"));
    list->append(QString("D0"));
    list->append(QString("D1"));
    list->append(QString("D2"));
    list->append(QString("D3"));
    list->append(QString("D4"));
    list->append(QString("D5"));
    list->append(QString("D6"));
    list->append(QString("D7"));
    list->append(QString("Period"));
    list->append(QString("Type"));
    MsgModel = new StringListModel(list);




    m_ui->MsgtableView->setModel(MsgModel);
    m_ui->MsgtableView->verticalHeader()->setDefaultSectionSize(15);
    m_ui->MsgtableView->setColumnWidth(0, 70);
    m_ui->MsgtableView->setColumnWidth(1, 45);
    m_ui->MsgtableView->setColumnWidth(2, 45);
    m_ui->MsgtableView->setColumnWidth(3, 45);
    m_ui->MsgtableView->setColumnWidth(4, 45);
    m_ui->MsgtableView->setColumnWidth(5, 45);
    m_ui->MsgtableView->setColumnWidth(6, 45);
    m_ui->MsgtableView->setColumnWidth(7, 45);
    m_ui->MsgtableView->setColumnWidth(8, 45);
    SelectedModelIndex = QModelIndex();
    delete list;
}

SendMsgDialog::~SendMsgDialog()
{
    delete MsgModel;
    delete m_ui;
}


void SendMsgDialog::changeEvent(QEvent *e)
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

//from msgdefdialog



void SendMsgDialog::newMsgAccepted(QString ID,
                                   QString Byte0, QString Byte1, QString Byte2, QString Byte3,
                                   QString Byte4, QString Byte5, QString Byte6, QString Byte7,
                                   QString PeriodTime,
                                   QString Trig,
                                   QString TrigId,
                                   QString TrByte0, QString TrByte1, QString TrByte2, QString TrByte3, QString TrByte4, QString TrByte5, QString TrByte6, QString TrByte7)
{




    if(PeriodTime.toInt(NULL,16) != 0)
    {
        MsgModel->insertRows(0, 1, QModelIndex());
        MsgModel->setData(MsgModel->index(0, 0, QModelIndex()), ID, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 1, QModelIndex()), Byte0, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 2, QModelIndex()), Byte1, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 3, QModelIndex()), Byte2, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 4, QModelIndex()), Byte3, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 5, QModelIndex()), Byte4, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 6, QModelIndex()), Byte5, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 7, QModelIndex()), Byte6, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 8, QModelIndex()), Byte7, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 9, QModelIndex()), PeriodTime, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 10, QModelIndex()), QString("Periodic"), Qt::EditRole, &black);

        return;
    }

    if(Trig == QString("true"))
    {
        MsgModel->insertRows(0, 1, QModelIndex());
        MsgModel->setData(MsgModel->index(0, 0, QModelIndex()), TrigId, Qt::EditRole, &green);
        MsgModel->setData(MsgModel->index(0, 1, QModelIndex()), TrByte0, Qt::EditRole, &green);
        MsgModel->setData(MsgModel->index(0, 2, QModelIndex()), TrByte1, Qt::EditRole, &green);
        MsgModel->setData(MsgModel->index(0, 3, QModelIndex()), TrByte2, Qt::EditRole, &green);
        MsgModel->setData(MsgModel->index(0, 4, QModelIndex()), TrByte3, Qt::EditRole, &green);
        MsgModel->setData(MsgModel->index(0, 5, QModelIndex()), TrByte4, Qt::EditRole, &green);
        MsgModel->setData(MsgModel->index(0, 6, QModelIndex()), TrByte5, Qt::EditRole, &green);
        MsgModel->setData(MsgModel->index(0, 7, QModelIndex()), TrByte6, Qt::EditRole, &green);
        MsgModel->setData(MsgModel->index(0, 8, QModelIndex()), TrByte7, Qt::EditRole, &green);
        MsgModel->setData(MsgModel->index(0, 10, QModelIndex()), QString("Trig Data"), Qt::EditRole, &black);


        MsgModel->insertRows(0, 1, QModelIndex());
        MsgModel->setData(MsgModel->index(0, 0, QModelIndex()), ID, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 1, QModelIndex()), Byte0, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 2, QModelIndex()), Byte1, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 3, QModelIndex()), Byte2, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 4, QModelIndex()), Byte3, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 5, QModelIndex()), Byte4, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 6, QModelIndex()), Byte5, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 7, QModelIndex()), Byte6, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 8, QModelIndex()), Byte7, Qt::EditRole, &black);
        MsgModel->setData(MsgModel->index(0, 10, QModelIndex()), QString("Trigger"), Qt::EditRole, &black);

        return;
    }


    MsgModel->insertRows(0, 1, QModelIndex());
    MsgModel->setData(MsgModel->index(0, 0, QModelIndex()), ID, Qt::EditRole, &black);
    MsgModel->setData(MsgModel->index(0, 1, QModelIndex()), Byte0, Qt::EditRole, &black);
    MsgModel->setData(MsgModel->index(0, 2, QModelIndex()), Byte1, Qt::EditRole, &black);
    MsgModel->setData(MsgModel->index(0, 3, QModelIndex()), Byte2, Qt::EditRole, &black);
    MsgModel->setData(MsgModel->index(0, 4, QModelIndex()), Byte3, Qt::EditRole, &black);
    MsgModel->setData(MsgModel->index(0, 5, QModelIndex()), Byte4, Qt::EditRole, &black);
    MsgModel->setData(MsgModel->index(0, 6, QModelIndex()), Byte5, Qt::EditRole, &black);
    MsgModel->setData(MsgModel->index(0, 7, QModelIndex()), Byte6, Qt::EditRole, &black);
    MsgModel->setData(MsgModel->index(0, 8, QModelIndex()), Byte7, Qt::EditRole, &black);
    MsgModel->setData(MsgModel->index(0, 10, QModelIndex()), QString("Single"), Qt::EditRole, &black);

    return;
}






void SendMsgDialog::on_SendButton_clicked()
{

    _CANMsg Msg;
    QModelIndex idx = MsgModel->index(SelectedMsg.row(), 0, QModelIndex());
    Msg.ID = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 1, QModelIndex());
    Msg.DATA[0] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 2, QModelIndex());
    Msg.DATA[1] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 3, QModelIndex());
    Msg.DATA[2] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 4, QModelIndex());
    Msg.DATA[3] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 5, QModelIndex());
    Msg.DATA[4] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 6, QModelIndex());
    Msg.DATA[5] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 7, QModelIndex());
    Msg.DATA[6] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 8, QModelIndex());
    Msg.DATA[7] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    Msg.LEN = 8;
    Msg.MSGTYPE = MSGTYPE_STANDARD;

    //period
    idx = MsgModel->index(SelectedMsg.row(), 9, QModelIndex());
    int Period = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 10);

    int Type=0;

    Type = SEND_SINGLE;

    if(Period)
        Type = SEND_PERIODIC;


    emit sendCANMsg(Msg, Period, Msg, Type);

    idx = MsgModel->index(SelectedMsg.row(), 0, QModelIndex());
    if(idx.row() != -1)
        m_ui->MsgtableView->selectRow(idx.row());

    if(Type == SEND_PERIODIC || Type == SEND_TRIG)
    {
        MsgModel->setflags(idx,MsgModel->flags(idx) & ~Qt::ItemIsEditable);
        MsgModel->setColor(idx,&red);
    }


}


void SendMsgDialog::on_SendStopButton_clicked()
{
        _CANMsg Msg;
    QModelIndex idx = MsgModel->index(SelectedMsg.row(), 0, QModelIndex());
    Msg.ID = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 1, QModelIndex());
    Msg.DATA[0] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 2, QModelIndex());
    Msg.DATA[1] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 3, QModelIndex());
    Msg.DATA[2] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 4, QModelIndex());
    Msg.DATA[3] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 5, QModelIndex());
    Msg.DATA[4] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 6, QModelIndex());
    Msg.DATA[5] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 7, QModelIndex());
    Msg.DATA[6] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    idx = MsgModel->index(SelectedMsg.row(), 8, QModelIndex());
    Msg.DATA[7] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

    Msg.LEN = 8;
    Msg.MSGTYPE = MSGTYPE_STANDARD;

    //period
    idx = MsgModel->index(SelectedMsg.row(), 9, QModelIndex());
    int Period = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 10);

    int Type=0;

    Type = SEND_SINGLE;

    if(Period)
        Type = SEND_PERIODIC;

    emit deleteCANMsg( Msg, Period, Msg, Type);

    idx = MsgModel->index(SelectedMsg.row(), 0, QModelIndex());
    if(idx.row() != -1)
        m_ui->MsgtableView->selectRow(idx.row());

    if(Type == SEND_PERIODIC || Type == SEND_TRIG)
    {
        MsgModel->setflags(idx,MsgModel->flags(idx) | Qt::ItemIsEditable);
        MsgModel->setColor(idx,&black);
    }
}

void SendMsgDialog::on_NewIdButton_clicked()
{
    //derive from selected id
    if((SelectedModelIndex.row() != -1) && (SelectedModelIndex.column() != -1))
    {
        QModelIndex SelectedParent = MsgModel->index(SelectedModelIndex.row(), SelectedModelIndex.column(), QModelIndex());
        QVariant var = MsgModel->data(SelectedParent, Qt::DisplayRole);
        SMsg = new MsgDefDialog(this, var.toString());

        connect(SMsg, SIGNAL(newMsgAccepted(QString,
                        QString, QString , QString , QString,
                        QString, QString , QString , QString,
                        QString,
                        QString,
                        QString,
                        QString, QString , QString , QString ,
                        QString, QString , QString , QString)),
                this, SLOT(newMsgAccepted(QString,
                        QString, QString , QString , QString,
                        QString, QString , QString , QString,
                        QString,
                        QString,
                        QString,
                        QString, QString , QString , QString ,
                        QString, QString , QString , QString)));

        SMsg->show();
        return;
    }


    SMsg = new MsgDefDialog(this, QString(""));


    connect(SMsg, SIGNAL(newMsgAccepted(QString,
                        QString, QString , QString , QString,
                        QString, QString , QString , QString,
                        QString,
                        QString,
                        QString,
                        QString, QString , QString , QString ,
                        QString, QString , QString , QString)),
            this, SLOT(newMsgAccepted(QString,
                        QString, QString , QString , QString,
                        QString, QString , QString , QString,
                        QString,
                        QString,
                        QString,
                        QString, QString , QString , QString ,
                        QString, QString , QString , QString)));


    connect(m_ui->MsgtableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const
                                                                          QItemSelection &)),
            this, SLOT(selectionChanged( const QItemSelection &, const QItemSelection &)));


    SMsg->show();
}

void SendMsgDialog::on_DeleteIdButton_clicked()
{
    on_SendStopButton_clicked();

    //index to remove
    QModelIndex idx = MsgModel->index(SelectedMsg.row(), 0, QModelIndex());
    QModelIndex root = MsgModel->index(0,0,QModelIndex());
    MsgModel->removeRows(idx.row(), 1, root);

}
void SendMsgDialog::closeEvent(QCloseEvent *e)
{
    e->ignore();
}

void SendMsgDialog::selectionChanged(const QItemSelection &current, const QItemSelection &parent)
{
    if(current.indexes().at(0).row() != -1)
        SelectedMsg = current.indexes().at(0);
}

void SendMsgDialog::on_MsgtableView_activated(QModelIndex index)
{
    QModelIndex idx = MsgModel->index(SelectedMsg.row(), 0, QModelIndex());
    if(idx.row() != -1)
        m_ui->MsgtableView->selectRow(idx.row());

}
void SendMsgDialog::on_MsgtableView_clicked(QModelIndex index)
{
    QModelIndex idx = MsgModel->index(SelectedMsg.row(), 0, QModelIndex());
    if(idx.row() != -1)
        m_ui->MsgtableView->selectRow(idx.row());
}

//! send the msg
void SendMsgDialog::on_MsgtableView_doubleClicked(QModelIndex index)
{
    QModelIndex idx = MsgModel->index(SelectedMsg.row(), 0, QModelIndex());
    if(idx.row() != -1)
        m_ui->MsgtableView->selectRow(idx.row());
}

void SendMsgDialog::on_MsgtableView_pressed(QModelIndex index)
{
    QModelIndex idx = MsgModel->index(SelectedMsg.row(), 0, QModelIndex());
    if(idx.row() != -1)
        m_ui->MsgtableView->selectRow(idx.row());
}

void SendMsgDialog::on_fromDbButton_clicked()
{
    Sel->append(new SignalSelectorDialog(this, pCollection));
    Sel->last()->setModal(true);
    if(Sel->last()->exec())
    {
        //Sel->last()->


    }
}

ofstream& SendMsgDialog::operator>>(ofstream& os)
{

    return os;
}
ifstream& SendMsgDialog::operator<<(ifstream& is)
{



    return is;
}
