#include "sendmsgdialog.h"
#include "ui_sendmsgdialog.h"



SendMsgDialog::SendMsgDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SendMsgDialog)
{
    m_ui->setupUi(this);

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
void SendMsgDialog::newMsgAccepted(QString ID, QString Byte0, QString Byte1, QString Byte2, QString Byte3
		   , QString Byte4, QString Byte5, QString Byte6, QString Byte7)
{
    MsgModel->insertRow(0, QModelIndex());

    MsgModel->setData(MsgModel->index(0, 0, QModelIndex()), ID, Qt::EditRole, new QColor(Qt::black));
    MsgModel->setData(MsgModel->index(0, 1, QModelIndex()), Byte0, Qt::EditRole, new QColor(Qt::black));
    MsgModel->setData(MsgModel->index(0, 2, QModelIndex()), Byte1, Qt::EditRole, new QColor(Qt::black));
    MsgModel->setData(MsgModel->index(0, 3, QModelIndex()), Byte2, Qt::EditRole, new QColor(Qt::black));
    MsgModel->setData(MsgModel->index(0, 4, QModelIndex()), Byte3, Qt::EditRole, new QColor(Qt::black));
    MsgModel->setData(MsgModel->index(0, 5, QModelIndex()), Byte4, Qt::EditRole, new QColor(Qt::black));
    MsgModel->setData(MsgModel->index(0, 6, QModelIndex()), Byte5, Qt::EditRole, new QColor(Qt::black));
    MsgModel->setData(MsgModel->index(0, 7, QModelIndex()), Byte6, Qt::EditRole, new QColor(Qt::black));
    MsgModel->setData(MsgModel->index(0, 8, QModelIndex()), Byte7, Qt::EditRole, new QColor(Qt::black));
    MsgModel->Update();
}

//new MSg
void SendMsgDialog::on_pushButton_3_clicked()
{
    //derive from selected id
    if((SelectedModelIndex.row() != -1) && (SelectedModelIndex.column() != -1))
    {
        QModelIndex SelectedParent = MsgModel->index(SelectedModelIndex.row(), SelectedModelIndex.column(), QModelIndex());
        QVariant var = MsgModel->data(SelectedParent, Qt::DisplayRole);
        SMsg = new MsgDefDialog(this, &var.toString());
        
        connect(SMsg, SIGNAL(newMsgAccepted(QString , QString , QString , QString , QString
                   , QString , QString , QString , QString )), 
                this, SLOT(newMsgAccepted(QString , QString , QString , QString , QString
                   , QString , QString , QString , QString )));
        
        SMsg->show();
        return;
    }


    SMsg = new MsgDefDialog(this, &QString(""));


    connect(SMsg, SIGNAL(newMsgAccepted(QString , QString , QString , QString , QString
                   , QString , QString , QString , QString )),
                this, SLOT(newMsgAccepted(QString , QString , QString , QString , QString
                   , QString , QString , QString , QString )));


    connect(m_ui->MsgtableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const
                                                                        QItemSelection &)),
                this, SLOT(selectionChanged( const QItemSelection &, const QItemSelection &)));


    SMsg->show();
}

void SendMsgDialog::on_MsgtableView_clicked(QModelIndex index)
{
    SelectedModelIndex = index;
}

//! send the msg
void SendMsgDialog::on_MsgtableView_doubleClicked(QModelIndex index)
{
    if(index.row() != -1)
        m_ui->MsgtableView->selectRow(index.row());

//m_ui->MsgtableView->


}

//Selection
void SendMsgDialog::on_MsgtableView_entered(QModelIndex index)
{

}

void SendMsgDialog::on_SendButton_clicked()
{
int f = SelectedMsgs.count();
    for(int s = 0 ; s < SelectedMsgs.count() ; s++)
    {

        TPCANMsg Msg;
	QModelIndex idx = MsgModel->index(SelectedMsgs.at(s).row(), 0, QModelIndex());
        Msg.ID = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

	idx = MsgModel->index(SelectedMsgs.at(s).row(), 1, QModelIndex());
        Msg.DATA[0] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

	idx = MsgModel->index(SelectedMsgs.at(s).row(), 2, QModelIndex());
        Msg.DATA[1] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

	idx = MsgModel->index(SelectedMsgs.at(s).row(), 3, QModelIndex());
        Msg.DATA[2] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

	idx = MsgModel->index(SelectedMsgs.at(s).row(), 4, QModelIndex());
        Msg.DATA[3] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

	idx = MsgModel->index(SelectedMsgs.at(s).row(), 5, QModelIndex());
        Msg.DATA[4] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

	idx = MsgModel->index(SelectedMsgs.at(s).row(), 6, QModelIndex());
        Msg.DATA[5] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

	idx = MsgModel->index(SelectedMsgs.at(s).row(), 7, QModelIndex());
        Msg.DATA[6] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

	idx = MsgModel->index(SelectedMsgs.at(s).row(), 8, QModelIndex());
        Msg.DATA[7] = MsgModel->data(idx, Qt::DisplayRole).toString().toInt(NULL, 16);

        emit sendCANMsg(&Msg);
    }
}

void SendMsgDialog::on_delete_2_clicked()
{
    MsgModel->removeRow(m_ui->MsgtableView->selectionModel()->currentIndex().row(),QModelIndex());
}

void SendMsgDialog::selectionChanged(const QItemSelection &current, const QItemSelection &parent)
{
    SelectedMsgs = current.indexes();
}
