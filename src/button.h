#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QPushButton>
#include <QGuiApplication>
#include <QClipboard>

class Button : public QPushButton
{
public:
    explicit Button(QWidget *parent = 0);
    /**
     * @brief Sets the text data stored by the button
     * @param data  The text data to be stored
     */
    void setData(QString data);

public slots:
    /**
     * @brief Copies text data to the clipboard
     */
    void copyData(bool c);

private:
    /**
     * @brief The text data stored by the button
     */
    QString textData;
};

#endif // BUTTON_H
