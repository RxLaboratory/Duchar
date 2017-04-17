#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QAction>
#include <QGuiApplication>
#include <QClipboard>

class Button : public QAction
{
    Q_OBJECT
public:
    explicit Button(QWidget *parent = 0);
    /**
     * @brief Sets the text data stored by the button
     * @param data  The text data to be stored
     */
    void setData(QString data);

signals:

    /**
     * @brief Emitted when a button is pressed
     * used in MainWindow to pause the stack update
     * @param The duration of the pause in milliseconds
     */
    void pauseUpdateStack(int);

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
