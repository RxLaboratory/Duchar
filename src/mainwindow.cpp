#include "mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    //appearance
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow);//subwindow removes the task bar item
    QFile cssFile(":/styles/default");
    cssFile.open(QFile::ReadOnly);
    QString css = cssFile.readAll();
    cssFile.close();
    qApp->setStyleSheet(css);

    //system tray
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        trayIcon = new QSystemTrayIcon(QIcon(":/icons/app"),this);
        QMenu *trayMenu = new QMenu("Duchar",this);
        trayMenu->addAction(actionManage_buttons);
        trayMenu->addAction(actionSettings);
        trayMenu->addAction(actionShow_Hide);
        trayMenu->addAction(actionQuit);
        actionSettings->setEnabled(false);
        actionManage_buttons->setEnabled(false);
        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();
    }

    //add buttons
    addButton("É","É");
    addButton("È","È");
    addButton("Ù","Ù");
    addButton("À","À");

    //drag window
    mover->installEventFilter(this);

    //connections
    mapEvents();
}

void MainWindow::mapEvents()
{
    //window buttons
    connect(btn_quit,SIGNAL(clicked()),qApp,SLOT(quit()));

    //tray icon
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(actionShow_Hide,SIGNAL(triggered()),this,SLOT(showHide()));
    connect(actionQuit,SIGNAL(triggered()),qApp,SLOT(quit()));
}

void MainWindow::addButton(QString label,QString data)
{
    Button *btn = new Button(this);
    btn->setText(label);
    btn->setData(data);
    mainLayout->insertWidget(1,btn);
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        showHide();
    }
}

void MainWindow::showHide()
{
    if (this->isVisible())
    {
        this->hide();
        actionShow_Hide->setIcon(QIcon(":/icons/show"));
        actionShow_Hide->setText("Show application");
    }
    else
    {
        this->show();
        actionShow_Hide->setIcon(QIcon(":/icons/hide"));
        actionShow_Hide->setText("Hide application");
    }
}

//EVENT FILTER

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
      QMouseEvent *mouseEvent = (QMouseEvent*)event;
      if (mouseEvent->button() == Qt::LeftButton)
      {
        moverClicked = true;
        dragPosition = mouseEvent->globalPos() - this->frameGeometry().topLeft();
        event->accept();
      }
      return true;
  }
  else if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = (QMouseEvent*)event;
    if (mouseEvent->buttons() & Qt::LeftButton && moverClicked)
    {
        if (this->isMaximized()) this->showNormal();
        this->move(mouseEvent->globalPos() - dragPosition);
        event->accept();
    }
    return true;
  }
  else if (event->type() == QEvent::MouseButtonRelease)
  {
      moverClicked = false;
      return true;
  }
  else
  {
      // standard event processing
      return QObject::eventFilter(obj, event);
  }
}

