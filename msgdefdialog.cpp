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
#include "msgdefdialog.h"
#include "ui_msgdefdialog.h"

MsgDefDialog::MsgDefDialog(QWidget *parent, QString ID) :
    QDialog(parent),
    m_ui(new Ui::MsgDefDialog)
{
    m_ui->setupUi(this);
    m_ui->IDlineEdit->setText(ID);

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
    QString PeriodTime;
    QString Trig("false");
    QString TrigId;

    QString Byte0 = m_ui->Byte0->text();
    QString Byte1 = m_ui->Byte1->text();
    QString Byte2 = m_ui->Byte2->text();
    QString Byte3 = m_ui->Byte3->text();
    QString Byte4 = m_ui->Byte4->text();
    QString Byte5 = m_ui->Byte5->text();
    QString Byte6 = m_ui->Byte6->text();
    QString Byte7 = m_ui->Byte7->text();

    if(m_ui->TriggerId->isEnabled())
        Trig = QString("true");

    TrigId = m_ui->TriggerId->text();
    QString TrByte0 = m_ui->Byte0_2->text();
    QString TrByte1 = m_ui->Byte1_2->text();
    QString TrByte2 = m_ui->Byte2_2->text();
    QString TrByte3 = m_ui->Byte3_2->text();
    QString TrByte4 = m_ui->Byte4_2->text();
    QString TrByte5 = m_ui->Byte5_2->text();
    QString TrByte6 = m_ui->Byte6_2->text();
    QString TrByte7 = m_ui->Byte7_2->text();




    PeriodTime = m_ui->period->text();


    emit newMsgAccepted(ID,
                        Byte0, Byte1, Byte2, Byte3, Byte4, Byte5, Byte6, Byte7,
                        PeriodTime,
                        Trig,
                        TrigId,
                        TrByte0, TrByte1, TrByte2, TrByte3, TrByte4, TrByte5, TrByte6, TrByte7);
}
//periodic
void MsgDefDialog::on_checkBox_toggled(bool checked)
{
    if(checked)
    {
        m_ui->checkBoxTrigger->setEnabled(false);
        m_ui->checkBoxTrigger->setChecked(false);
        m_ui->period->setEnabled(true);
    }

    else
    {
        m_ui->checkBoxTrigger->setEnabled(true);
        m_ui->checkBoxTrigger->setChecked(true);
        m_ui->period->setEnabled(false);
    }

}

void MsgDefDialog::on_checkBoxTrigger_toggled(bool checked)
{
    if(checked)
    {
        m_ui->checkBox->setEnabled(false);
        m_ui->checkBox->setChecked(false);
        m_ui->TriggerId->setEnabled(true);
    }

    else
    {
        m_ui->checkBox->setEnabled(true);
        m_ui->checkBox->setChecked(true);
        m_ui->TriggerId->setEnabled(false);
    }

}
