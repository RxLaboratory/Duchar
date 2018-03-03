#include "button.h"
#include <QtDebug>

Button::Button(QWidget *parent) :
    QAction(parent)
{
    connect(this, &QAction::triggered,this, &Button::copyData);
    _inToolBar = false;
    _inTray = false;
}

void Button::setData(QString data)
{
    textData = data;
}

QString Button::getData()
{
    return textData;
}

void Button::copyData(bool c)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    emit pauseUpdateStack(10);
    clipboard->setText(textData);
}

bool Button::inToolBar() const
{
    return _inToolBar;
}

void Button::setInToolBar(bool inToolBar)
{
    _inToolBar = inToolBar;
}

bool Button::inTray() const
{
    return _inTray;
}

void Button::setInTray(bool inTray)
{
    _inTray = inTray;
}
