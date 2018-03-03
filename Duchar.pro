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
	button.cpp \
    frameless.cpp \
    rainboxui.cpp \
    buttonmanager.cpp

HEADERS  += mainwindow.h \
    button.h \
    frameless.h \
    rainboxui.h \
    buttonmanager.h

FORMS    += mainwindow.ui \
    buttonmanager.ui \
    buttonmanager.ui \
    src/buttonmanager.ui

RESOURCES += \
    src/resources.qrc
