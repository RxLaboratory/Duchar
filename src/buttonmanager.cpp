#include "buttonmanager.h"

ButtonManager::ButtonManager(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow);//subwindow removes the task bar item
    //remove right click on toolbar
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //toolBar->addAction(actionUp);
    //toolBar->addAction(actionDown);
    toolBar->addAction(actionRemove);

    QWidget *spacer = new QWidget(toolBar);
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    toolBar->addWidget(spacer);

    toolBar->addAction(actionQuit);

    //drag window
    toolBar->installEventFilter(this);

    //connections
    mapEvents();
}

void ButtonManager::mapEvents()
{
    connect(actionRemove,SIGNAL(triggered()),this,SLOT(removeButton()));
    connect(actionQuit,SIGNAL(triggered()),this,SLOT(hide()));
}

void ButtonManager::addButton(Button *b)
{
    buttonList->addItem(b->text());
    buttons.append(b);
}

void ButtonManager::removeButton()
{
    if (buttonList->selectedItems().length() == 0) return;
    for(int i = 0 ; i < buttons.length() ; i++)
    {
        if (buttonList->currentItem()->text() == buttons[i]->text())
        {
            QListWidgetItem *item = buttonList->takeItem(buttonList->currentRow());
            delete item;
            Button *btn = buttons.takeAt(i);
            delete btn;
            break;
        }
    }
}


//EVENT FILTER

bool ButtonManager::eventFilter(QObject *obj, QEvent *event)
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
