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


using namespace std;



namespace Ui
{
    class MainWindow;
}





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent( QCloseEvent *e );
    QList<CanFrameRuleSet*> *RuleList;
private:
    StringListModel *TraceModel;
    Ui::MainWindow *ui;
    ReadThread *rt;
    FilterDialog *FilterDlg;
    struct timeval tv_1;
    QTimer *periodicTimer;
    ProcessDataBase *DB;
    GraphicWindow *GraphWnd[MAX_GRAPH_WINDOWS];
    ObserverDialog *ObserverWnd [MAX_GRAPH_WINDOWS];
    int MsgCounter;
    SendMsgDialog *SendMsgDlg;

signals:
    void setDev(QString PathArg, int BaudRate, int MsgType);
    void QuitThread();
    void ClearAll();

public slots:
    void newMessage(CANMsgandTimeStruct *, int);
    void periodicUpdate(void);


private slots:
    void on_checkBox_2_toggled(bool checked);
    void on_actionSendDialog_triggered();
    void on_actionAbout_triggered();
    void on_actionObserverWindow_triggered();
    void on_tableView_clicked(QModelIndex index);
    void on_actionDatabase_triggered();
    void on_actionGraphicWindow_triggered();
    void on_MainWindow_destroyed();
    void on_actionClose_triggered();
    void on_checkBox_clicked(bool checked);
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_actionClear_triggered();
    void on_actionStop_triggered();
    void on_actionStart_triggered();
    void on_actionDevice_triggered();

};

#endif // MAINWINDOW_H
