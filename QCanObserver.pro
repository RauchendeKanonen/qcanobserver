# -------------------------------------------------
# Project created by QtCreator 2009-11-10T14:30:12
# -------------------------------------------------
TARGET = QCanObserver
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    readthread.cpp \
    DevDialog.cpp \
    candevice.cpp \
    errordialog.cpp \
    messagebufferinterface.cpp \
    stringlistmodel.cpp \
    FilterDialog.cpp \
    filterdef.cpp \
    graphicwindow.cpp \
    processdatabase.cpp \
    calrule.cpp \
    candataitemselector.cpp
HEADERS += mainwindow.h \
    readthread.h \
    DevDialog.h \
    candevice.h \
    errordialog.h \
    messagebufferinterface.h \
    stringlistmodel.h \
    FilterDialog.h \
    filterdef.h \
    graphicwindow.h \
    processdatabase.h \
    calrule.h \
    candataitemselector.h
FORMS += mainwindow.ui \
    DevDialog.ui \
    errordialog.ui \
    FilterDialog.ui \
    FilterDialog.ui \
    filterdef.ui \
    filterdef.ui \
    graphicwindow.ui \
    candataitemselector.ui
INCLUDEPATH = /usr/include/qwt5/
LIBS += -L/usr/lib/ \
    -lqwt \
    -lpcan
QT += xml
