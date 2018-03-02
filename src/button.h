#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QAction>
#include <QGuiApplication>
#include <QClipboard>
#include <QMenu>

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

    /**
     * @brief Wether this button is used in the system tray
     * @return
     */
    bool inTray() const;
    /**
     * @brief Use this to set to true if you add this button in the tray
     * @param inTray
     */
    void setInTray(bool inTray);

    /**
     * @brief Wether this button is used in the toolbar
     * @return
     */
    bool inToolBar() const;
    /**
     * @brief Use this to set to true if you add this button in the toolbar
     * @param inToolBar
     */
    void setInToolBar(bool inToolBar);

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
    bool _inTray;
    bool _inToolBar;
};

#endif // BUTTON_H
