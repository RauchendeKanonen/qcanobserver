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



using namespace std;



namespace Ui
{
    class MainWindow;
}

typedef struct
{
    QList<QStandardItem*> * row;
}TraceItems;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent( QCloseEvent *e );
    QList<CalRule*> *RuleList;
private:
    StringListModel *TraceModel;
    Ui::MainWindow *ui;
    ReadThread *rt;
    FilterDialog *FilterDlg;
    struct timeval tv_1;
    QTimer *periodicTimer;
    ProcessDataBase *DB;
    GraphicWindow *GraphWnd[MAX_GRAPH_WINDOWS];
    int MsgCounter;

signals:
    void setDev(QString PathArg, int BaudRate, int MsgType);
    void QuitThread();
    void ClearAll();

public slots:
    void newMessage(CANMsgandTimeStruct *, int);
    void periodicUpdate(void);


private slots:
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
