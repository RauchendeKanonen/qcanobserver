#include "qdevdialog.h"
#include "ui_qdevdialog.h"

QDevDialog::QDevDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::QDevDialog)
{
    DevPath = "/dev/pcanusb0";
    m_ui->setupUi(this);
}

QDevDialog::~QDevDialog()
{
    delete m_ui;
}

void QDevDialog::changeEvent(QEvent *e)
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

void QDevDialog::on_buttonBox_accepted()
{
    emit setDev(DevPath);
}

void QDevDialog::on_lineEdit_textEdited(QString Path)
{
    DevPath = Path;
}
