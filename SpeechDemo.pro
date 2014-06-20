#-------------------------------------------------
#
# Project created by QtCreator 2014-04-18T09:57:26
#

#-------------------------------------------------

QT       += core gui multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpeechDemo
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    speechinput.cpp \
    protocol.cpp \
    socketdemo.cpp

HEADERS  += widget.h \
    speechinput.h \
    protocol.h \
    socketdemo.h

RESOURCES += \
    speechRc.qrc
