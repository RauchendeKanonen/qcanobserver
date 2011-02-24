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
#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ConfigDialog)
{
    MainMemByte = 0;
    GraphMemByte = 0;
    ObserverMemByte = 0;
    WriteToDisk = 1;
    m_ui->setupUi(this);
    uiUpdate();
}

void ConfigDialog::uiUpdate(void)
{
    if(MainMemByte)
    {
        m_ui->checkBoxMainMem->setChecked(true);
        m_ui->doubleSpinBoxMainMem->setValue(MainMemByte);
    }

    else
    {
        m_ui->checkBoxMainMem->setChecked(false);
        m_ui->doubleSpinBoxMainMem->setValue(MainMemByte);
    }


    if(GraphMemByte)
    {
        m_ui->checkBoxMemGraphic->setChecked(true);
        m_ui->doubleSpinBoxGraphMem->setValue(GraphMemByte);
    }
    else
    {
        m_ui->checkBoxMemGraphic->setChecked(false);
        m_ui->doubleSpinBoxGraphMem->setValue(GraphMemByte);
    }

    if(ObserverMemByte)
    {
        m_ui->checkBoxObserverMem->setChecked(true);
        m_ui->doubleSpinBoxObserverMem->setValue(ObserverMemByte);
    }
    else
    {
        m_ui->checkBoxObserverMem->setChecked(false);
        m_ui->doubleSpinBoxObserverMem->setValue(ObserverMemByte);
    }

    if(WriteToDisk)
         m_ui->checkBoxWriteToDisk->setChecked(true);
    else
         m_ui->checkBoxWriteToDisk->setChecked(false);


}

ConfigDialog::~ConfigDialog()
{
    delete m_ui;
}

void ConfigDialog::changeEvent(QEvent *e)
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

void ConfigDialog::on_buttonBox_accepted()
{

    if(m_ui->checkBoxMainMem->isChecked())
    {
        MainMemByte = m_ui->doubleSpinBoxMainMem->value();
    }
    else
        MainMemByte = 0;

    if(m_ui->checkBoxMemGraphic->isChecked())
    {
        GraphMemByte = m_ui->doubleSpinBoxGraphMem->value();
    }
    else
        GraphMemByte = 0;

    if(m_ui->checkBoxObserverMem->isChecked())
    {
        ObserverMemByte = m_ui->doubleSpinBoxObserverMem->value();
    }
    else
        ObserverMemByte = 0;

    if(m_ui->checkBoxWriteToDisk->isChecked())
        WriteToDisk = 1;
    else
        WriteToDisk = 0;


    informOthers();
}

ofstream& ConfigDialog::operator>>(ofstream& os)
{
    char temp[512];
    memset(temp, 0, 512);
    memcpy(temp, &MainMemByte, sizeof(MainMemByte));
    memcpy(temp+sizeof(MainMemByte), &GraphMemByte, sizeof(MainMemByte));
    memcpy(temp+2*sizeof(MainMemByte), &ObserverMemByte, sizeof(MainMemByte));
    memcpy(temp+3*sizeof(WriteToDisk), &WriteToDisk, sizeof(WriteToDisk));

    for(int f = 0; f < 512 ; f++)
        os.put(temp[f]);

    return os;
}

ifstream& ConfigDialog::operator<<(ifstream& is)
{
    int g=0;
    char temp[512];
    memset(temp, 0, 512);
    for(int f = 0; f < 512 ; f++)
        is.get(temp[f]);

    memcpy(&MainMemByte, temp,sizeof(MainMemByte));
    memcpy(&GraphMemByte, temp+sizeof(MainMemByte), sizeof(MainMemByte));
    memcpy(&ObserverMemByte, temp+2*sizeof(MainMemByte), sizeof(MainMemByte));
    memcpy(&WriteToDisk, temp+3*sizeof(WriteToDisk), sizeof(MainMemByte));



    uiUpdate();
    informOthers();



    return is;
}



void ConfigDialog::informOthers(void)
{
    __config cfg;
    cfg.GraphMemByte = GraphMemByte;
    cfg.MainMemByte = MainMemByte;
    cfg.ObserverMemByte = ObserverMemByte;
    cfg.WriteToDisk = WriteToDisk;
    emit configChanged(cfg);
}
