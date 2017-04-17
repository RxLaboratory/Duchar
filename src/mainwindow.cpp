#include "mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    //appearance
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QFile cssFile(":/styles/default");
    qDebug() << cssFile.exists();
    cssFile.open(QFile::ReadOnly);
    QString css = cssFile.readAll();
    cssFile.close();
    qApp->setStyleSheet(css);

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
}

void MainWindow::addButton(QString label,QString data)
{
    Button *btn = new Button(this);
    btn->setText(label);
    btn->setData(data);
    mainLayout->insertWidget(1,btn);
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

