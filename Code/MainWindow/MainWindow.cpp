#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QListView>
#include <QTreeView>
#include "AboutDialog/AboutDialog.h"
#include "FilesAndDirsDialog/FilesAndDirsDialog.h"

//Transform providers
#include "TransformProviders/TransformProvider_Case.h"
#include "TransformProviders/TransformProvider_RemoveChars.h"
#include "TransformProviders/TransformProvider_Numbering.h"
#include "TransformProviders/TransformProvider_InsertOverwrite.h"
#include "TransformProviders/TransformProvider_SearchReplace.h"
#include "TransformProviders/TransformProvider_DateTime.h"

MainWindow::MainWindow(QWidget *parent, QApplication* app)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon::fromTheme(QStringLiteral("edit-rename")));

    connect(ui->closePushButton, &QPushButton::clicked, app, &QCoreApplication::quit, Qt::QueuedConnection);
    ui->fileNamesTableView->setModel(&transformEngine);
    connect(ui->fileNamesTableView->horizontalHeader(),
                &QHeaderView::sortIndicatorChanged, this, &MainWindow::tableSortOrderChanged);

    //These transform providers are deleted in the MainWindow destructor
    addProvider(new TransformProvider_Case(this));
    addProvider(new TransformProvider_RemoveChars(this));
    addProvider(new TransformProvider_Numbering(this));
    addProvider(new TransformProvider_InsertOverwrite(this));
    addProvider(new TransformProvider_SearchReplace(this));
    addProvider(new TransformProvider_DateTime(this));
}

MainWindow::~MainWindow()
{
    removeProviders();
    TransformEngine::DeleteProviders();
    delete ui;
}

QString MainWindow::setInitialTransformItems(QStringList urls)
{
    return transformEngine.addTransformItems(urls);
}

void MainWindow::addProvider(TransformProvider* provider)
{
    TransformEngine::addProvider(provider);
    ui->TransformPagesStackedWidget->addWidget(provider);
    ui->operationComboBox->addItem(provider->displayName);
}

void MainWindow::removeProviders()
{
    int count = ui->TransformPagesStackedWidget->count();
    for(int i = count-1; i >= 0; i--)
    {
        QWidget *widget = ui->TransformPagesStackedWidget->widget(i);
        ui->TransformPagesStackedWidget->removeWidget(widget);
    }
}

void MainWindow::on_operationComboBox_currentIndexChanged(int index)
{
    ui->TransformPagesStackedWidget->setCurrentIndex(index);
    transformEngine.selectProvider(index);
}

// This slot is called by transformProviders when their controls change to initiate
// recalculation of transforms and update of file table display
void MainWindow::doTransforms()
{
    transformEngine.doTransforms(true);
}

void MainWindow::on_AddPushButton_clicked()
{
    FilesAndDirsDialog fileDialog(this, tr("Add files"));
    //fileDialog.setLabelText(QFileDialog::Accept, tr("Add"));
    //fileDialog.setLabelText(QFileDialog::LookIn, tr("LookIn"));
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setDirectory("/mnt/Data/Computing/Programming/Code/Temp - Test/");
    fileDialog.setNameFilter(tr("All files and folders (*)"));
    fileDialog.setOption(QFileDialog::DontUseNativeDialog, true);

    if (!fileDialog.exec()) return;

    auto fileUrls=fileDialog.selectedFiles();
    if (fileUrls.size() == 0 ) return;

    transformEngine.addTransformItems(fileUrls);
}

void MainWindow::on_RemovePushButton_clicked()
{
    auto indexList = ui->fileNamesTableView->selectionModel()->selectedRows();
    QList<int> rowList;
    int rowIndex;
    //As we're deleting, need to iterate backwards or indices will be wrong
    for (auto index = indexList.crbegin(); index != indexList.crend(); index++)
    {
        rowIndex = (*index).row();
        rowList.append(rowIndex);
    }

    transformEngine.removeTransformItems(rowList);
}

void MainWindow::on_clearPushButton_clicked()
{
    transformEngine.clearTransformItems();
}

void MainWindow::on_renamePushButton_clicked()
{
    QString errorString=TransformEngine::renameFiles();
    if (errorString!="")
    {
        QMessageBox msg;
        msg.setText(errorString);
        msg.exec();
        return;
    }
    // TODO - GUI update
}

void MainWindow::on_aboutButton_clicked()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::on_targetComboBox_currentIndexChanged(int index)
{
    transformEngine.selectScope((transformScope)index);
}

void MainWindow::tableSortOrderChanged(int logicalIndex, Qt::SortOrder sortOrder)
{
    if (logicalIndex==0)   //Name column header
    {
        // auto val=ui->fileNamesTableView->horizontalHeader()->sortIndicatorOrder();
        if (sortOrder==Qt::SortOrder::AscendingOrder)
        {
            TransformEngine::sortItemsBySourceFileName(false);
        }
        else
        {
            TransformEngine::sortItemsBySourceFileName(true);
        }
        //Quick but slightly dirty way of reordering target filename list and updating table...
        doTransforms();
    }
}

