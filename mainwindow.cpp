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

    periodicTimer = new QTimer( );
    connect(periodicTimer, SIGNAL(timeout()), this, SLOT(periodicUpdate()));

    FilterDlg = new FilterDialog();
    QObject::connect(FilterDlg, SIGNAL(setFilter(int, int, int)),
                     rt, SLOT(setFilter(int, int, int)));

    QObject::connect(this, SIGNAL(ClearAll()),
                     rt, SIGNAL(ClearAll()));

    DB = NULL;

    ui->setupUi(this);
    ui->checkBox->setDisabled(true);

    ui->tableView->verticalHeader()->setDefaultSectionSize(15);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(100);

    ui->MsgLossWarning->setNumDigits(0);

    TraceModel = new StringListModel();
   
    ui->tableView->setModel(TraceModel);


    QObject::connect(this, SIGNAL(setDev(QString, int, int)),
                     rt, SLOT(setDev(QString, int, int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionDevice_triggered()
{
    DevDialog *qd = new DevDialog(this);





    QObject::connect(qd, SIGNAL(setDev(QString, int, int)),
                     this, SIGNAL(setDev(QString, int, int)));

    QObject::connect(this, SIGNAL(QuitThread()),
                     rt, SLOT(QuitThread()));

    qd->setModal(true);
    qd->exec();

    ui->checkBox->setEnabled(true);
    delete qd;
}

void MainWindow::on_actionStart_triggered()
{
    rt->start();
    periodicTimer->start(100);
}

void MainWindow::on_actionStop_triggered()
{
    //Switch to the full list
    periodicTimer->stop();
    emit QuitThread();
}

void MainWindow::newMessage(CANMsgandTimeStruct *CANMsgandTime, int MsgCnt)
{
    QString MsgString;
    QString IDString;
    QString TimeString;

    MsgCounter ++ ;

    if(MsgCounter != MsgCnt)
    {
        ui->MsgLossWarning->setNumDigits(1);
    }



    MsgString.sprintf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", CANMsgandTime->CANMsg.DATA[0], CANMsgandTime->CANMsg.DATA[1], CANMsgandTime->CANMsg.DATA[2], CANMsgandTime->CANMsg.DATA[3], CANMsgandTime->CANMsg.DATA[4]
                                                            , CANMsgandTime->CANMsg.DATA[5], CANMsgandTime->CANMsg.DATA[6], CANMsgandTime->CANMsg.DATA[7]);
    IDString.sprintf("0x%04x", CANMsgandTime->CANMsg.ID);
    TimeString.sprintf("%f", (float)CANMsgandTime->timev.tv_sec + (float)CANMsgandTime->timev.tv_usec/1000000.0);

    QModelIndex index1 = TraceModel->index(0, 0, QModelIndex());
    TraceModel->insertRows(0, 1, (const QModelIndex &)index1);


    QVariant Col0(IDString);
    TraceModel->setData(index1,Col0,Qt::EditRole);

    index1 = TraceModel->index(0, 1, QModelIndex());
    QVariant Col1(MsgString);
    TraceModel->setData(index1,Col1,Qt::EditRole);

    index1 = TraceModel->index(0, 2, QModelIndex());
    QVariant Col2(TimeString);
    TraceModel->setData(index1,Col2,Qt::EditRole);
}


void MainWindow::on_actionClear_triggered()
{
    emit ClearAll();
    delete TraceModel;
    TraceModel = new StringListModel();
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

void MainWindow::periodicUpdate(void)
{
    TraceModel->Update();
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    QRect Rect = this->geometry();
    int w = Rect.width();
    Rect.setX(Rect.x() + 600);
    Rect.setWidth(w);
    if(checked)
    {
        FilterDlg->setGeometry(Rect);
        FilterDlg->show();
    }
    else
        FilterDlg->hide();
}

void MainWindow::on_actionClose_triggered()
{

}

void MainWindow::on_MainWindow_destroyed()
{

}

void MainWindow::closeEvent( QCloseEvent *e )
{

    delete FilterDlg;


    //Delete alle Graphic Windows
    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        if(GraphWnd[i] != NULL)
            delete GraphWnd[i];

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
                GraphWnd[i] = new GraphicWindow(NULL, RuleList);
                GraphWnd[i]->show();

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
        DB->getRuleList(&RuleList);
        RuleList->count();
    }
    else
        return;
}

void MainWindow::on_tableView_clicked(QModelIndex index)
{
    unsigned char hexdat[8];

    QModelIndex id_idx = TraceModel->index(index.row(), 0, QModelIndex());
    QModelIndex data_idx = TraceModel->index(index.row(), 1, QModelIndex());

    QVariant DataVar = TraceModel->data(data_idx, Qt::DisplayRole);
    QString DataStr = DataVar.toString();

    QVariant IdVar = TraceModel->data(id_idx, Qt::DisplayRole);
    QString IdStr = IdVar.toString();

    int id = IdStr.toInt(NULL, 16);
    char *data;
    data = (char*)DataStr.toStdString().c_str();

    for(int i = 0 ; i < 8 ; i ++ )
        hexdat[i] = strtol(data+i*5, NULL, 16);

    if(DB)
    {
//        int NumOfVals = DB->getNumOfValueNamePairs(id);
//
//        float Values[32];
//        QString Names[32];
//        QString InformationText;
//
//        if(DB->getValueNamePairs((char*)hexdat, id, Values, Names))
//        {
//            for(int f = 0 ; f < NumOfVals ; f++ )
//            {
//                QString Value;
//                Value.sprintf("%f",(Values[f]));
//                InformationText = InformationText+QString(" ")+Names[f]+QString(" = ")+Value+QString(" ");
//            }
//            ui->FrameInformationLable->setText(InformationText);
//        }
    }
}
