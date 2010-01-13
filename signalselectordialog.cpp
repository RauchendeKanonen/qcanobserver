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
#include "signalselectordialog.h"
#include "ui_signalselectordialog.h"
#include <QVariant>

SignalSelectorDialog::SignalSelectorDialog(QWidget *parent, CANSignalCollection *Collection) :
    QDialog(parent),
    m_ui(new Ui::SignalSelectorDialog)
{

    pCollection = Collection;
    m_ui->setupUi(this);



    m_ui->ComboItemColorSelector->addItem(QString("red"),QVariant(QColor("red")));

    m_ui->ComboItemColorSelector->addItem(QString("blue"),QVariant(QColor("blue")));
    m_ui->ComboItemColorSelector->addItem(QString("green"),QVariant(QColor("green")));
    m_ui->ComboItemColorSelector->addItem(QString("yellow"),QVariant(QColor("yellow")));
    m_ui->ComboItemColorSelector->addItem(QString("cyan"),QVariant(QColor("cyan")));
    m_ui->ComboItemColorSelector->addItem(QString("black"),QVariant(QColor("black")));
    m_ui->ComboItemColorSelector->addItem(QString("grey"),QVariant(QColor("grey")));

    for(int i = 0 ; i < pCollection->count() ; i++ )
    {
	QString ID;
	ID.sprintf("<0x%x>",Collection->getSignal(i)->Id);
	m_ui->ComboItemSelector->addItem(ID+Collection->getSignal(i)->Name,NULL);
    }

}

SignalSelectorDialog::~SignalSelectorDialog()
{
    delete m_ui;
}

void SignalSelectorDialog::changeEvent(QEvent *e)
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

void SignalSelectorDialog::on_ComboItemSelector_highlighted(int index)
{

}

void SignalSelectorDialog::on_ComboItemSelector_currentIndexChanged(int index)
{

}

void SignalSelectorDialog::on_pushButton_clicked()
{

}

void SignalSelectorDialog::on_AddItem_clicked()
{
    int index = m_ui->ComboItemColorSelector->currentIndex();
    QString ColorName = m_ui->ComboItemColorSelector->itemText(index);
    QColor Color(ColorName);

    index = m_ui->ComboItemSelector->currentIndex();
    QString ItemStr = m_ui->ComboItemSelector->itemText(index);

    QString IDStr = ItemStr.right(-ItemStr.indexOf(QString("<"))+ItemStr.length()-1);
    IDStr = IDStr.left(IDStr.indexOf(QString(">")));

    QString SignalName = ItemStr.right(-ItemStr.indexOf(QString("("))+ItemStr.length());

    SignalName = SignalName.left(SignalName.indexOf(QString(")"))+1);

    int id = IDStr.toInt(NULL, 16);


    //is the item already in the list
    for(int i = 0 ; i < m_ui->ItemsToDraw->count() ; i ++ )
    {
        if(m_ui->ItemsToDraw->item(i)->text() ==  ItemStr)
            return;
    }

    QListWidgetItem *Item = new QListWidgetItem(ItemStr, NULL, 0);
    Item->setData(Qt::ItemIsSelectable, QVariant(QColor(Color)));

    m_ui->ItemsToDraw->addItem(Item);//(ItemStr);

    //inform the parent about

    emit addItemToDraw(pCollection->getSignal(id, SignalName), Color);
    return;

}

void SignalSelectorDialog::on_DeleteItem_clicked()
{
    int row;
    if(m_ui->ItemsToDraw->selectionModel()->selection().count())
        row = m_ui->ItemsToDraw->selectionModel()->selectedRows(0).at(0).row();
    else
        return;

    QString ItemStr = m_ui->ItemsToDraw->item(row)->text();
    QString SignalName = ItemStr.right(-ItemStr.indexOf(QString("("))+ItemStr.length());

    QString IDStr = ItemStr.right(-ItemStr.indexOf(QString("<"))+ItemStr.length()-1);
    IDStr = IDStr.left(IDStr.indexOf(QString(">")));

    int id = IDStr.toInt(NULL, 16);



    if(m_ui->ItemsToDraw->selectionModel()->selectedRows(0).count())
        delete m_ui->ItemsToDraw->takeItem(m_ui->ItemsToDraw->selectionModel()->selectedRows(0).at(0).row());


    emit deleteItemToDraw(pCollection->getSignal(id, SignalName));
    return;
}


ofstream& SignalSelectorDialog::operator>>(ofstream& ofs)
{
    //Store num of selected items
    ofs.put((char)m_ui->ItemsToDraw->count());
    char temp[64];

    for(int f = 0 ; f < m_ui->ItemsToDraw->count() ; f ++ )
    {
        //SignalName
        memset(temp, 0, 64);
        memcpy(temp, m_ui->ItemsToDraw->item(f)->text().toStdString().c_str(), m_ui->ItemsToDraw->item(f)->text().count());
        for(int c = 0; c < 64 ; c++)
            ofs.put(temp[c]);

        //color
        QVariant VColor = m_ui->ItemsToDraw->item(f)->data(Qt::ItemIsSelectable);
        QColor Color =  *((QColor*)VColor.data());

        memset(temp, 0, 64);
        memcpy(temp, Color.name().toStdString().c_str(), Color.name().count());
        for(int c = 0; c < 64 ; c++)
            ofs.put(temp[c]);
    }
    return ofs;
}
ifstream& SignalSelectorDialog::operator<<(ifstream& is)
{
    char numofitems;
    is >> numofitems;

    for(int f = 0; f < numofitems ; f ++ )
    {
        QListWidgetItem item;
        char temp[64];

        //signal
        memset(temp, 0, 64);

        for(int d = 0 ; d < 64 ; d ++ )
            is >> temp[d];

        int idx = m_ui->ComboItemSelector->findText(QString(temp),Qt::MatchExactly);

        //Signal not found in the database
        if(idx == -1)
        {
            ErrorDialog *ed = new ErrorDialog();

            QString ErrMsg;
            ErrMsg.sprintf("Could not find the CAN Signal %s in the Database", temp);

            ed->SetErrorMessage(ErrMsg);
            ed->setModal(true);
            ed->exec();
            delete ed;
            return is;
        }
        //Select the signal
        m_ui->ComboItemSelector->setCurrentIndex(idx);




        //Color
        memset(temp, 0, 64);

        for(int d = 0 ; d < 64 ; d ++ )
            is >> temp[d];

        QColor StoredColor(temp);
        int coloridx;



        for(int v = 0 ; v < m_ui->ComboItemColorSelector->count() ; v ++ )
        {
            QString ColorName = m_ui->ComboItemColorSelector->itemText(v);
            QColor Color(ColorName);
            if(StoredColor == Color)
            {
                coloridx = v;
                break;
            }
        }
        //Select the color
        //(User is too lazy)
        m_ui->ComboItemColorSelector->setCurrentIndex(coloridx);

        //click on the add button
        on_AddItem_clicked();
    }
    return is;
}
