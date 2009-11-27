#include "errordialog.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ErrorDialog)
{
    m_ui->setupUi(this);
}

ErrorDialog::~ErrorDialog()
{
    delete m_ui;
}

void ErrorDialog::SetErrorMessage(QString Message)
{
    m_ui->textEdit->setText(Message);
}

void ErrorDialog::changeEvent(QEvent *e)
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
