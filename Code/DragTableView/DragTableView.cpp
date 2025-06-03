#include "DragTableView.h"

DragTableView::DragTableView(QWidget *parent):QTableView(parent)
{
    selection= this->selectionModel();
}

void DragTableView::mousePressEvent(QMouseEvent* event)
{
    //if (event->button() == Qt::LeftButton)
    //{
    //    sourceRow = rowAt(event->position().toPoint().y());
    //}
    QTableView::mousePressEvent(event);
}

void DragTableView::dropEvent(QDropEvent* event)
{
    if (event->source()!=this)
        return;

    //if (event->source() != this || event->dropAction() != Qt::MoveAction)
    //    return;

    //if (selection->hasSelection())
    //{
    //    QModelIndexList selectedRows= selection->selectedRows();

    //}

    selection=this->selectionModel();
    QModelIndexList selectedRows= selection->selectedRows();
    qDebug("---");
    foreach (auto selectedRow, selectedRows)
    {
        qDebug("Selrow %i",selectedRow.row());
    }
    qDebug("---");


    auto dropRow = rowAt(event->position().toPoint().y());
    int dropIndex;
    DropIndicatorPosition dropIndicator = dropIndicatorPosition();
    switch (dropIndicator)
    {
        case QAbstractItemView::AboveItem:
            qDebug("above");
            dropIndex=dropRow; // i.e. insert above
            break;
        case QAbstractItemView::BelowItem:
            dropIndex=dropRow+1;
            qDebug("below"); // i.e. insert below
            break;
        case QAbstractItemView::OnItem:
            dropIndex=-1; // This shouldn't happen
            qDebug("on");
            break;
        case QAbstractItemView::OnViewport:
            dropIndex=this->model()->rowCount();
            qDebug("onViewport");
            break;
    }

    if (dropIndex>=0)
    {
        auto sourceRowStart = selectedRows.first().row();
        auto numRows = selectedRows.last().row()-sourceRowStart+1;
        model()->moveRows(QModelIndex(), selectedRows.first().row(), numRows, QModelIndex(), dropIndex);
    }

    event->accept();
}


