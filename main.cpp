#define QT_THREAD_SUPPORT
#include <QtGui/QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow *MainWin = new MainWindow;
    MainWin->show();

    a.exec();
    delete MainWin;
    return 1;
}
