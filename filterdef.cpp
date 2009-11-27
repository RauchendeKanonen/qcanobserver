#include "filterdef.h"
#include "ui_filterdef.h"

Filterdef::Filterdef(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Filterdef)
{
    m_ui->setupUi(this);
}

Filterdef::~Filterdef()
{
    delete m_ui;
}

void Filterdef::changeEvent(QEvent *e)
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

void Filterdef::on_buttonBox_accepted()
{
    emit acceptedFilter(m_ui->fromID->text().toInt(NULL,16), m_ui->toID->text().toInt(NULL,16));
}
