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

#include "observerdialog.h"
#include "ui_observerdialog.h"
#include "extrect.h"



ObserverDialog::ObserverDialog(QWidget *parent, CANSignalCollection *Collection) :
    QDialog(parent),
    m_ui(new Ui::ObserverDialog)
{
    pparent = parent;
    pCollection = Collection;

    m_ui->setupUi(this);

    QStringList *list = new QStringList();
    list->append(QString("Name"));
    list->append(QString("Value"));
    list->append(QString("Unit"));
    list->append(QString("Time"));
    TraceModel = new StringListModel(list);
    delete list;

    m_ui->tableView->setModel(TraceModel);

    m_ui->tableView->setColumnWidth(0, 120);
    m_ui->tableView->setColumnWidth(1, 80);
    m_ui->tableView->setColumnWidth(2, 40);
    m_ui->tableView->setColumnWidth(3, 120);
    m_ui->tableView->verticalHeader()->setDefaultSectionSize(15);
    m_ui->tableView->horizontalHeader()->setStretchLastSection(true);

    Sel = new SignalSelectorDialog(NULL, pCollection);
    connect(Sel, SIGNAL(addItemToDraw(CANSignal*, QColor)), this, SLOT(addItemToObserve(CANSignal*, QColor)));
    connect(Sel, SIGNAL(deleteItemToDraw(CANSignal*)), this, SLOT(deleteItemToObserve(CANSignal*)));

}

ObserverDialog::~ObserverDialog()
{
    delete m_ui;
    delete Sel;
    delete TraceModel;
    for(int i = 0; i < CANItems.count() ; i++)
        delete CANItems.at(i);
}


void ObserverDialog::newMessage(_CANMsg CANMsg, int Cnt)
{

    for( int i = 0 ; CANItems.count() >  i ; i++ )
    {

        if(CANMsg.ID == (DWORD)CANItems.at(i)->Signal->Id)
        {
            this->TempCANFRMS.append(CANMsg);
        }
    }

}

void ObserverDialog::MainTimerSlot()
{

    QList <_CANMsg> TempLst;
    TempLst =  QList <_CANMsg> (TempCANFRMS);
    TempCANFRMS.clear();


    QModelIndex index1 = TraceModel->index(0, 0, QModelIndex());
    TraceModel->insertRows(0,TempLst.count() , (const QModelIndex &)index1);

    int line = 0;

    QModelIndex vis = m_ui->tableView->indexAt(QPoint(1,1));



    while(TempLst.count())
    {
        _CANMsg CANMsg = TempLst.takeLast();
        for( int i = 0 ; CANItems.count() >  i ; i++ )
        {
            if(CANMsg.ID == (DWORD)CANItems.at(i)->Signal->Id)
            {
                SignalDataCollection DataCol;

                if(CANItems.at(i)->Signal->getSignalDataCollection(CANMsg.DATA, &DataCol))
                {
                    QString Name = DataCol.Name;
                    QString Unit = DataCol.Unit;
                    float Value = DataCol.Value;
                    index1 = TraceModel->index(line, 0, QModelIndex());


                    //get the valid index
                    index1 = TraceModel->index(line, 0, QModelIndex());
                    QVariant Col0(Name);
                    TraceModel->setData(index1,Col0,Qt::EditRole, CANItems.at(i)->Color);


                    index1 = TraceModel->index(line, 1, QModelIndex());

                    QVariant Col1(Value);
                    TraceModel->setData(index1,Col1,Qt::EditRole,  CANItems.at(i)->Color);

                    index1 = TraceModel->index(line, 2, QModelIndex());
                    QVariant Col2(Unit);
                    TraceModel->setData(index1,Col2,Qt::EditRole, CANItems.at(i)->Color);

                    index1 = TraceModel->index(line, 3, QModelIndex());
                    QString TimeString;
                    TimeString.sprintf("%f", (float)CANMsg.tv.tv_sec + (float)CANMsg.tv.tv_usec/1000000.0);
                    QVariant Col3(TimeString);
                    TraceModel->setData(index1,Col3,Qt::EditRole, CANItems.at(i)->Color);
                    line ++ ;
                }
            }
        }
    }
    if(0 != MaxDots && TraceModel->rowCount(QModelIndex()) > MaxDots)
    {
        int cnt = TraceModel->rowCount(QModelIndex());
        TraceModel->removeRows(MaxDots ,cnt-MaxDots, QModelIndex());
    }
    if(vis.isValid())
        m_ui->tableView->scrollTo(vis,QAbstractItemView::PositionAtTop);
}

void ObserverDialog::ClearAll()
{
    disconnect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));

    delete TraceModel;
    QStringList *list = new QStringList();
    list->append(QString("Name"));
    list->append(QString("Value"));
    list->append(QString("Unit"));
    list->append(QString("Time"));
    TraceModel = new StringListModel(list);
    delete list;



    m_ui->tableView->setModel(TraceModel);

    m_ui->tableView->setColumnWidth(0, 120);
    m_ui->tableView->setColumnWidth(1, 80);
    m_ui->tableView->setColumnWidth(2, 40);
    m_ui->tableView->setColumnWidth(3, 120);
    m_ui->tableView->verticalHeader()->setDefaultSectionSize(15);
    m_ui->tableView->horizontalHeader()->setStretchLastSection(true);

    m_ui->tableView->setModel(TraceModel);

    connect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));
}


//!SLOT that addes an Item to observe
//!Takes a CalRule (specific for an ID) and the invoked Rule
void ObserverDialog::addItemToObserve(CANSignal* Signal, QColor Color)
{
    CANItems.append(new ObserveItems(Signal, new QColor(Color)));
}
void ObserverDialog::deleteItemToObserve(CANSignal* Signal)
{
    for( int i = 0 ; i < CANItems.count() ; i ++ )
    {
        if(CANItems.at(i)->Signal == Signal)
        {
            delete CANItems.at(i);
            CANItems.removeAt(i);
        }
    }
}

void ObserverDialog::closeEvent( QCloseEvent *e )
{
    delete this;
    QWidget::closeEvent( e );
}

void ObserverDialog::changeEvent(QEvent *e)
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

void ObserverDialog::on_pushButton_clicked()
{
    Sel->show();
}

ofstream& ObserverDialog::operator>>(ofstream& os)
{
    //Geometry
    ExtRect e;
    QRect   q = this->geometry();
    e = &q;
    e >> os;

    //connection
    char connectedstate = (char)m_ui->ConnectedcheckBox->isChecked();
    os.put(connectedstate);

    (*Sel) >> os;
    return os;
}
ifstream& ObserverDialog::operator<<(ifstream& is)
{
    //Geometry
    ExtRect e;
    e << is;
    this->setGeometry(e);

    //connection
    char connectedstate;
    is >> connectedstate;
    m_ui->ConnectedcheckBox->setChecked((bool)connectedstate);

    (*Sel) << is;
    return is;
}

void ObserverDialog::on_ConnectedcheckBox_toggled(bool checked)
{
    if(checked)
        connect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));
    else
        disconnect(pparent, SIGNAL(newMessage(_CANMsg ,int)), this, SLOT(newMessage(_CANMsg ,int)));

}


void ObserverDialog::configChanged(__config cfg)
{

    if( 0 != cfg.GraphMemByte)
        MaxDots = cfg.GraphMemByte/(64);//??64Bytes in Tracemode for stringlist???
    else
        MaxDots = 0;
}

