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
#include <QCloseEvent>



void MainWindow::initSatelites()
{
    rt = new ReadThread;


    QObject::connect(rt->MsgBuf, SIGNAL(newMessage(_CANMsg *, int)),
                     this, SLOT(addnewMessage(_CANMsg *, int)));
    QObject::connect(rt->MsgBuf, SIGNAL(newMessage(_CANMsg *, int)),
                     this, SIGNAL(newMessage(_CANMsg *, int)));
    QObject::connect(this, SIGNAL(ClearAll()),
                     rt, SIGNAL(ClearAll()));
    QObject::connect(this, SIGNAL(setDev(void *, QString, bool)),
                     rt, SLOT(setDev(void *, QString, bool)));
    QObject::connect(this, SIGNAL(StopCapture()),
                     rt, SLOT(QuitThread()));
    QObject::connect(rt, SIGNAL(DevIsConfigured(bool)),
                     this, SLOT(DevIsConfigured(bool)));

    wt = new WriteThread;

    QObject::connect(this, SIGNAL(setDev(void *, QString, bool)),
                     wt, SLOT(setDev(void *, QString, bool)));
    QObject::connect(this, SIGNAL(ClearAll()),
                     rt, SIGNAL(ClearAll()));

 //   QObject::connect(rt->MsgBuf, SIGNAL(newMessage(_CANMsg *, int)),
 //                    wt, SLOT(addnewMessage(_CANMsg *, int)));

    QObject::connect(rt, SIGNAL(setDevLibInstance(CANDevice *)),
                     wt, SLOT(setDevLibInstance(CANDevice *)));

    QObject::connect(this, SIGNAL(StopCapture()),
                     wt, SLOT(QuitThread()));



    CANSignals = NULL;

    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
    {
        GraphWnd[i] = NULL;
    }

    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
    {
        ObserverWnd[i] = NULL;
    }

    FilterDlg = new FilterDialog();

    SendMsgDlg = new SendMsgDialog();



    //After the device is configured
    DevDlg = new DevDialog(this);
    QObject::connect(DevDlg, SIGNAL(setDev(void *, QString, bool)),
                     this, SIGNAL(setDev(void *, QString, bool)));

    QObject::connect(SendMsgDlg, SIGNAL(sendCANMsg(_CANMsg , int, _CANMsg, int)),
                     wt, SLOT(sendCANMsg(_CANMsg , int , _CANMsg, int )));
    QObject::connect(SendMsgDlg, SIGNAL(deleteCANMsg(_CANMsg , int, _CANMsg, int)),
                     wt, SLOT(deleteCANMsg(_CANMsg , int, _CANMsg, int )));


    QObject::connect(FilterDlg, SIGNAL(setFilter(int, int, int)),
                     rt, SLOT(setFilter(int, int, int)));
    DevDlg->setWindowTitle("Select a device");


    //for RTRs and Error Frames
    SpecEvtDlg = new SpecialEventDialog();
    QObject::connect(rt->MsgBuf, SIGNAL(newSpecialMessage(_CANMsg *)),
                     SpecEvtDlg, SLOT(newSpecialMessage(_CANMsg *)));
    connect(periodicTimer, SIGNAL(timeout()), SpecEvtDlg, SLOT(MainTimerSlot()));
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

    qRegisterMetaType<_CANMsg>("_CANMsg");



    MsgCounter = 0;

    black = QColor(Qt::black);


    periodicTimer = new QTimer( );
    connect(periodicTimer, SIGNAL(timeout()), this, SLOT(periodicUpdate()));




    DB = NULL;

    ui->setupUi(this);
    setWindowTitle("QCANObserver");

    //Filter
    ui->checkBoxFilters->setEnabled(false);
    //SendMsgs
    ui->checkBoxSendMsg->setEnabled(false);


    ui->MsgCounter->setNumDigits(7);
    ui->MsgLossWarning->setNumDigits(7);

    ui->tableView->verticalHeader()->setDefaultSectionSize(15);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(200);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList *list = new QStringList();
    list->append(QString("ID"));
    list->append(QString("Data"));
    list->append(QString("Time"));
    TraceModel = new StringListModel(list);
    
    delete list;

    //Model Checker
    //new ModelTest(TraceModel, this);

    ui->tableView->setModel(TraceModel);



    ui->tableView->setColumnWidth(0, 70);
    ui->tableView->setColumnWidth(1, 300);
    ui->tableView->setColumnWidth(2, 90);

    initSatelites();

    if(-1 == loadDefaultConfig())
    {
        ErrorDialog *Error = new ErrorDialog();
        Error->SetErrorMessage("Could not load default Configuration file (.config)");
        Error->setModal(true);
        Error->exec();
    }
}

MainWindow::~MainWindow()
{
    rt->terminate();
    wt->terminate();

    emit StopCapture();


    delete rt;
    delete wt;
    delete periodicTimer;
    delete TraceModel;

    if(FilterDlg)
        delete FilterDlg;

    if(CANSignals)
        delete CANSignals;

    if(DB)
        delete DB;
    if(SpecEvtDlg)
        delete SpecEvtDlg;

    delete ui;
}


//add the message to the Model
//SLOT
void MainWindow::addnewMessage(_CANMsg *CANMsg, int MsgCnt)
{
    QString MsgString;
    QString IDString;
    QString TimeString;

    MsgCounter ++ ;

    if(MsgCounter != MsgCnt)
    {
        ui->MsgLossWarning->display(MsgCnt - MsgCounter);
    }



    MsgString.sprintf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", CANMsg->DATA[0], CANMsg->DATA[1], CANMsg->DATA[2], CANMsg->DATA[3], CANMsg->DATA[4]
                                                            , CANMsg->DATA[5], CANMsg->DATA[6], CANMsg->DATA[7]);
    IDString.sprintf("0x%04x", (unsigned int)CANMsg->ID);
    TimeString.sprintf("%f", (float)CANMsg->tv.tv_sec + (float)CANMsg->tv.tv_usec/1000000.0);


    QModelIndex index1 = TraceModel->index(0, 0, QModelIndex());
    TraceModel->insertRows(0, 1, (const QModelIndex &)index1);

    index1 = TraceModel->index(1, 0, QModelIndex());
    QVariant Col0(IDString);
    TraceModel->setData(index1,Col0,Qt::EditRole, &black);

    index1 = TraceModel->index(1, 1, QModelIndex());
    QVariant Col1(MsgString);
    TraceModel->setData(index1,Col1,Qt::EditRole, &black);

    index1 = TraceModel->index(1, 2, QModelIndex());
    QVariant Col2(TimeString);
    TraceModel->setData(index1,Col2,Qt::EditRole, &black);
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
    //stop first
    if(rt->isRunning() || wt->isRunning())
    {
        e->ignore();
        ErrorDialog *ed = new ErrorDialog();
        ed->SetErrorMessage("Stop Capture first!");
        ed->setModal(true);
        ed->exec();
        delete ed;
        return;
    }

    //Delete alle Graphic Windows
    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        if(GraphWnd[i] != NULL)
            delete GraphWnd[i];

    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        if(ObserverWnd[i] != NULL)
            delete ObserverWnd[i];

    delete SpecEvtDlg;
    SpecEvtDlg = NULL;
    delete SendMsgDlg;
    SendMsgDlg = NULL;
    delete FilterDlg;
    FilterDlg = NULL;
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
                GraphWnd[i] = new GraphicWindow(this, CANSignals);
                GraphWnd[i]->setWindowTitle("GraphicWindow");
                GraphWnd[i]->move(this->pos().x()+this->geometry().width(), this->pos().y());
                GraphWnd[i]->show();
                connect(this, SIGNAL(StopCapture()), GraphWnd[i], SLOT(StopCapture()));
                connect(this, SIGNAL(newMessage(_CANMsg *, int)), GraphWnd[i], SLOT(newMessage(_CANMsg *,int)));
                connect(periodicTimer, SIGNAL(timeout()), GraphWnd[i], SLOT(MainTimerSlot()));
                connect(this, SIGNAL(ClearAll()), GraphWnd[i], SLOT(ClearAll()));
                connect(GraphWnd[i], SIGNAL(destroyed(QObject*)), this, SLOT(SateliteDestroyed(QObject *)));
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
        if(dlg.selectedFiles().count())
        {
            loadDatabase(dlg.selectedFiles().at(0));
        }
    }
    else
        return;
}

int MainWindow::loadDatabase(QString File)
{
    if(DB)
    {
        //Delete alle Graphic Windows
        for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        {
            if(GraphWnd[i] != NULL)
            {
                ErrorDialog *ed = new ErrorDialog();
                ed->SetErrorMessage("Close first all Graphic/Observer Windows!");
                ed->setModal(true);
                ed->exec();
                delete ed;
                return -1;
            }
        }

        for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        {
            if(ObserverWnd[i] != NULL)
            {
                ErrorDialog *ed = new ErrorDialog();
                ed->SetErrorMessage("Close first all Graphic/Observer Windows!");
                ed->setModal(true);
                ed->exec();
                delete ed;
                return -1;
            }
        }
        delete DB;
        delete CANSignals;
    }

    DB = new ProcessDataBase(File);
    //make the Data public
    CANSignals = DB->getCANSignalList();

    if(CANSignals)
        return 1;
    else
    {
        delete DB;
        DB = NULL;
        return -1;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////Private Action Handlers from the controls///////////////////////////



void MainWindow::on_actionObserverWindow_triggered()
{
    if(DB)
    {
        for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        {
            if(ObserverWnd[i] == NULL)
            {
                ObserverWnd[i] = new ObserverDialog(this, CANSignals);
                ObserverWnd[i]->setWindowTitle("ObserverWindow");
                ObserverWnd[i]->move(this->pos().x()+this->geometry().width(), this->pos().y());
                ObserverWnd[i]->show();

                connect(this, SIGNAL(newMessage(_CANMsg *, int)), ObserverWnd[i], SLOT(newMessage(_CANMsg *,int)));
                connect(periodicTimer, SIGNAL(timeout()), ObserverWnd[i], SLOT(MainTimerSlot()));
                connect(this, SIGNAL(ClearAll()), ObserverWnd[i], SLOT(ClearAll()));
                connect(ObserverWnd[i], SIGNAL(destroyed(QObject*)), this, SLOT(SateliteDestroyed(QObject *)));
                return;
            }
        }
    }
    return;
}

void MainWindow::DevIsConfigured(bool isIt)
{
    if(isIt)
    {
        //Filter
        ui->checkBoxFilters->setEnabled(true);
        //SendMsgs
        ui->checkBoxSendMsg->setEnabled(true);


        // is the device capable of receving error frames
        //int Flags = DevDlg->getDeviceFlags();

        //if((Flags & ERROR_FR) || (Flags & RTR_FR))
        ui->checkBoxSpecEvtDlg->setEnabled(true);
    }
}

void MainWindow::on_actionDevice_triggered()
{
    DevDlg->setModal(true);
    DevDlg->exec();
}


void MainWindow::on_actionAbout_triggered()
{
    AboutBox *about = new AboutBox();
    about->setModal(true);
    about->exec();
    delete about;
}

void MainWindow::on_actionStart_triggered()
{
    if(rt->isConfigured())
    {
        rt->start();
        rt->moveToThread(rt);
        periodicTimer->start(100);

        //disable MenuItems
        QList <QAction*> act = ui->menuFile->actions();
        for(int i = 0; i < act.count() ; i ++)
        {
            if(act.at(i)->text() == QString("Start"))
                act.at(i)->setEnabled(false);
            if(act.at(i)->text() == QString("Clear"))
                act.at(i)->setEnabled(false);
        }

        ui->menuFile_2->setEnabled(false);
    }
    else
    {
        ErrorDialog *ed = new ErrorDialog;
        ed->SetErrorMessage("No Device is Configured!");

        ed->setModal(true);
        ed->exec();
        delete ed;

        return;
    }
}

void MainWindow::on_actionStop_triggered()
{
    //Switch to the full list
    rt->terminate();
    wt->terminate();
    periodicTimer->stop();

    //enable Menu Items
    QList <QAction*> act = ui->menuFile->actions();
    for(int i = 0; i < act.count() ; i ++)
    {
        if(act.at(i)->text() == QString("Start"))
            act.at(i)->setEnabled(true);
        if(act.at(i)->text() == QString("Clear"))
            act.at(i)->setEnabled(true);
    }
    ui->menuFile_2->setEnabled(true);
    emit StopCapture();
}


void MainWindow::on_actionClose_triggered()
{
    delete this;
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
    delete list;
    MsgCounter = 0;
    ui->MsgLossWarning->display(0);
    ui->MsgCounter->display(0);
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
void MainWindow::SateliteDestroyed(QObject *Obj)
{
    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        if(GraphWnd[i] == Obj)
            GraphWnd[i] = NULL;

    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        if(ObserverWnd[i] == Obj)
            ObserverWnd[i] = NULL;
    return;
}

int MainWindow::SaveConfig(QString Filename)
{

    ofstream ofs(Filename.toStdString().c_str(), ios::binary);

    ExtRect pos;
    QRect b = this->geometry();
    pos = &b;
    pos >> ofs;
    //DeviceConfig
    ofs.flush();
    (*DevDlg) >> ofs;
    ofs.flush();
    if(DB)
    {
        //db is configured
        ofs.put((char)1);
        //Store the dbfilename
        char temp[512];
        memset(temp, 0, 512);
        QString DBFileName = DB->getFileName();
        memcpy(temp,DBFileName.toStdString().c_str(),DBFileName.count());
        for(int f = 0; f < 512 ; f++)
            ofs.put(temp[f]);

        for( char i = 0 ; i < MAX_GRAPH_WINDOWS ; i++ )
        {
            if(GraphWnd[(int)i] == NULL)
            {
                ofs.put(i);       //store the number of grph windows
                break;
            }
        }

        for( int i = 0 ; i < MAX_GRAPH_WINDOWS ; i++ )
        {
            if(GraphWnd[i] == NULL)
                break;
            (*GraphWnd[i]) >> ofs;
        }

        for( char i = 0 ; i < MAX_GRAPH_WINDOWS ; i++ )
        {
            if(ObserverWnd[(int)i] == NULL)
            {
                ofs.put(i);       //store the number of grph windows
                break;
            }
        }

        for( int i = 0 ; i < MAX_GRAPH_WINDOWS ; i++ )
        {
            if(ObserverWnd[i] == NULL)
                break;
            (*ObserverWnd[i]) >> ofs;
        }
    }

    else //No DB
        ofs.put(0);

    ofs.close();
    return 1;
}

void MainWindow::on_actionSave_Config_triggered()
{

    QFileDialog dlg(this, QString("Select a Configuration File *.ObsCfg"),QString("cfg/"),NULL);
    dlg.setModal(true);
    if(dlg.exec())
    {
        QStringList Files = dlg.selectedFiles();
        QString File = Files.at(0);
        SaveConfig(File);
    }
    else
        return;
}



void MainWindow::on_actionSave_Config_as_Default_triggered()
{
    SaveConfig(QString("cfg/.config"));
}

int MainWindow::loadDefaultConfig()
{
    return loadConfig(QString("cfg/.config"));
}

int MainWindow::loadConfig(QString FileName)
{
    ifstream ifs(FileName.toStdString().c_str(), ios::binary);

    if(!ifs.is_open())
        return -1;

    ExtRect pos;
    pos << ifs;

    this->setGeometry(pos);

    //DeviceConfig
    (*DevDlg) << ifs;

    char dbConfigured;
    ifs.get(dbConfigured);
    //Store the dbfilename

    if(dbConfigured)
    {
        //load the dbfilename
        char temp[512];


        for(int f = 0; f < 512 ; f++)
            ifs.get(temp[f]);

        QString DBFileName(temp);

        if(-1 == loadDatabase(DBFileName))
            return -1;             //open Observer or GraphicWindows -> break up

        //load the GraphicWindows
        char numofgraphwnds;
        ifs.get(numofgraphwnds);

        for(int i = 0 ; i < numofgraphwnds ; i++ )
        {
            on_actionGraphicWindow_triggered();
            (*GraphWnd[i]) << ifs;
        }

        //load the ObserverWindows
        char numofobserverwnds;
        ifs.get(numofobserverwnds);

        for(int i = 0 ; i < numofobserverwnds ; i++ )
        {
            on_actionObserverWindow_triggered();
            (*ObserverWnd[i]) << ifs;
        }
    }
    ifs.close();
    return 1;
}

void MainWindow::on_actionLoad_Config_triggered()
{
    QFileDialog dlg(this, QString("Select a Configuration File *.Obscfg"),QString("cfg/"),NULL);
    dlg.setModal(true);
    if(dlg.exec())
    {
        if(dlg.selectedFiles().count())
            loadConfig(dlg.selectedFiles().at(0));
    }

    else
        return;         //cancel
}

void MainWindow::on_actionConfiguration_triggered()
{

}


void MainWindow::on_checkBoxSpecEvtDlg_toggled(bool checked)
{
    if(checked)
        SpecEvtDlg->show();
    else
        SpecEvtDlg->hide();
}

void MainWindow::on_checkBoxFilters_toggled(bool checked)
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

void MainWindow::on_checkBoxSendMsg_toggled(bool checked)
{
    SendMsgDlg->move(this->pos().x(), this->pos().y()+this->geometry().height()+30);
    if(checked)
    {
        wt->start(QThread::TimeCriticalPriority);
        wt->moveToThread(wt);
        SendMsgDlg->show();
    }
    else
    {
        SendMsgDlg->hide();
        wt->terminate();
    }
}
