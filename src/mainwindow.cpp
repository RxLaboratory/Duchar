#include "mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(ButtonManager *bm, QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    //button manager
    buttonManager = bm;

    updateStack = false;

    //appearance
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow);//subwindow removes the task bar item
    //set style
    updateCSS(":/styles/default");
    //remove right click on toolbar
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //system tray
    trayMenu = new QMenu("Duchar",this);
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        trayIcon = new QSystemTrayIcon(QIcon(":/icons/app"),this);
        trayMenu->addAction(actionShow_Hide);
        trayMenu->addAction(actionQuit);
        trayMenu->setToolTipsVisible(true);
        actionSettings->setEnabled(false);
        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();
    }

    //toolBar
    QWidget *spacer = new QWidget(toolBar);
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    actionSpacer = toolBar->insertWidget(actionSettings,spacer);

    //add buttons
    loadButtons();

    //drag window
    toolBar->installEventFilter(this);

    //clipboard
    clipboard = QGuiApplication::clipboard();

    timer = new QTimer(this);
    timer->setSingleShot(true);

    //connections
    mapEvents();

    updateStack = true;

    on_actionClipboard_History_triggered(true);
}

void MainWindow::mapEvents()
{
    //clipboard
    connect(clipboard,SIGNAL(dataChanged()),this,SLOT(clipboardChanged()));

    //tray icon
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));

    //timer
    connect(timer,SIGNAL(timeout()),this,SLOT(resumeUpdateStack()));

    //buttonManager
    connect(buttonManager,SIGNAL(buttonMoved(int,int)),this,SLOT(onButtonMove(int,int)));

}

void MainWindow::loadButtons()
{
    QSettings settings;
    int size = settings.beginReadArray("buttons");
    for(int i = 0; i < size; i++)
    {
        settings.setArrayIndex(i);
        addButton(settings.value("name").toString(),settings.value("data").toString());
    }
    settings.endArray();
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionManage_buttons_triggered(bool checked)
{
    actionManage_buttons->setChecked(true);
    actionClipboard_History->setChecked(false);
    actionSettings->setChecked(false);
    stackedWidget->setCurrentIndex(1);

    actionAdd_to_favorites->setVisible(false);
    actionClear->setVisible(false);
    actionRemove->setVisible(true);

    buttonManager->show();
}

void MainWindow::on_actionClipboard_History_triggered(bool checked)
{
    actionClipboard_History->setChecked(true);
    actionManage_buttons->setChecked(false);
    actionSettings->setChecked(false);
    stackedWidget->setCurrentIndex(0);

    actionAdd_to_favorites->setVisible(true);
    actionClear->setVisible(true);
    actionRemove->setVisible(false);
}

void MainWindow::on_actionSettings_triggered(bool checked)
{
    actionSettings->setChecked(true);
    actionManage_buttons->setChecked(false);
    actionClipboard_History->setChecked(false);
    stackedWidget->setCurrentIndex(2);

    actionAdd_to_favorites->setVisible(false);
    actionClear->setVisible(false);
    actionRemove->setVisible(false);
}


void MainWindow::on_actionShow_Hide_triggered()
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

void MainWindow::on_actionClear_triggered()
{
    stack->clear();
}

void MainWindow::on_actionAdd_to_favorites_triggered()
{
    if (stack->selectedItems().length() == 0) return;
    QListWidgetItem *item = stack->currentItem();
    QString name = item->text();
    addButton(name,item->data(Qt::UserRole).toString());
}

void MainWindow::onButtonMove(int from, int to)
{
    //TODO
}

void MainWindow::updateCSS(QString cssFileName)
{
    QStringList cssFiles(cssFileName);
    //check if there's a duchar file to include
    QFileInfo cssFileInfo(cssFileName);
    QString includeName = cssFileInfo.completeBaseName() + "-duchar";
    QString includePath = cssFileInfo.path() + "/" + includeName + ".css";
    QFile includeFile(includePath);
    includePath = cssFileInfo.path() + "/" + includeName;
    if (!includeFile.exists()) includeFile.setFileName(includePath);
    if (includeFile.exists())
    {
        cssFiles << includePath;
    }
    QString css = RainboxUI::loadCSS(cssFiles);
    qApp->setStyleSheet(css);
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
        clipboardText = clipboardText.left(30) + "...";
    }

    QListWidgetItem *item = new QListWidgetItem(clipboardText);
    item->setData(Qt::UserRole,clipboard->text());
    item->setToolTip(clipboard->text());
    item->setFlags (item->flags () | Qt::ItemIsEditable);
    stack->insertItem(0,item);
}

void MainWindow::addButton(QString label, QString data)
{
    Button *btn = new Button(this);
    btn->setText(label);
    btn->setData(data);
    btn->setToolTip(data);

    trayMenu->insertAction(actionAdd_to_favorites,btn);
    buttonManager->addButton(btn);

    buttons << btn;

    connect(btn,SIGNAL(pauseUpdateStack(int)),this,SLOT(pauseUpdateStack(int)));
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        on_actionShow_Hide_triggered();
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

