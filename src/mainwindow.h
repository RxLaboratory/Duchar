#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QFile>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer>
#include <QFileInfo>
#include "button.h"
#include "rainboxui.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

public slots:
    /**
     * @brief Pauses the stack update
     * @param duration    dely before resuming update, in milliseconds
     */
    void pauseUpdateStack(int duration = 10);

    /**
     * @brief Resumes the stack update
     */
    void resumeUpdateStack();

private slots:
    /**
     * @brief updates the style of the application
     * @param cssFileName   The path to a css file to load
     */
    void updateCSS(QString cssFileName);
    /**
     * @brief Handles events from tray icon
     * @param reason    The reason why the icon was activated
     */
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);

    /**
     * @brief Show or hides the application
     */
    void showHide();

    /**
     * @brief Called each time the data stored in the system clipboard has changed
     * adds a new line in the stack (if data type is text)
     */
    void clipboardChanged();

    /**
     * @brief Called when an item in the stack is clicked
     * @param item  The item clicked
     */
    void on_stack_itemClicked(QListWidgetItem *item);

private:

    /**
     * @brief Connects all signals/slots during app init
     */
    void mapEvents();

    /**
     * @brief The system clipboard
     */
    QClipboard *clipboard;

    /**
     * @brief Adds a button to the window
     * @param The text label of the button
     * @param The text data stored by the button
     */
    void addButton(QString label,QString data);

    /**
     * @brief The spacer in the tool bar
     */
    QAction *actionSpacer;

    /**
     * @brief Used to drag window, true is the mover is currently clicked
     */
    bool moverClicked;

    /**
     * @brief Drag position
     * Used for dragging window
     */
    QPoint dragPosition;

    /**
     * @brief Icon in the system tray
     */
    QSystemTrayIcon *trayIcon;

    /**
     * @brief Wether the stack must be updated if the clipboard changes
     */
    bool updateStack;

    /**
     * @brief The timer used to pause the stack update
     */
    QTimer *timer;

    /**
     * @brief trayMenu
     */
    QMenu *trayMenu;

protected:

    // Reimplemented methods
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
