#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QFile>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QMenu>
#include "button.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    /**
     * @brief Handles events from tray icon
     * @param reason    The reason why the icon was activated
     */
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);

    /**
     * @brief Show or hides the application
     */
    void showHide();

private:

    /**
     * @brief Connects all signals/slots during app init
     */
    void mapEvents();

    /**
     * @brief Adds a button to the window
     * @param The text label of the button
     * @param The text data stored by the button
     */
    void addButton(QString label,QString data);

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

protected:

    // Reimplemented methods
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
