#include "aboutbox.h"
#include "ui_aboutbox.h"

AboutBox::AboutBox(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AboutBox)
{
    m_ui->setupUi(this);
    QString about("QCANObserver Version 0.3\nwritten by Netseal (c) 2009\nLicense GPL");
    m_ui->TextLabel->setText(about);
}

AboutBox::~AboutBox()
{
    delete m_ui;
}

void AboutBox::changeEvent(QEvent *e)
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
