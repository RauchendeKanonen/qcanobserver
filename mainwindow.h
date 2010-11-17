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
#include <QObject>
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "readthread.h"
#include "writethread.h"
#include "DevDialog.h"
#include <QStandardItemModel>
#include "messagebufferinterface.h"
#include <QItemSelection>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <QTimer>
#include <list>
#include "stringlistmodel.h"
#include "FilterDialog.h"
#include "graphicwindow.h"
#include "processdatabase.h"
#include "observerdialog.h"
#include "sendmsgdialog.h"
#include "cansignalcollection.h"
#include "extrect.h"
#include "specialeventdialog.h"
#include "obscan.h"
#include "averagefilter.h"
#include "debugterminal.h"
using namespace std;



namespace Ui
{
    class MainWindow;
}


#define UPDATETIM_MS 200


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent( QCloseEvent *e );



private:
    StringListModel *TraceModel;
    Ui::MainWindow *ui;

    ReadThread *rt;
    WriteThread *wt;
    AverageFilter *SpeedFilter;
    int FreqDivFlipFlop;
    CANSignalCollection *CANSignals;
    QColor black;
    QTimer *periodicTimer;
    ProcessDataBase *DB;
    struct timeval tv_1;
    unsigned int MainStringListLength;
    int msgs_1;
    struct timeval lasttime;

    void initSendMsgDlg(void);
    SendMsgDialog *SendMsgDlg;
    DevDialog *DevDlg;
    SpecialEventDialog *SpecEvtDlg;
    GraphicWindow *GraphWnd[MAX_GRAPH_WINDOWS];
    ObserverDialog *ObserverWnd [MAX_GRAPH_WINDOWS];
    FilterDialog *FilterDlg;
    DebugTerminal *DbgTerminal[MAX_GRAPH_WINDOWS];


    void initSatelites();
    int loadDatabase(QString File);
    int loadDefaultConfig();
    int SaveConfig(QString Filename);
    int loadConfig(QString FileName);

signals:
    void newMessage(_CANMsg , int);
    void setDev(void *ConfData, QString InterfaceLib, bool);
    void StopCapture();
    void ClearAll();


public slots:
    void addnewMessage(_CANMsg , int);
    void periodicUpdate(void);
    void SateliteDestroyed(QObject *);
    void DevIsConfigured(bool);



private slots:
    void on_RunButton_clicked();
    void on_ClearButton_clicked();
    void on_actionDebug_Terminal_triggered();
    void on_listLengtLineEdit_editingFinished();
    void on_checkBoxSendMsg_toggled(bool checked);
    void on_checkBoxFilters_toggled(bool checked);
    void on_checkBoxSpecEvtDlg_toggled(bool checked);
    void on_actionSave_Config_triggered();
    void on_actionConfiguration_triggered();
    void on_actionLoad_Config_triggered();
    void on_actionSave_Config_as_Default_triggered();
    void on_actionAbout_triggered();
    void on_actionObserverWindow_triggered();
    void on_actionDatabase_triggered();
    void on_actionGraphicWindow_triggered();
    void on_actionClose_triggered();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_actionClear_triggered();
    void on_actionStop_triggered();
    void on_actionStart_triggered();
    void on_actionDevice_triggered();

};

#endif // MAINWINDOW_H
