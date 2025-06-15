#include "DragTableView.h"

DragTableView::DragTableView(QWidget *parent):QTableView(parent)
{
    selection= this->selectionModel();
}

void DragTableView::mousePressEvent(QMouseEvent* event)
{
    QTableView::mousePressEvent(event);
}

void DragTableView::dropEvent(QDropEvent* event)
{
    if (event->source()!=this)
        return;

    selection=this->selectionModel();
    QModelIndexList selectedRows= selection->selectedRows();

    auto dropRow = rowAt(event->position().toPoint().y());
    int dropIndex;
    DropIndicatorPosition dropIndicator = dropIndicatorPosition();
    switch (dropIndicator)
    {
        case QAbstractItemView::AboveItem:
            dropIndex=dropRow; // i.e. insert above
            break;
        case QAbstractItemView::BelowItem:
            dropIndex=dropRow+1;
            break;
        case QAbstractItemView::OnViewport:
            dropIndex=this->model()->rowCount(); // i.e. insert at bottom
            break;
        case QAbstractItemView::OnItem:
        default:
            dropIndex=-1; // This shouldn't happen
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


