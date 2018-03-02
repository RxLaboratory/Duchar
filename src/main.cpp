#include "mainwindow.h"
#include "frameless.h"
#include "buttonmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ButtonManager *buttonManager = new ButtonManager();
    FrameLess ff(buttonManager);
    MainWindow *w = new MainWindow(buttonManager);
    FrameLess f(w);
    w->show();

    return a.exec();
}
