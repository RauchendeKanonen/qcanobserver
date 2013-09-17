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
#include "errno.h"
#include <sys/types.h>
#include <unistd.h>
#include "suggestiondialog.h"


void MainWindow::initSatelites()
{
    ConfDlg = new ConfigDialog();
     QObject::connect(ConfDlg, SIGNAL(configChanged(__config)),
                     this, SLOT(configChanged(__config)));

    rt = new ReadThread;
    QObject::connect(ConfDlg, SIGNAL(configChanged(__config)),
                    rt, SIGNAL(configChanged(__config)));
    QObject::connect(ConfDlg, SIGNAL(configChanged(__config)),
                    rt->MsgBuf, SLOT(configChanged(__config)));

    QObject::connect(rt->MsgBuf, SIGNAL(newMessage(_CANMsg , int)),
                     this, SLOT(addnewMessage(_CANMsg , int)));
    QObject::connect(rt->MsgBuf, SIGNAL(newMessage(_CANMsg , int)),
                     this, SIGNAL(newMessage(_CANMsg , int)));
    QObject::connect(this, SIGNAL(ClearAll()),
                     rt, SIGNAL(ClearAll()));
    QObject::connect(this, SIGNAL(setDev(void *, QString, bool)),
                     rt, SLOT(setDev(void *, QString, bool)));
    QObject::connect(rt, SIGNAL(StopCapture()),
		     this, SLOT(QuitThread()));
    QObject::connect(rt, SIGNAL(DevIsConfigured(bool)),
                     this, SLOT(DevIsConfigured(bool)));

    wt = new WriteThread;

    QObject::connect(this, SIGNAL(setDev(void *, QString, bool)),
                     wt, SLOT(setDev(void *, QString, bool)));
    QObject::connect(this, SIGNAL(ClearAll()),
                     rt, SIGNAL(ClearAll()));
    QObject::connect(rt, SIGNAL(NoMem()),
		     this, SLOT(NoMem()));
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
        DbgTerminal[i] = NULL;
    }

    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
    {
        ObserverWnd[i] = NULL;
    }

    FilterDlg = new FilterDialog();

    //After the device is configured
    DevDlg = new DevDialog(this);
    QObject::connect(DevDlg, SIGNAL(setDev(void *, QString, bool)),
                     this, SIGNAL(setDev(void *, QString, bool)));

    QObject::connect(FilterDlg, SIGNAL(setFilter(int, int, int)),
                     rt, SLOT(setFilter(int, int, int)));
    DevDlg->setWindowTitle("Select a device");


    //for RTRs and Error Frames
    SpecEvtDlg = new SpecialEventDialog(this);
    QObject::connect(rt->MsgBuf, SIGNAL(newSpecialMessage(_CANMsg )),
                     SpecEvtDlg, SLOT(newSpecialMessage(_CANMsg )));
    connect(periodicTimer, SIGNAL(timeout()), SpecEvtDlg, SLOT(MainTimerSlot()));
    connect(this, SIGNAL(ClearAll()), SpecEvtDlg, SLOT(ClearAll()));
}


void MainWindow::initSendMsgDlg(void)
{

    SendMsgDlg = new SendMsgDialog(this, CANSignals);
    QObject::connect(SendMsgDlg, SIGNAL(sendCANMsg(_CANMsg , int, _CANMsg, int)),
                     wt, SLOT(sendCANMsg(_CANMsg , int , _CANMsg, int )));
    QObject::connect(SendMsgDlg, SIGNAL(deleteCANMsg(_CANMsg , int, _CANMsg, int)),
                     wt, SLOT(deleteCANMsg(_CANMsg , int, _CANMsg, int )));
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

    qRegisterMetaType<_CANMsg>("_CANMsg");

    black = QColor(Qt::black);

    periodicTimer = new QTimer( );
    connect(periodicTimer, SIGNAL(timeout()), this, SLOT(periodicUpdate()));

    MaxTraceListLenght = 0;

    DB = NULL;
    queueTimer = NULL;
    Args = NULL;

    msgs_1 = 0;
    FreqDivFlipFlop = 0;
    SpeedFilter = new AverageFilter(5);

    ui->setupUi(this);
    setWindowTitle("QCANObserver");

    //Filter
    ui->checkBoxFilters->setEnabled(false);
    //SendMsgs
    ui->checkBoxSendMsg->setEnabled(false);


    ui->MsgCounter->setNumDigits(7);
    ui->lcdNumberMsgsperSec->setNumDigits(7);
    ui->lcdNumberMsgsperSec->setEnabled(true);

    ui->tableView->verticalHeader()->setDefaultSectionSize(15);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(200);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList *list = new QStringList();
    list->append(QString("ID"));
    list->append(QString("Data"));
    list->append(QString("Time"));
    TraceModel = new RawDataModel(list);
    delete list;

    //Model Checker
    //new ModelTest(TraceModel, this);

    ui->tableView->setModel(TraceModel);



    ui->tableView->setColumnWidth(0, 70);
    ui->tableView->setColumnWidth(1, 270);
    ui->tableView->setColumnWidth(2, 90);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    initSatelites();


    if(-1 == loadDefaultConfig())
    {
        ErrorDialog *Error = new ErrorDialog();
        Error->SetErrorMessage("Could not load default Configuration file (.config)");
        Error->setModal(true);
        Error->exec();
    }

    initSendMsgDlg();


    if(!QFile("DialogShown").exists())
    {
        SuggestionDialog Dlg;
        Dlg.setModal(true);
        Dlg.exec();

        QFile Mark("DialogShown");
        Mark.open(QFile::WriteOnly);
        Mark.write("Shown");
        Mark.close();
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
void MainWindow::addnewMessage(_CANMsg CANMsg, int MsgCnt)
{
    TempDataList.append(CANMsg);
}



//update the view with all arrived msgs
void MainWindow::periodicUpdate(void)
{
    FreqDivFlipFlop++;
    QList  <_CANMsg> TempLocList;

    int ArrivedMsgs = TempDataList.count();
    msgs_1 += ArrivedMsgs;
    SpeedFilter->addPoint((float)(ArrivedMsgs)*1000.0/UPDATETIM_MS);

    QString NumOfMsgs;
    NumOfMsgs.sprintf("%d",msgs_1);
    ui->MsgCounter->display(NumOfMsgs);


    if(FreqDivFlipFlop > 5)
    {
        FreqDivFlipFlop = 0;
        ui->lcdNumberMsgsperSec->display(SpeedFilter->getPoint());
    }
    QModelIndex vis = ui->tableView->indexAt(QPoint(1,1));



    int msgsadded = TempDataList.count();
    QModelIndex index1;
    if(TempDataList.count())
    {
        index1 = TraceModel->index(0, 0, QModelIndex());
        TraceModel->insertRows(0, TempDataList.count(), (const QModelIndex &)index1);

        for(int i = 0; TempDataList.count() ; i ++)
        {
            index1 = TraceModel->index(i, 0, QModelIndex());
            _CANMsg Msg = TempDataList.takeLast();
            TraceModel->setData(index1, &Msg,Qt::EditRole);
        }
    }

    if(MaxTraceListLenght != 0 && TraceModel->rowCount(QModelIndex()) > MaxTraceListLenght)
    {
        int cnt = TraceModel->rowCount(QModelIndex());
        TraceModel->removeRows(MaxTraceListLenght ,cnt-MaxTraceListLenght, QModelIndex());
    }

    if(vis.isValid())
        ui->tableView->scrollTo(vis,QAbstractItemView::PositionAtTop);
}



void MainWindow::closeEvent( QCloseEvent *e )
{
    //stop first

    //stop the write thread if running    
    ui->checkBoxSendMsg->setChecked(false);


    if(rt->isRunning() || wt->isRunning())
    {
        on_actionStop_triggered();
    }

    //Delete alle Graphic Windows
    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        if(GraphWnd[i] != NULL)
            delete GraphWnd[i];

    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        if(GraphWnd[i] != NULL)
            delete DbgTerminal[i];

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
                connect(this, SIGNAL(newMessage(_CANMsg , int)), GraphWnd[i], SLOT(newMessage(_CANMsg ,int)));
                connect(periodicTimer, SIGNAL(timeout()), GraphWnd[i], SLOT(MainTimerSlot()));
                connect(this, SIGNAL(ClearAll()), GraphWnd[i], SLOT(ClearAll()));
                connect(GraphWnd[i], SIGNAL(destroyed(QObject*)), this, SLOT(SateliteDestroyed(QObject *)));
                connect(ConfDlg, SIGNAL(configChanged(__config)),  GraphWnd[i], SLOT(configChanged(__config)));
                ConfDlg->informOthers();
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

                connect(this, SIGNAL(newMessage(_CANMsg , int)), ObserverWnd[i], SLOT(newMessage(_CANMsg ,int)));
                connect(periodicTimer, SIGNAL(timeout()), ObserverWnd[i], SLOT(MainTimerSlot()));
                connect(this, SIGNAL(ClearAll()), ObserverWnd[i], SLOT(ClearAll()));
                connect(ObserverWnd[i], SIGNAL(destroyed(QObject*)), this, SLOT(SateliteDestroyed(QObject *)));
                connect(ConfDlg, SIGNAL(configChanged(__config)),  ObserverWnd[i], SLOT(configChanged(__config)));
                ConfDlg->informOthers();
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
    emit ClearAll();
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
    if((NULL == Args->parseParameter("-ss")) && (NULL == Args->parseParameter("-su")))
        startForks();
    if(rt->isConfigured())
    {
        ui->MsgCounter->setEnabled(true);

        rt->start();
        if(rt->thread() == this->thread())
            rt->moveToThread(rt);

        if(ui->checkBoxSendMsg->isChecked())
        {
            wt->start(QThread::TimeCriticalPriority);
            wt->moveToThread(wt);
        }

        periodicTimer->start(UPDATETIM_MS);

        //disable MenuItems
        QList <QAction*> act = ui->menuFile->actions();
        for(int i = 0; i < act.count() ; i ++)
        {
            if(act.at(i)->text() == QString("Start"))
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

    ui->MsgCounter->setEnabled(false);

    if((NULL == Args->parseParameter("-ss")) && (NULL == Args->parseParameter("-su")))
        stopForks();

    //enable Menu Items
    QList <QAction*> act = ui->menuFile->actions();
    for(int i = 0; i < act.count() ; i ++)
    {
        if(act.at(i)->text() == QString("Start"))
            act.at(i)->setEnabled(true);
    }
    ui->menuFile_2->setEnabled(true);
    emit StopCapture();
}

void MainWindow::NoMem(void)
{
    on_actionStop_triggered();

    ErrorDialog *ed = new ErrorDialog;
    ed->SetErrorMessage("Can not write to disk. Disabling.");

    ed->setModal(true);
    ed->exec();
    delete ed;

    return;
}


void MainWindow::on_actionClose_triggered()
{
    delete this;
}


void MainWindow::on_actionClear_triggered()
{
    msgs_1 = 0;
    emit ClearAll();
    delete TraceModel;
    QStringList *list = new QStringList();
    list->append(QString("ID"));
    list->append(QString("Data"));
    list->append(QString("Time"));
    TraceModel = new RawDataModel(list);
    delete list;
    ui->MsgCounter->display(0);
    ui->tableView->setModel(TraceModel);
    rt->MsgBuf->ClearAll();

    if((NULL == Args->parseParameter("-ss")) && (NULL == Args->parseParameter("-su")))
    {
        clearForks();
    }
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

    for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        if(DbgTerminal[i] == Obj)
            DbgTerminal[i] = NULL;
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
        //Store the dbname
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

        for( char i = 0 ; i < MAX_GRAPH_WINDOWS ; i++ )
        {
            if(DbgTerminal[(int)i] == NULL)
            {
                ofs.put(i);       //store the number of grph windows
                break;
            }
        }

        for( int i = 0 ; i < MAX_GRAPH_WINDOWS ; i++ )
        {
            if(DbgTerminal[i] == NULL)
                break;
            (*DbgTerminal[i]) >> ofs;
        }

        (*ConfDlg) >> ofs;


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

        //load the DebugWindows
        char numofdebugwnds;
        ifs.get(numofdebugwnds);

        for(int i = 0 ; i < numofdebugwnds ; i++ )
        {
            on_actionDebug_Terminal_triggered();
            (*DbgTerminal[i]) << ifs;
        }

        (*ConfDlg) << ifs;

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
    ConfDlg->show();
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



void MainWindow::on_actionDebug_Terminal_triggered()
{
    if(DB)
    {
        for(int i=0;i < MAX_GRAPH_WINDOWS;i++)
        {
            if(DbgTerminal[i] == NULL)
            {
                DbgTerminal[i] = new DebugTerminal(this, CANSignals);
                DbgTerminal[i]->setWindowTitle("Debug Terminal");
                DbgTerminal[i]->move(this->pos().x()+this->geometry().width(), this->pos().y());
                DbgTerminal[i]->show();
                connect(this, SIGNAL(StopCapture()), DbgTerminal[i], SLOT(StopCapture()));
                connect(this, SIGNAL(newMessage(_CANMsg , int)), DbgTerminal[i], SLOT(newMessage(_CANMsg ,int)));
                connect(periodicTimer, SIGNAL(timeout()), DbgTerminal[i], SLOT(MainTimerSlot()));
                connect(this, SIGNAL(ClearAll()), DbgTerminal[i], SLOT(ClearAll()));
                connect(DbgTerminal[i], SIGNAL(destroyed(QObject*)), this, SLOT(SateliteDestroyed(QObject *)));
                ConfDlg->informOthers();
                return;
            }
        }
    }
    return;
}


void MainWindow::on_ClearButton_clicked()
{
    on_actionClear_triggered();
}

void MainWindow::on_RunButton_clicked()
{
    DevDlg->setModal(true);
    DevDlg->exec();
    DevDlg->accept();
}

void MainWindow::configChanged(__config cfg)
{
    MaxTraceListLenght = cfg.MainMemByte/sizeof(_CANMsg);
}

void MainWindow::setArgs(cpparglib *argsa)
{
    Args = argsa;
    char *sec = Args->parseParameterForValue("-ss");
    char *usec = Args->parseParameterForValue("-su");

    if(sec == NULL || usec == NULL)
        return;
    struct timeval tv;
    tv.tv_sec = atoi(sec);
    tv.tv_usec = atoi(usec);
    rt->setStartTime(tv);
    open_queues();
    if(queueTimer != NULL)
        delete queueTimer;
    queueTimer = new QTimer( );
    connect(queueTimer, SIGNAL(timeout()), this, SLOT(queueTimerUpdate()));
    queueTimer->setInterval(10);
    queueTimer->start();
}

int MainWindow::create_queues(void)
{
    char QueueName[256];
    struct mq_attr attr;

    memset((void*)&attr, 0, sizeof(struct mq_attr));
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = 1;
    sprintf(QueueName, "/QCANCMD");
    CommandQueue = mq_open(QueueName, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &attr);
    return 0;
}

int MainWindow::open_queues(void)
{
    char QueueName[256];
    struct mq_attr attr;

    memset((void*)&attr, 0, sizeof(struct mq_attr));
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = 1;
    sprintf(QueueName, "/QCANCMD");
    CommandQueue = mq_open(QueueName, O_RDONLY, S_IWUSR | S_IRUSR, &attr);
    return 0;
}

int MainWindow::startForks(void)
{
    struct timespec timesp;
    timesp.tv_sec = 0;
    timesp.tv_nsec = 0;
    int ret = mq_timedsend(CommandQueue, "s", 1, 0, &timesp);

    char *sterr = strerror(errno);
    QString Err(sterr);
    return 0;

}

int MainWindow::stopForks(void)
{

    struct timespec timesp;
    timesp.tv_sec = 0;
    timesp.tv_nsec = 0;
    int ret = mq_timedsend(CommandQueue, "e", 1, 0, &timesp);

    char *sterr = strerror(errno);
    QString Err(sterr);
    return 0;
}

int MainWindow::clearForks(void)
{

    struct timespec timesp;
    timesp.tv_sec = 0;
    timesp.tv_nsec = 0;
    int ret = mq_timedsend(CommandQueue, "c", 1, 0, &timesp);

    char *sterr = strerror(errno);
    QString Err(sterr);
    return 0;
}

void MainWindow::queueTimerUpdate(void)
{
    if((NULL != Args->parseParameter("-ss")) && (NULL != Args->parseParameter("-su")))
    {

        struct timespec timesp;
        timesp.tv_sec = 0;
        timesp.tv_nsec = 0;
        char Cmd[2];

        int read = mq_timedreceive(CommandQueue, Cmd, 1, NULL, &timesp);
        if(read)
        {
            if(Cmd[0] == 's')
            {
                on_actionStart_triggered();
                on_actionClear_triggered();
            }
            if(Cmd[0] == 'e')
            {
                this->on_actionStop_triggered();
            }

            if(Cmd[0] == 'c')
            {
                on_actionClear_triggered();
            }

        }
    }
}

void MainWindow::on_actionFork_triggered()
{
    //int spawn (char* program, char** arg_list)
    create_queues();

    char *arg_list[5];


    struct timeval val;
    val = rt->getStartTime();

    arg_list[0] = (char*)malloc(32);
    arg_list[1] = (char*)malloc(32);
    arg_list[2] = (char*)malloc(32);
    arg_list[3] = (char*)malloc(32);
    arg_list[4] = (char*)malloc(32);

    sprintf(arg_list[0], "QCanObserver");
    sprintf(arg_list[1], "-ss");
    sprintf(arg_list[2], "%d" , val.tv_sec);
    sprintf(arg_list[3], "-su");
    sprintf(arg_list[4], "%d" , val.tv_usec);
    arg_list[5] = NULL;

    pid_t child_pid;

    /* Duplicate this process.  */
    child_pid = fork ();
    if (child_pid != 0)
        return;
    else
    {
        /* Now execute PROGRAM, searching for it in the path.  */
        execvp ("./QCanObserver", (char**)arg_list);
        /* The execvp function returns only if an error occurs.  */
        fprintf (stderr, "an error occurred in execvp\n");
        abort ();
    }

}
