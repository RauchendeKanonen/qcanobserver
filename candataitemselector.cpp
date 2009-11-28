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
#include "candataitemselector.h"
#include "ui_candataitemselector.h"
#include "calrule.h"

CANDataItemSelector::CANDataItemSelector(QWidget *parent, QList<CalRule*> *RuleList) :
    QDialog(parent),
    m_ui(new Ui::CANDataItemSelector)
{

    pRuleList = RuleList;
    m_ui->setupUi(this);



    m_ui->ComboItemColorSelector->addItem(QString("red"),NULL);
    m_ui->ComboItemColorSelector->addItem(QString("blue"),NULL);
    m_ui->ComboItemColorSelector->addItem(QString("green"),NULL);
    m_ui->ComboItemColorSelector->addItem(QString("yellow"),NULL);
    m_ui->ComboItemColorSelector->addItem(QString("cyan"),NULL);
    m_ui->ComboItemColorSelector->addItem(QString("black"),NULL);
    m_ui->ComboItemColorSelector->addItem(QString("grey"),NULL);

    for(int i = 0 ; i < pRuleList->count() ; i++ )
    {
        for(int c = 0 ; c < pRuleList->at(i)->getNumOfRules() ; c ++ )
        {
            QString ID;
            ID.sprintf("<0x%x> (%d) ",(pRuleList->at(i)->getId()), c);

            m_ui->ComboItemSelector->addItem(ID+pRuleList->at(i)->getName(c),NULL);
        }


    }

}

CANDataItemSelector::~CANDataItemSelector()
{
    delete m_ui;
}

void CANDataItemSelector::changeEvent(QEvent *e)
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

void CANDataItemSelector::on_ComboItemSelector_highlighted(int index)
{

}

void CANDataItemSelector::on_ComboItemSelector_currentIndexChanged(int index)
{

}

void CANDataItemSelector::on_pushButton_clicked()
{

}

void CANDataItemSelector::on_AddItem_clicked()
{
    int index = m_ui->ComboItemColorSelector->currentIndex();
    QString ColorName = m_ui->ComboItemColorSelector->itemText(index);
    QColor Color(ColorName);


    index = m_ui->ComboItemSelector->currentIndex();
    QString RuleName = m_ui->ComboItemSelector->itemText(index); 

    QString IDStr = RuleName.right(-RuleName.indexOf(QString("<"))+RuleName.length()-1);
    IDStr = IDStr.left(IDStr.indexOf(QString(">")));


    QString RuleStr = RuleName.right(-RuleName.indexOf(QString('('))+RuleName.length()-1);
    RuleStr = RuleStr.left(RuleStr.indexOf(QString(')')));

    int id = IDStr.toInt(NULL, 16);
    int Rule = RuleStr.toInt(NULL, 10);

    //is the item already in the list
    for(int i = 0 ; i < m_ui->ItemsToDraw->count() ; i ++ )
    {
        if(m_ui->ItemsToDraw->item(i)->text() ==  RuleName)
            return;
    }

    m_ui->ItemsToDraw->addItem(RuleName);

    for(int i = 0; pRuleList->count() > i ; i ++ )
    {
        if(pRuleList->at(i)->getId() == id)
        {
            emit addItemToDraw(pRuleList->at(i), Rule, Color);
            return;
        }
    }
}
