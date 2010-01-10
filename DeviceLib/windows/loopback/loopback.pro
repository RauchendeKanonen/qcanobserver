# -------------------------------------------------
# Project created by QtCreator 2009-12-11T06:30:43
# -------------------------------------------------
QT -= gui
CONFIG += shared
TARGET = loopback
TEMPLATE = lib
DEFINES += PCANINTERFACE_LIBRARY
SOURCES += loopback.cpp \
    ../../../wingettimeofday.cpp

HEADERS += ../../CANDevice.h \
    ../../../wingettimeofday.h
