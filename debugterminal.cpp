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
#include "debugterminal.h"
#include "ui_debugterminal.h"
#include "qconsole.h"
#include "extrect.h"
DebugTerminal::DebugTerminal(QWidget *parent, CANSignalCollection *Collection) :
    QDialog(parent),
    m_ui(new Ui::DebugTerminal)
{
    Sel = new SignalSelectorDialog(NULL, Collection);
    connect(Sel, SIGNAL(addItemToDraw(CANSignal*, QColor)), this, SLOT(addItem(CANSignal*, QColor)));
    connect(Sel, SIGNAL(deleteItemToDraw(CANSignal*)), this, SLOT(deleteItem(CANSignal*)));
    Console = new QConsole();

    m_ui->setupUi(this);
    m_ui->verticalLayout->addWidget(Console);
    pparent = parent;
}

DebugTerminal::~DebugTerminal()
{
    delete m_ui;
}

void DebugTerminal::changeEvent(QEvent *e)
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

void DebugTerminal::on_AddItem_clicked()
{
    Sel->show();
}
void DebugTerminal::newMessage(_CANMsg CANMsg, int Cnt)
{
    if( m_ui->ConnectedCheckBox->isChecked())
    {
        for(int i = 0; i < Signals.count() ; i ++ )
        {
            if(Signals.at(i).Id == CANMsg.ID)
            {
                char *ptr;
                ptr = (char*)CANMsg.DATA;
                Console->insertText(ptr, Colors.at(i), 8);
                return;
            }
        }
    }
}

void DebugTerminal::MainTimerSlot()
{


}

void DebugTerminal::ClearAll()
{
    if( m_ui->ConnectedCheckBox->isChecked())
    {
        disconnect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));

        Console->clear();

        connect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));
    }
}


void DebugTerminal::StopCapture()
{

}

void DebugTerminal::addItem(CANSignal* Signal, QColor Color)
{
    Signals.append(*Signal);
    Colors.append(Color);
}

void DebugTerminal::deleteItem(CANSignal* Signal)
{
    /*int idx = Signals.indexOf(*Signal, 0);

    Signals.removeAt(idx);
    Colors.removeAt(idx);*/

}


ofstream& DebugTerminal::operator>>(ofstream& os)
{
    //layout
    ExtRect e;
    QRect   q = this->geometry();
    e = &q;
    e >> os;

    //connection
    char connectedstate = (char)m_ui->ConnectedCheckBox->isChecked();
    os.put(connectedstate);

    //selected signals
    (*Sel) >> os;
    return os;
}
ifstream& DebugTerminal::operator<<(ifstream& is)
{
    //layout
    ExtRect e;
    e << is;
    this->setGeometry(e);

    //connection
    char connectedstate;
    is >> connectedstate;
    m_ui->ConnectedCheckBox->setChecked((bool)connectedstate);

    //selected signals

    (*Sel) << is;
    return is;
}
