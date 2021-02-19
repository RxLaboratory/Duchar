#include "mainwindow.h"
#include "buttonmanager.h"
#include "duqf-app/app-utils.h"
#include <QApplication>


#ifdef Q_OS_WIN
#include "windows.h"
#endif

int main(int argc, char *argv[])
{
    DuApplication a(argc, argv);

    // process CLI arguments
    if ( duqf_processArgs(argc, argv) ) return 0;

    ButtonManager *buttonManager = new ButtonManager();

#ifndef Q_OS_LINUX
    FrameLessWindow ff(buttonManager);
#endif

    MainWindow *w = new MainWindow(buttonManager);
#ifndef Q_OS_LINUX
    FrameLessWindow f(w);
#endif
    w->show();

    return a.exec();
}
