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
