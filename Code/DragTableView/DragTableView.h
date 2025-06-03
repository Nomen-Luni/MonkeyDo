#ifndef DRAGTABLEVIEW_H
#define DRAGTABLEVIEW_H

#include <QTableView>
#include <QMouseEvent>
#include <QDropEvent>

class DragTableView: public QTableView
{
    //Q_OBJECT
public:
    explicit DragTableView(QWidget* parent = nullptr);

private:
    void mousePressEvent(QMouseEvent* event);
    void dropEvent(QDropEvent* event);
    int sourceRow;
    // int selectedRowPrev;

    QItemSelectionModel* selection;
};

#endif // DRAGTABLEVIEW_H
