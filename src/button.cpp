#include "button.h"
#include <QtDebug>

Button::Button(QWidget *parent) :
    QAction(parent)
{
    connect(this, &QAction::triggered,this, &Button::copyData);
}

void Button::setData(QString data)
{
    textData = data;
}

void Button::copyData(bool c)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    emit pauseUpdateStack(10);
    clipboard->setText(textData);
}
