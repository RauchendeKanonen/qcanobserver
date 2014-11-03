# -------------------------------------------------
# Project created by QtCreator 2009-11-10T14:30:12
# -------------------------------------------------
TARGET = QCanObserver
TEMPLATE = app
# CONFIG += QWT6
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
    averagefilter.cpp \
    debugterminal.cpp \
    qconsole.cpp \
    rawdatamodel.cpp \
    cpparglib.cpp \
    suggestiondialog.cpp
HEADERS += mainwindow.h \
    readthread.h \
    DevDialog.h \
    errordialog.h \
    messagebufferinterface.h \
    stringlistmodel.h \
    FilterDialog.h \
    filterdef.h \
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
    averagefilter.h \
    debugterminal.h \
    qconsole.h \
    rawdatamodel.h \
    cpparglib.h \
    suggestiondialog.h
FORMS += mainwindow.ui \
    DevDialog.ui \
    errordialog.ui \
    FilterDialog.ui \
    FilterDialog.ui \
    filterdef.ui \
    filterdef.ui \
    observerdialog.ui \
    aboutbox.ui \
    sendmsgdialog.ui \
    msgdefdialog.ui \
    configdialog.ui \
    signalselectordialog.ui \
    specialeventdialog.ui \
    debugterminal.ui \
    suggestiondialog.ui

QWT6 {
    DEFINES += QWT6
    FORMS += graphicwindowqwt6.ui
    HEADERS += graphicwindowqwt6.h
    SOURCES += graphicwindowqwt6.cpp
    INCLUDEPATH += /usr/include/qwt6/
    LIBS += -L/usr/lib/ \
        -lqwt6
}
else {
    FORMS += graphicwindow.ui
    HEADERS += graphicwindow.h \
    qcancostumplotcurve.h
    SOURCES += graphicwindow.cpp \
    qcancostumplotcurve.cpp
    INCLUDEPATH += /usr/include/qwt5/
    LIBS += -L/usr/lib/ \
        -lqwt
}


QT += xml
include(modeltest/modeltest.pri)
LIBS += -ldl
