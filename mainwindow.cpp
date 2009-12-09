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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
 #include <QMap>
#include "errordialog.h"
#include "aboutbox.h"
#include <modeltest.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    MsgCounter = 0;

    rt = new ReadThread;
    QObject::connect(rt->MsgBuf, SIGNAL(newMessage(CANMsgandTimeStruct *, int)),
                     this, SLOT(newMessage(CANMsgandTimeStruct *, int)));


    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
    {
        GraphWnd[i] = NULL;
    }

    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
    {
        ObserverWnd[i] = NULL;
    }
    periodicTimer = new QTimer( );
    connect(periodicTimer, SIGNAL(timeout()), this, SLOT(periodicUpdate()));

    FilterDlg = new FilterDialog();

    SendMsgDlg = new SendMsgDialog();

    QObject::connect(this, SIGNAL(ClearAll()),
                     rt, SIGNAL(ClearAll()));

    DB = NULL;

    ui->setupUi(this);
    setWindowTitle("QCANObserver");

    //Filter
    ui->checkBox->setEnabled(false);
    //SendMsgs
    ui->checkBox_2->setEnabled(false);


    ui->MsgCounter->setNumDigits(7);
    ui->MsgLossWarning->setNumDigits(7);
    ui->TextLabelAddedMsgs->setText(QwtText(QString("Added Messages:")));
    ui->TextLabelLostMsgs->setText(QwtText(QString("Lost Messages:")));




    ui->tableView->verticalHeader()->setDefaultSectionSize(15);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(200);


    QStringList *list = new QStringList();
    list->append(QString("ID"));
    list->append(QString("Data"));
    list->append(QString("Time"));
    TraceModel = new StringListModel(list);

    //Model Checker
    //new ModelTest(TraceModel, this);

    ui->tableView->setModel(TraceModel);



    ui->tableView->setColumnWidth(0, 70);
    ui->tableView->setColumnWidth(1, 300);
    ui->tableView->setColumnWidth(2, 90);



    QObject::connect(this, SIGNAL(setDev(QString, int, int, QString)),
                     rt, SLOT(setDev(QString, int, int, QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


//add the message to the Model
//SLOT
void MainWindow::newMessage(CANMsgandTimeStruct *CANMsgandTime, int MsgCnt)
{
    QString MsgString;
    QString IDString;
    QString TimeString;

    MsgCounter ++ ;

    if(MsgCounter != MsgCnt)
    {
        ui->MsgLossWarning->display(MsgCnt - MsgCounter);
    }



    MsgString.sprintf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", CANMsgandTime->CANMsg.DATA[0], CANMsgandTime->CANMsg.DATA[1], CANMsgandTime->CANMsg.DATA[2], CANMsgandTime->CANMsg.DATA[3], CANMsgandTime->CANMsg.DATA[4]
                                                            , CANMsgandTime->CANMsg.DATA[5], CANMsgandTime->CANMsg.DATA[6], CANMsgandTime->CANMsg.DATA[7]);
    IDString.sprintf("0x%04x", CANMsgandTime->CANMsg.ID);
    TimeString.sprintf("%f", (float)CANMsgandTime->timev.tv_sec + (float)CANMsgandTime->timev.tv_usec/1000000.0);

    QModelIndex index1 = TraceModel->index(0, 0, QModelIndex());
    TraceModel->insertRows(0, 1, (const QModelIndex &)index1);

    index1 = TraceModel->index(0, 0, QModelIndex());
    QVariant Col0(IDString);
    TraceModel->setData(index1,Col0,Qt::EditRole, new QColor(Qt::black));

    index1 = TraceModel->index(0, 1, QModelIndex());
    QVariant Col1(MsgString);
    TraceModel->setData(index1,Col1,Qt::EditRole, new QColor(Qt::black));

    index1 = TraceModel->index(0, 2, QModelIndex());
    QVariant Col2(TimeString);
    TraceModel->setData(index1,Col2,Qt::EditRole, new QColor(Qt::black));
}



//update the view with all arrived msgs
void MainWindow::periodicUpdate(void)
{
    TraceModel->Update();
    int msgs = TraceModel->rowCount();

    QString NumOfMsgs;
    NumOfMsgs.sprintf("%d",msgs);
    ui->MsgCounter->display(NumOfMsgs);
}



void MainWindow::closeEvent( QCloseEvent *e )
{

    delete FilterDlg;
    delete SendMsgDlg;

    //Delete alle Graphic Windows
    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        if(GraphWnd[i] != NULL)
            delete GraphWnd[i];

    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        if(ObserverWnd[i] != NULL)
            delete ObserverWnd[i];

    QMainWindow::closeEvent( e );
}

void MainWindow::on_actionGraphicWindow_triggered()
{
    if(DB)
    {
        for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        {
            if(GraphWnd[i] == NULL)
            {
		GraphWnd[i] = new GraphicWindow(NULL, CANSignals);
                GraphWnd[i]->setWindowTitle("GraphicWindow");
                GraphWnd[i]->move(this->pos().x()+this->geometry().width(), this->pos().y());
                GraphWnd[i]->show();
                connect(this, SIGNAL(StopCapture()), GraphWnd[i], SLOT(StopCapture()));
                connect(rt->MsgBuf, SIGNAL(newMessage(CANMsgandTimeStruct *,int)), GraphWnd[i], SLOT(newMessage(CANMsgandTimeStruct *,int)));
                connect(periodicTimer, SIGNAL(timeout()), GraphWnd[i], SLOT(MainTimerSlot()));
                connect(this, SIGNAL(ClearAll()), GraphWnd[i], SLOT(ClearAll()));
		return;
            }
        }
    }
    return;
}

void MainWindow::on_actionDatabase_triggered()
{
    QFileDialog dlg(this, QString("Select a Database *.xml"),QString("db/"),NULL);
    dlg.setModal(true);
    if(dlg.exec())
    {
        QStringList Files = dlg.selectedFiles();
        QString File = Files.at(0);
        DB = new ProcessDataBase(File);
	//make the Data public
	CANSignals = DB->getCANSignalList();
	//delete DB;

    }
    else
        return;
}



//add message
void MainWindow::on_checkBox_2_toggled(bool checked)
{
    SendMsgDlg->move(this->pos().x(), this->pos().y()+this->geometry().height()+30);
    if(checked)
        SendMsgDlg->show();
    else
        SendMsgDlg->hide();
}





//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////Private Action Handlers from the controls///////////////////////////


//Unused
void MainWindow::on_tableView_clicked(QModelIndex index)
{
    unsigned char hexdat[8];

    QModelIndex id_idx = TraceModel->index(index.row(), 0, QModelIndex());
    QModelIndex data_idx = TraceModel->index(index.row(), 1, QModelIndex());

    QVariant DataVar = TraceModel->data(data_idx, Qt::DisplayRole);
    QString DataStr = DataVar.toString();

    QVariant IdVar = TraceModel->data(id_idx, Qt::DisplayRole);
    QString IdStr = IdVar.toString();

    char *data;
    data = (char*)DataStr.toStdString().c_str();

    for(int i = 0 ; i < 8 ; i ++ )
        hexdat[i] = strtol(data+i*5, NULL, 16);

}


void MainWindow::on_actionObserverWindow_triggered()
{
    if(DB)
    {
        for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        {
            if(ObserverWnd[i] == NULL)
            {
                ObserverWnd[i] = new ObserverDialog(NULL, CANSignals);
                ObserverWnd[i]->setWindowTitle("ObserverWindow");
                ObserverWnd[i]->move(this->pos().x()+this->geometry().width(), this->pos().y());
                ObserverWnd[i]->show();

                connect(rt->MsgBuf, SIGNAL(newMessage(CANMsgandTimeStruct *,int)), ObserverWnd[i], SLOT(newMessage(CANMsgandTimeStruct *,int)));
                connect(periodicTimer, SIGNAL(timeout()), ObserverWnd[i], SLOT(MainTimerSlot()));
                connect(this, SIGNAL(ClearAll()), ObserverWnd[i], SLOT(ClearAll()));
                return;
            }
        }
    }
    return;
}

void MainWindow::on_actionDevice_triggered()
{
    DevDialog *qd = new DevDialog(this);


    QObject::connect(qd, SIGNAL(setDev(QString , int , int , QString )),
                     this, SIGNAL(setDev(QString , int , int , QString )));

    QObject::connect(this, SIGNAL(StopCapture()),
                     rt, SLOT(QuitThread()));

    QObject::connect(SendMsgDlg, SIGNAL(sendCANMsg(TPCANMsg *)),
                     rt, SLOT(sendCANMsg(TPCANMsg *)));

    QObject::connect(FilterDlg, SIGNAL(setFilter(int, int, int)),
                     rt, SLOT(setFilter(int, int, int)));


    qd->setWindowTitle("Select a device");
    qd->setModal(true);
    qd->exec();

    //Filter
    ui->checkBox->setEnabled(true);
    //SendMsgs
    ui->checkBox_2->setEnabled(true);
    delete qd;
}


void MainWindow::on_actionAbout_triggered()
{
    AboutBox *about = new AboutBox();
    about->setModal(true);
    about->exec();
    delete about;
}
//Unused
void MainWindow::on_actionSendDialog_triggered()
{


}

void MainWindow::on_actionStart_triggered()
{
    if(rt->isConfigured())
    {
        rt->start();
        periodicTimer->start(100);
    }
    else
    {
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage("No Device is Configured!");
        ed->setModal(true);
        ed->show();

        //delete ed;
        return;
    }
}

void MainWindow::on_actionStop_triggered()
{
    //Switch to the full list
    periodicTimer->stop();
    emit StopCapture();
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    if(checked)
    {
        int x = this->geometry().x();
        FilterDlg->move(x+600,0);
        FilterDlg->show();
    }
    else
        FilterDlg->hide();
}
//Unused
void MainWindow::on_actionClose_triggered()
{

}
//Unused
void MainWindow::on_MainWindow_destroyed()
{

}

void MainWindow::on_actionClear_triggered()
{
    emit ClearAll();
    delete TraceModel;
    QStringList *list = new QStringList();
    list->append(QString("ID"));
    list->append(QString("Data"));
    list->append(QString("Time"));
    TraceModel = new StringListModel(list);
    MsgCounter = 0;
    ui->tableView->setModel(TraceModel);
}

void MainWindow::on_actionSave_triggered()
{
    QFileDialog dlg(this, QString("Select a Capture File *.dat"),QString("Save/"),NULL);
    dlg.setModal(true);
    if(dlg.exec())
    {
        QStringList Files = dlg.selectedFiles();
        QString File = Files.at(0);
        rt->MsgBuf->Save((char*)File.toStdString().c_str());

    }
    else
        return;
}

void MainWindow::on_actionLoad_triggered()
{
    QFileDialog dlg(this, QString("Select a Capture File *.dat"),QString("Save/"),NULL);
    dlg.setModal(true);
    if(dlg.exec())
    {
        QStringList Files = dlg.selectedFiles();
        QString File = Files.at(0);
        rt->MsgBuf->Load((char*)File.toStdString().c_str());

        TraceModel->Update();

        //Inform All the other Windows
        QTimer::singleShot(3, (QObject*)periodicTimer, SIGNAL(timeout()));

    }
    else
        return;
}
