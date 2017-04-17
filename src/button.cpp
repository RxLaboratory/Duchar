#include "button.h"
#include <QtDebug>

Button::Button(QWidget *parent) :
    QPushButton(parent)
{
    this->setMaximumSize(30,30);
    this->setMinimumSize(30,30);
    connect(this, &QPushButton::clicked,this, &Button::copyData);
}

void Button::setData(QString data)
{
    textData = data;
}

void Button::copyData(bool c)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(textData);
}
