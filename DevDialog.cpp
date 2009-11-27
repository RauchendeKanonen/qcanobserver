#include "DevDialog.h"
#include "ui_DevDialog.h"
#include "candevice.h"


DevDialog::DevDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::DevDialog)
{
    m_ui->setupUi(this);
}

DevDialog::~DevDialog()
{
    delete m_ui;
}

void DevDialog::changeEvent(QEvent *e)
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

void DevDialog::on_buttonBox1_accepted()
{
    emit setDev(m_ui->lineEdit->text(), m_ui->lEBaudRate->text().toInt(NULL,16), MSGTYPE_STANDARD);
}
