#-------------------------------------------------
#
# Project created by QtCreator 2017-04-17T13:06:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Duchar

TEMPLATE = app

VPATH += ./src


SOURCES += main.cpp\
        mainwindow.cpp \
	button.cpp

HEADERS  += mainwindow.h \
    button.h

FORMS    += mainwindow.ui

RESOURCES += \
    src/resources.qrc
