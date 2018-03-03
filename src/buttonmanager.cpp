#include "buttonmanager.h"
#include <QtDebug>

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

    //connect the edit of the items
    connect(buttonList->itemDelegate(), &QAbstractItemDelegate::commitData, this, &ButtonManager::onItemEdited);
    //connect rows moved
    connect(buttonList->model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(onRowsMoved(QModelIndex,int,int,QModelIndex,int)));
}

void ButtonManager::addButton(Button *b)
{
    QListWidgetItem *item = new QListWidgetItem(b->text());
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    buttonList->addItem(item);
    buttons.append(b);

    updateSettings();
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
    updateSettings();
}

void ButtonManager::updateSettings()
{
    settings.beginWriteArray("buttons");
    for(int i = 0; i < buttons.length(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("name", buttons[i]->text());
        settings.setValue("data", buttons[i]->getData());
    }
    settings.endArray();
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

void ButtonManager::onItemEdited(QWidget* pLineEdit)
{
    QString currentText = reinterpret_cast<QLineEdit*>(pLineEdit)->text();
    int ind = buttonList->currentRow();
    buttons[ind]->setText(currentText);
    updateSettings();
}

void ButtonManager::onRowsMoved(const QModelIndex sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow)
{
    Button *btn = buttons.takeAt(sourceStart);
    buttons.insert(destinationRow,btn);
    updateSettings();
    emit buttonMoved(sourceStart,destinationRow);
}
