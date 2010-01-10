# -------------------------------------------------
# Project created by QtCreator 2009-12-11T06:30:43
# -------------------------------------------------
#QT -= gui
CONFIG += shared
TARGET = pcaninterface
TEMPLATE = lib
DEFINES += PCANINTERFACE_LIBRARY
SOURCES += pcanlib.cpp \
    ../../../wingettimeofday.cpp \
    confdialog.cpp
HEADERS += ../../CANDevice.h \
    ../../../wingettimeofday.h \
    confdialog.h
FORMS += confdialog.ui
