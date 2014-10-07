#-------------------------------------------------
#
# Project created by QtCreator 2013-09-18T14:36:49
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Asn1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    weathermodel.cpp

HEADERS  += mainwindow.h \
    weathermodel.h

FORMS    += mainwindow.ui

RESOURCES += \
    seasons2.qrc
