#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#include "ui_buttonmanager.h"
#include <QMouseEvent>
#include <QSettings>
#include <QLineEdit>
#include "button.h"

class ButtonManager : public QMainWindow, private Ui::ButtonManager
{
    Q_OBJECT

    void mapEvents();
public:
    explicit ButtonManager(QWidget *parent = 0);
    void addButton(Button *b);

signals:
    void buttonMoved(int from,int to);

private slots:
    void removeButton();

    void onItemEdited(QWidget* pLineEdit);
    void onRowsMoved(const QModelIndex sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationRow);

private:

    /**
     * @brief Updates the list of buttons stored in the settings
     */
    void updateSettings();

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
    QSettings settings;

protected:

    // Reimplemented methods
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // BUTTONMANAGER_H
