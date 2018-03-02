#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#include "ui_buttonmanager.h"
#include <QMouseEvent>
#include "button.h"

class ButtonManager : public QMainWindow, private Ui::ButtonManager
{
    Q_OBJECT

    void mapEvents();
public:
    explicit ButtonManager(QWidget *parent = 0);
    void addButton(Button *b);

private slots:
    void removeButton();

private:
    /**
     * @brief Used to drag window, true is the mover is currently clicked
     */
    bool moverClicked;

    /**
     * @brief Drag position
     * Used for dragging window
     */
    QPoint dragPosition;

    QList<Button *> buttons;

protected:

    // Reimplemented methods
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // BUTTONMANAGER_H
