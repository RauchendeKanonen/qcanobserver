# -------------------------------------------------
# Project created by QtCreator 2009-11-10T14:30:12
# -------------------------------------------------
TARGET = QCanObserver
TEMPLATE = app

# CONFIG(release, debug|release)
# CONFIG += release
CONFIG += debug
SOURCES += main.cpp \
    mainwindow.cpp \
    readthread.cpp \
    DevDialog.cpp \
    errordialog.cpp \
    messagebufferinterface.cpp \
    stringlistmodel.cpp \
    FilterDialog.cpp \
    filterdef.cpp \
    graphicwindow.cpp \
    processdatabase.cpp \
    observerdialog.cpp \
    aboutbox.cpp \
    sendmsgdialog.cpp \
    msgdefdialog.cpp \
    cansignal.cpp \
    cansignalcollection.cpp \
    extrect.cpp \
    configdialog.cpp \
    signalselectordialog.cpp \
    specialeventdialog.cpp \
    writethread.cpp \
    qcancostumplotcurve.cpp \
    averagefilter.cpp \
    debugterminal.cpp \
    qconsole.cpp \
    rawdatamodel.cpp \
    cpparglib.cpp
HEADERS += mainwindow.h \
    readthread.h \
    DevDialog.h \
    errordialog.h \
    messagebufferinterface.h \
    stringlistmodel.h \
    FilterDialog.h \
    filterdef.h \
    graphicwindow.h \
    processdatabase.h \
    observerdialog.h \
    aboutbox.h \
    sendmsgdialog.h \
    msgdefdialog.h \
    cansignal.h \
    cansignalcollection.h \
    config.h \
    obscan.h \
    extrect.h \
    configdialog.h \
    signalselectordialog.h \
    specialeventdialog.h \
    writethread.h \
    DeviceLib/candevice.h \
    qcancostumplotcurve.h \
    averagefilter.h \
    debugterminal.h \
    qconsole.h \
    rawdatamodel.h \
    cpparglib.h
FORMS += mainwindow.ui \
    DevDialog.ui \
    errordialog.ui \
    FilterDialog.ui \
    FilterDialog.ui \
    filterdef.ui \
    filterdef.ui \
    graphicwindow.ui \
    observerdialog.ui \
    aboutbox.ui \
    sendmsgdialog.ui \
    msgdefdialog.ui \
    configdialog.ui \
    signalselectordialog.ui \
    specialeventdialog.ui \
    debugterminal.ui
unix { 
    INCLUDEPATH += /usr/include/qwt5/
    LIBS += -L/usr/lib/ \
        -lqwt
}
win32 { 
    SOURCES += wingettimeofday.cpp
    HEADERS += wingettimeofday.h
    INCLUDEPATH += c:\Qwt-5.2.0\include
    CONFIG(release, debug|release):LIBS += C:\Qwt-5.2.0\lib\libqwt5.a
    CONFIG(debug, debug|release):LIBS += C:\Qwt-5.2.0\lib\libqwtd5.a
}
QT += xml
include(modeltest/modeltest.pri)
