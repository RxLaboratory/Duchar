#-------------------------------------------------
#
# Project created by QtCreator 2017-04-17T13:06:27
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DuChar
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
    duqf-app/app-style.cpp \
    duqf-widgets/aboutdialog.cpp \
    duqf-widgets/appearancesettingswidget.cpp \
    duqf-widgets/settingswidget.cpp \
    duqf-widgets/toolbarspacer.cpp \
        mainwindow.cpp \
	button.cpp \
    rainboxui.cpp \
    buttonmanager.cpp

HEADERS  += mainwindow.h \
    button.h \
    duqf-app/app-style.h \
    duqf-app/app-utils.h \
    duqf-app/app-version.h \
    duqf-widgets/aboutdialog.h \
    duqf-widgets/appearancesettingswidget.h \
    duqf-widgets/settingswidget.h \
    duqf-widgets/toolbarspacer.h \
    rainboxui.h \
    buttonmanager.h

FORMS    += mainwindow.ui \
    buttonmanager.ui \
    buttonmanager.ui \
    buttonmanager.ui \
    duqf-widgets/aboutdialog.ui \
    duqf-widgets/appearancesettingswidget.ui \
    duqf-widgets/settingswidget.ui \
    duqf-widgets/toolbarspacer.ui

RESOURCES += \
    resources.qrc \
    duqf_resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# OS Specific configurations
win* {
    # Add version and other metadata
    DISTFILES += app.rc
    RC_FILE = app.rc
    !build_pass:touch($$RC_FILE, duqf-app/app-version.h)
    # Enable console output
    CONFIG += console
    msvc: LIBS += -luser32
} else:unix {
    # Fix issue with c++ version used to compile Qt in some distros (Ubuntu) with Qt <= 5.12.
    # Need to check the version of c++ used with distros providing Qt > 5.12
    equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 13)
    {
        QMAKE_CXXFLAGS += "-fno-sized-deallocation" \
            "-Wdeprecated-copy" \ # Ignore annoying errors on older versions of Qt
            "-Wdeprecated" # Ignore annoying errors on older versions of Qt
    }
} else:macx {
    # Just in case...
}

