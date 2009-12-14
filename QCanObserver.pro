# -------------------------------------------------
# Project created by QtCreator 2009-11-10T14:30:12
# -------------------------------------------------
TARGET = QCanObserver
TEMPLATE = app
#CONFIG(release, debug|release)
#CONFIG -= debug
#CONFIG += release
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
    candataitemselector.cpp \
    observerdialog.cpp \
    aboutbox.cpp \
    sendmsgdialog.cpp \
    msgdefdialog.cpp \
    cansignal.cpp \
    cansignalcollection.cpp
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
    candataitemselector.h \
    observerdialog.h \
    aboutbox.h \
    sendmsgdialog.h \
    msgdefdialog.h \
    cansignal.h \
    cansignalcollection.h \
    can.h \
    config.h
FORMS += mainwindow.ui \
    DevDialog.ui \
    errordialog.ui \
    FilterDialog.ui \
    FilterDialog.ui \
    filterdef.ui \
    filterdef.ui \
    graphicwindow.ui \
    candataitemselector.ui \
    observerdialog.ui \
    aboutbox.ui \
    sendmsgdialog.ui \
    msgdefdialog.ui

unix{
    INCLUDEPATH += /usr/include/qwt5/
    LIBS += -L/usr/lib/ \
    -lqwt \
    -lpcan
}

win32{
    INCLUDEPATH += c:\Qwt-5.2.1-svn\include

    CONFIG(release, debug|release){
        LIBS += C:\Qwt-5.2.1-svn\lib\libqwt5.a
    }

    CONFIG(debug, debug|release){
        LIBS += C:\Qwt-5.2.1-svn\lib\libqwtd5.a
    }
}



QT += xml
include(modeltest/modeltest.pri)
