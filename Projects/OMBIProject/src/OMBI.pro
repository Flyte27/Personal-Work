#-------------------------------------------------
#
# Project created by QtCreator 2013-10-22T10:49:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MunicipalityCity
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    ombidata.cpp \
    servicearea.cpp \
    servicemeasure.cpp \
    welcomescreen.cpp \
    mainscreen.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    ombidata.h \
    servicearea.h \
    servicemeasure.h \
    welcomescreen.h \
    mainscreen.h

FORMS    += mainwindow.ui \
    mainscreen.ui \
    welcomescreen.ui

RESOURCES   += Resources.qrc
