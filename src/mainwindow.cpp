#include "mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    updateStack = false;

    //appearance
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow);//subwindow removes the task bar item
    QFile cssFile(":/styles/default");
    cssFile.open(QFile::ReadOnly);
    QString css = cssFile.readAll();
    cssFile.close();
    qApp->setStyleSheet(css);

    //system tray
    trayMenu = new QMenu("Duchar",this);
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        trayIcon = new QSystemTrayIcon(QIcon(":/icons/app"),this);
        trayMenu->addAction(actionClear);
        trayMenu->addAction(actionManage_buttons);
        trayMenu->addAction(actionSettings);
        trayMenu->addAction(actionShow_Hide);
        trayMenu->addAction(actionQuit);
        actionSettings->setEnabled(false);
        actionManage_buttons->setEnabled(false);
        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();
    }

    //toolBar
    toolBar->addAction(actionClear);
    toolBar->addAction(actionManage_buttons);
    toolBar->addAction(actionSettings);
    toolBar->addAction(actionShow_Hide);
    toolBar->addAction(actionQuit);
    QWidget *spacer = new QWidget(toolBar);
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    actionSpacer = toolBar->insertWidget(actionClear,spacer);

    //add buttons
    addButton("É","É");
    addButton("È","È");
    addButton("Ù","Ù");
    addButton("À","À");
    addButton("Ç","Ç");
    addButton("•","•");

    //drag window
    toolBar->installEventFilter(this);

    //clipboard
    clipboard = QGuiApplication::clipboard();

    timer = new QTimer(this);
    timer->setSingleShot(true);

    //connections
    mapEvents();

    updateStack = true;
}

void MainWindow::mapEvents()
{
    //clipboard
    connect(clipboard,SIGNAL(dataChanged()),this,SLOT(clipboardChanged()));

    //tray icon
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(actionShow_Hide,SIGNAL(triggered()),this,SLOT(showHide()));
    connect(actionQuit,SIGNAL(triggered()),qApp,SLOT(quit()));
    connect(actionClear,SIGNAL(triggered()),stack,SLOT(clear()));

    //timer
    connect(timer,SIGNAL(timeout()),this,SLOT(resumeUpdateStack()));
}

void MainWindow::clipboardChanged()
{
    if (!updateStack) return;

    if (clipboard->text() == "") return;
    if (stack->count() > 0)
    {
        if (clipboard->text() == stack->item(0)->data(Qt::UserRole).toString()) return;
    }


    QString clipboardText = clipboard->text();

    if (clipboardText.length() > 33)
    {
        qDebug() << "raccourci";
        clipboardText = clipboardText.left(30) + "...";
    }

    QListWidgetItem *item = new QListWidgetItem(clipboardText);
    item->setData(Qt::UserRole,clipboard->text());
    item->setToolTip(clipboard->text());
    stack->insertItem(0,item);
}

void MainWindow::addButton(QString label,QString data)
{
    Button *btn = new Button(this);
    btn->setText(label);
    btn->setData(data);
    toolBar->insertAction(actionSpacer,btn);

    trayMenu->insertAction(actionClear,btn);

    connect(btn,SIGNAL(pauseUpdateStack(int)),this,SLOT(pauseUpdateStack(int)));
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

void MainWindow::pauseUpdateStack(int duration)
{
    timer->setInterval(duration);
    updateStack = false;
    timer->start();
}

void MainWindow::resumeUpdateStack()
{
    updateStack = true;
}

void MainWindow::on_stack_itemClicked(QListWidgetItem *item)
{
    pauseUpdateStack();
    clipboard->setText(item->data(Qt::UserRole).toString());
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
