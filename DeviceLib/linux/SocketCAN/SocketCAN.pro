# -------------------------------------------------
# Project created by QtCreator 2009-12-06T15:27:35
# -------------------------------------------------
#QT -= gui
CONFIG += debug
TARGET = SocketCAN
TEMPLATE = lib
QMAKE_CXXFLAGS += -DPF_CAN=29 \
    -DAF_CAN=PF_CAN
INCLUDEPATH += /usr/src/linux/include

# DEFINES += PCANINTERFACE_LIBRARY
SOURCES += SocketCAN.cpp \
    confdialog.cpp
HEADERS += ../../candevice.h \
    confdialog.h
FORMS += confdialog.ui
