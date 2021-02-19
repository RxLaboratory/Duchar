#include "mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(ButtonManager *bm, QWidget *parent) :
    QMainWindow(parent)
{
    // Build the form
    setupUi(this);
    // Add default stuff
    duqf_initUi();

    //button manager
    buttonManager = bm;

    updateStack = false;

    //appearance
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::SubWindow);//subwindow removes the task bar item

    //system tray
    trayMenu = new QMenu("Duchar",this);
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        trayIcon = new QSystemTrayIcon(QIcon(":/icons/tray"),this);
        trayMenu->addAction(actionShow_Hide);
        trayMenu->addAction(actionQuit);
        trayMenu->setToolTipsVisible(true);
        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();
    }

    //toolBar
    ToolBarSpacer *spacer = new ToolBarSpacer(mainToolBar);
    mainToolBar->insertWidget(actionShow_Hide,spacer);

    //add buttons
    loadButtons();

    //clipboard
    clipboard = QGuiApplication::clipboard();

    timer = new QTimer(this);
    timer->setSingleShot(true);

    //connections
    mapEvents();

    updateStack = true;

    on_actionClipboard_History_triggered(true);

    // Set style
    duqf_setStyle();
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

void MainWindow::duqf_initUi()
{
    // ===== ABOUT ========
    duqf_aboutDialog = new AboutDialog();

    // ===== TOOLBAR ======

    // remove right click on toolbar
    mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    // set frameless
#ifndef Q_OS_MAC
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif
    //drag window
    duqf_toolBarClicked = false;
    mainToolBar->installEventFilter(this);

    // ==== TOOLBAR BUTTONS
    /*mainToolBar->addWidget(new ToolBarSpacer());
    title = new QLabel(STR_FILEDESCRIPTION);
    mainToolBar->addWidget(title);
    //minimize
#ifndef Q_OS_MAC
    QToolButton *minimizeButton = new QToolButton();
    minimizeButton->setIcon(QIcon(":/icons/minimize"));
    minimizeButton->setObjectName("windowButton");
    mainToolBar->addWidget(minimizeButton);
#endif
    //maximize
    duqf_maximizeButton = new QToolButton(this);
    duqf_maximizeButton->setIcon(QIcon(":/icons/maximize"));
    duqf_maximizeButton->setObjectName("windowButton");
    mainToolBar->addWidget(duqf_maximizeButton);
    //quit
    QToolButton *quitButton = new QToolButton(this);
    quitButton->setIcon(QIcon(":/icons/quit"));
    quitButton->setObjectName("windowButton");
    mainToolBar->addWidget(quitButton);*/

    // ===== STATUSBAR ======

    // version in statusbar
    mainStatusBar->addPermanentWidget(new QLabel("v" + QString(STR_VERSION)));
    duqf_settingsButton = new QToolButton();
    duqf_settingsButton->setIcon(QIcon(":/icons/settings"));
    duqf_settingsButton->setToolTip("Go to Settings");
    duqf_settingsButton->setCheckable(true);
    mainStatusBar->addPermanentWidget(duqf_settingsButton);
    QToolButton *helpButton = new QToolButton();
    helpButton->setIcon(QIcon(":/icons/help"));
    helpButton->setToolTip("Get Help");
    helpButton->setPopupMode( QToolButton::InstantPopup );
    helpMenu = new QMenu(this);
    if (QString(URL_DOC) != "")
    {
        QAction *docAction = new QAction(QIcon(":/icons/documentation"), "Help");
        docAction->setToolTip("Read the documentation");
        docAction->setShortcut(QKeySequence("F1"));
        helpMenu->addAction(docAction);
        connect(docAction, SIGNAL(triggered()), this, SLOT(duqf_doc()));
    }
    QAction *aboutAction = new QAction(QIcon(":/icons/about"), "About");
    helpMenu->addAction(aboutAction);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(duqf_about()));
    helpMenu->addSeparator();
    bool chat = QString(URL_CHAT) != "";
    bool bugReport = QString(URL_BUGREPORT) != "";
    bool forum = QString(URL_FORUM) != "";
    if (bugReport)
    {
        QAction *bugReportAction = new QAction(QIcon(":/icons/bug-report"), "Bug Report");
        bugReportAction->setToolTip("Report a bug");
        helpMenu->addAction(bugReportAction);
        if (!chat && !forum) helpMenu->addSeparator();
        connect(bugReportAction, SIGNAL(triggered()), this, SLOT(duqf_bugReport()));
    }
    if (chat)
    {
        QAction *chatAction = new QAction(QIcon(":/icons/chat"), "Chat");
        chatAction->setToolTip("Come and have a chat");
        helpMenu->addAction(chatAction);
        if (!forum) helpMenu->addSeparator();
        connect(chatAction, SIGNAL(triggered()), this, SLOT(duqf_chat()));
    }
    if (forum)
    {
        QAction *forumAction = new QAction(QIcon(":/icons/forum"), "Forum");
        forumAction->setToolTip("Join us on our forum");
        helpMenu->addAction(forumAction);
        helpMenu->addSeparator();
        connect(forumAction, SIGNAL(triggered()), this, SLOT(duqf_forum()));
    }
    QAction *aboutQtAction = new QAction(QIcon(":/icons/qt"), "About Qt");
    helpMenu->addAction(aboutQtAction);

    helpButton->setMenu(helpMenu);
    mainStatusBar->addPermanentWidget(helpButton);

    // ========= SETTINGS ========

    settingsWidget = new SettingsWidget();
    duqf_settingsLayout->addWidget(settingsWidget);
    duqf_closeSettingsButton->setObjectName("windowButton");

    AppearanceSettingsWidget *asw = new AppearanceSettingsWidget();
    settingsWidget->addPage(asw, "Appearance", QIcon(":/icons/color"));

    // ====== CONNECTIONS ======
    /*connect(duqf_maximizeButton,SIGNAL(clicked()),this,SLOT(duqf_maximize()));
#ifndef Q_OS_MAC
    connect(minimizeButton,SIGNAL(clicked()),this,SLOT(showMinimized()));
#endif
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));*/

    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(duqf_settingsButton, SIGNAL(clicked(bool)), this, SLOT(duqf_settings(bool)));
    connect(duqf_closeSettingsButton, SIGNAL(clicked()), this, SLOT(duqf_closeSettings()));
    connect(duqf_reinitSettingsButton, SIGNAL(clicked()), this, SLOT(duqf_reinitSettings()));
}

void MainWindow::duqf_setStyle()
{
    // ======== STYLE ========

    //Re-set StyleSheet
    QString cssFile = settings.value("appearance/cssFile", ":/styles/default").toString();
    QString style = settings.value("appearance/style","Default").toString();
    if (cssFile != "")
    {
        DuUI::updateCSS(cssFile);
    }
    else
    {
        DuUI::updateCSS("");
        qApp->setStyle(QStyleFactory::create(style));
    }
    //and font
    DuUI::setFont(settings.value("appearance/font", "Ubuntu").toString());
    //and tool buttons
    int styleIndex = settings.value("appearance/toolButtonStyle", 0).toInt();
    DuUI::setToolButtonStyle(styleIndex);
}

/*void MainWindow::duqf_maximize(bool max)
{
    if (!max)
    {
        duqf_maximizeButton->setIcon(QIcon(":/icons/maximize"));
        this->showNormal();
    }
    else
    {
        duqf_maximizeButton->setIcon(QIcon(":/icons/unmaximize"));
        this->showMaximized();
    }
}

void MainWindow::duqf_maximize()
{
    duqf_maximize(!this->isMaximized());
}*/

void MainWindow::duqf_bugReport()
{
    QDesktopServices::openUrl ( QUrl( URL_BUGREPORT ) );
}

void MainWindow::duqf_forum()
{
    QDesktopServices::openUrl ( QUrl( URL_FORUM ) );
}

void MainWindow::duqf_chat()
{
    QDesktopServices::openUrl ( QUrl( URL_CHAT ) );
}

void MainWindow::duqf_doc()
{
    QDesktopServices::openUrl ( QUrl( URL_DOC ) );
}

void MainWindow::duqf_settings(bool checked)
{
    duqf_settingsButton->setChecked(checked);
    if (checked)
    {
        mainStack->setCurrentIndex(1);
    }
    else
    {
        mainStack->setCurrentIndex(0);
    }
}

void MainWindow::duqf_closeSettings()
{
    duqf_settings(false);
}

void MainWindow::duqf_reinitSettings()
{
    QMessageBox::StandardButton choice = QMessageBox::question(this, "Reset settings", "This will reset all settings to their default values and restart the application.\nAre you sure you want to continue?" );
    if (choice == QMessageBox::Yes)
    {
        settings.clear();
        settings.sync();
        this->close();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

void MainWindow::duqf_about()
{
    duqf_aboutDialog->show();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
      QMouseEvent *mouseEvent = (QMouseEvent*)event;
      if (mouseEvent->button() == Qt::LeftButton)
      {
        duqf_toolBarClicked = true;
        duqf_dragPosition = mouseEvent->globalPos() - this->frameGeometry().topLeft();
        event->accept();
      }
      return true;
  }
  else if (event->type() == QEvent::MouseMove)
  {
      if (this->isMaximized()) return false;
    QMouseEvent *mouseEvent = (QMouseEvent*)event;
    if (mouseEvent->buttons() & Qt::LeftButton && duqf_toolBarClicked)
    {
        this->move(mouseEvent->globalPos() - duqf_dragPosition);
        event->accept();
    }
    return true;
  }
  else if (event->type() == QEvent::MouseButtonRelease)
  {
      duqf_toolBarClicked = false;
      return true;
  }
#ifndef Q_OS_MAC
  else if (event->type() == QEvent::MouseButtonDblClick)
  {
      //duqf_maximize();
      event->accept();
      return true;
  }
#endif
  else
  {
      // standard event processing
      return QObject::eventFilter(obj, event);
  }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QFontDatabase::removeAllApplicationFonts();
    event->accept();
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
    mainStack->setCurrentIndex(2);

    actionAdd_to_favorites->setVisible(false);
    actionClear->setVisible(false);
    actionRemove->setVisible(true);

    buttonManager->show();
}

void MainWindow::on_actionClipboard_History_triggered(bool checked)
{
    actionClipboard_History->setChecked(true);
    actionManage_buttons->setChecked(false);
    mainStack->setCurrentIndex(0);

    actionAdd_to_favorites->setVisible(true);
    actionClear->setVisible(true);
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
