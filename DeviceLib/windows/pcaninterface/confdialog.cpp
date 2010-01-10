#include "confdialog.h"
#include "ui_confdialog.h"

#include <windows.h>
#include "Pcan_usb.h"

#include <stdio.h>
#include <stdlib.h>


ConfDialog::ConfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfDialog)
{
    MsgType = 0;
    BTR0BTR1 = 0;
    ui->setupUi(this);
}

ConfDialog::~ConfDialog()
{
    delete ui;
}

void ConfDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ConfDialog::on_checkBoxkbit_toggled(bool checked)
{
    if(!checked)
        ui->checkBoxBTR0BTR1->setEnabled(true);
    else
        ui->checkBoxBTR0BTR1->setEnabled(false);

}

void ConfDialog::on_checkBoxBTR0BTR1_toggled(bool checked)
{
    if(!checked)
        ui->checkBoxkbit->setEnabled(true);
    else
        ui->checkBoxkbit->setEnabled(false);
}

void ConfDialog::on_buttonBox_accepted()
{
    if(ui->checkBoxkbit->isChecked())
    {
        if(ui->comboBox->currentText() == QString("1000"))
            BTR0BTR1 = CAN_BAUD_1M;
        if(ui->comboBox->currentText() == QString("500"))
            BTR0BTR1 = CAN_BAUD_500K;
        if(ui->comboBox->currentText() == QString("250"))
            BTR0BTR1 = CAN_BAUD_250K;
        if(ui->comboBox->currentText() == QString("125"))
            BTR0BTR1 = CAN_BAUD_125K;
        if(ui->comboBox->currentText() == QString("100"))
            BTR0BTR1 = CAN_BAUD_100K;
        if(ui->comboBox->currentText() == QString("50"))
            BTR0BTR1 = CAN_BAUD_50K;
        if(ui->comboBox->currentText() == QString("20"))
            BTR0BTR1 = CAN_BAUD_20K;
        if(ui->comboBox->currentText() == QString("10"))
            BTR0BTR1 = CAN_BAUD_10K;
    }
    else
    {
        BTR0BTR1 = ui->lineEditBTR0BTR1->text().toInt(NULL, 16);
    }

    MsgType = 0;

    if(ui->checkBoxExtended->isChecked())
        MsgType |= MSGTYPE_EXTENDED;

    if(ui->checkBoxRTR->isChecked())
        MsgType |= MSGTYPE_RTR;
    if(ui->checkBoxERROR->isChecked())
        MsgType |= MSGTYPE_STATUS;
}

void ConfDialog::getValues(int *Reg, int *MsgTypes)
{
    *Reg = BTR0BTR1;
    *MsgTypes = MsgType;
}

void ConfDialog::setValues(int Reg, int MsgTypes)
{
    char BTRStr[32];
    sprintf(BTRStr, "%x", Reg);
    BTR0BTR1 = Reg;
    MsgType = MsgTypes;
    ui->checkBoxBTR0BTR1->setChecked(true);
    ui->checkBoxkbit->setChecked(false);
    ui->checkBoxkbit->setEnabled(false);
    ui->lineEditBTR0BTR1->setText(QString(BTRStr));


    if(MsgType & MSGTYPE_EXTENDED)
        ui->checkBoxExtended->setChecked(true);

    if(MsgType & MSGTYPE_RTR)
        ui->checkBoxRTR->setChecked(true);

    if(MsgType & MSGTYPE_STATUS)
        ui->checkBoxERROR->setChecked(true);
}
