#include "msgdefdialog.h"
#include "ui_msgdefdialog.h"

MsgDefDialog::MsgDefDialog(QWidget *parent, QString *ID) :
    QDialog(parent),
    m_ui(new Ui::MsgDefDialog)
{
    m_ui->setupUi(this);
    m_ui->IDlineEdit->setText(*ID);

    m_ui->Byte0->setText(QString("0x00"));
    m_ui->Byte1->setText(QString("0x00"));
    m_ui->Byte2->setText(QString("0x00"));
    m_ui->Byte3->setText(QString("0x00"));
    m_ui->Byte4->setText(QString("0x00"));
    m_ui->Byte5->setText(QString("0x00"));
    m_ui->Byte6->setText(QString("0x00"));
    m_ui->Byte7->setText(QString("0x00"));
}

MsgDefDialog::~MsgDefDialog()
{
    delete m_ui;
}

void MsgDefDialog::changeEvent(QEvent *e)
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

void MsgDefDialog::on_buttonBox_accepted()
{
    QString ID = m_ui->IDlineEdit->text();

    QString Byte0 = m_ui->Byte0->text();
    QString Byte1 = m_ui->Byte1->text();
    QString Byte2 = m_ui->Byte2->text();
    QString Byte3 = m_ui->Byte3->text();
    QString Byte4 = m_ui->Byte4->text();
    QString Byte5 = m_ui->Byte5->text();
    QString Byte6 = m_ui->Byte6->text();
    QString Byte7 = m_ui->Byte7->text();

    emit newMsgAccepted(ID, Byte0, Byte1, Byte2, Byte3, Byte4, Byte5, Byte6, Byte7);
}
