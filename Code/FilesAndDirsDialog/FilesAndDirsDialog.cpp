#include "FilesAndDirsDialog.h"
#include <QListView>
#include <QTreeView>

FilesAndDirsDialog::FilesAndDirsDialog(QWidget *parent, const QString &caption, const QString &directory, const QString &filter) : QFileDialog(parent, caption, directory, filter)
{
    // Try to select multiple files and directories at the same time in QFileDialog
    QListView* listView = this->findChild<QListView*>("listView");
    if (listView)
    {
        listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }

    QTreeView* treeView = this->findChild<QTreeView*>();
    if (treeView)
    {
        treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
}

void FilesAndDirsDialog::accept()
{
    QStringList files = selectedFiles();
    if (files.isEmpty())
        return;
    emit filesSelected(files);
    QDialog::accept();
}
