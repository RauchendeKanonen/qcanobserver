#include "confdialog.h"
#include "ui_confdialog.h"

ConfDialog::ConfDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ConfDialog)
{
    m_ui->setupUi(this);
}

ConfDialog::~ConfDialog()
{
    delete m_ui;
}

void ConfDialog::changeEvent(QEvent *e)
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

void ConfDialog::getValues(QString *DevName, bool *logErrFrs, bool *LoopOwnMsgs)
{
    if(m_ui->checkBoxCostumDevName->isChecked())
        *DevName = m_ui->CostumDevNamelineEdit->text();
    else
        *DevName = m_ui->comboBoxNetDev->currentText();

    if(m_ui->checkBoxloopOwnMsgs->isChecked())
        *LoopOwnMsgs = true;
    else
        *LoopOwnMsgs = false;

    if(m_ui->checkBoxRecErrorFrs->isChecked())
        *logErrFrs = true;
    else
        *logErrFrs = false;
}

int ConfDialog::setValues(QString DevName, bool logErrFrs, bool LoopOwnMsgs)
{
    m_ui->checkBoxloopOwnMsgs->setChecked(LoopOwnMsgs);
    m_ui->checkBoxRecErrorFrs->setChecked(logErrFrs);

    int idx = m_ui->comboBoxNetDev->findText(DevName);

    if(idx == -1)
    {
        m_ui->CostumDevNamelineEdit->setEnabled(true);
        m_ui->checkBoxCostumDevName->setChecked(true);
        m_ui->CostumDevNamelineEdit->setText(DevName);
    }

    else
        m_ui->comboBoxNetDev->setCurrentIndex(idx);

    return 1;
}

void ConfDialog::SetNetworks(QStringList lst)
{
    m_ui->comboBoxNetDev->clear();
    for(int i = 0; lst.count() > i ; i ++ )
        m_ui->comboBoxNetDev->addItem(lst.at(i));
}


void ConfDialog::on_checkBoxCostumDevName_toggled(bool checked)
{
    if(checked)
        m_ui->CostumDevNamelineEdit->setEnabled(true);

    else
        m_ui->CostumDevNamelineEdit->setEnabled(false);
}