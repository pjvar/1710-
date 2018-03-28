#-------------------------------------------------
#
#2018å¹?3æœ?9æ—?12:55:23
#
#-------------------------------------------------

QT       += core gui network
QT += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xm2
TEMPLATE = app


SOURCES += main.cpp\
        win.cpp \
    player.cpp \
    threadUpData.cpp \
    threadNet.cpp

HEADERS  += win.h \
    player.h \
    threadNet.h

FORMS    += win.ui

RESOURCES += \
    background.qrc
QMAKE_CXXFLAGS+=-std=c++0x
