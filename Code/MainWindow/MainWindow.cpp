#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "AboutDialog/AboutDialog.h"

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

    // This doesn't work on Wayland - see call to QGuiApplication::setDesktopFileName() in main.cpp.
    setWindowIcon(QIcon::fromTheme(QStringLiteral("edit-rename")));

    connect(ui->closePushButton, &QPushButton::clicked, app, &QCoreApplication::quit, Qt::QueuedConnection);
    ui->fileNamesTableView->setModel(&transformEngine);
    connect(ui->fileNamesTableView->horizontalHeader(),
                &QHeaderView::sortIndicatorChanged, this, &MainWindow::tableSortOrderChanged);

    //TODO: delete providers
    addProvider(new TransformProvider_Case(this));
    addProvider(new TransformProvider_RemoveChars(this));
    addProvider(new TransformProvider_Numbering(this));
    addProvider(new TransformProvider_InsertOverwrite(this));
    addProvider(new TransformProvider_SearchReplace(this));
    addProvider(new TransformProvider_DateTime(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addProvider(TransformProvider* provider)
{
    TransformEngine::addProvider(provider);
    ui->TransformPagesStackedWidget->addWidget(provider);
    ui->operationComboBox->addItem(provider->displayName);
}

void MainWindow::on_operationComboBox_currentIndexChanged(int index)
{
    TransformEngine::selectProvider(index);
    ui->TransformPagesStackedWidget->setCurrentIndex(index);
    doTransforms();
}

void MainWindow::updateFileNamesTable()
{
    QStringList sourceFileNames=TransformEngine::getSourceFileNamesList();
    QStringList targetFileNames=TransformEngine::getTargetFileNamesList();

    // TODO: Already got filenames - rejig
    transformEngine.setFileNames(sourceFileNames, targetFileNames);
}

void MainWindow::doTransforms()
{
    TransformEngine::doTransform();
    updateFileNamesTable();
}

void MainWindow::on_AddPushButton_clicked()
{
    // auto fileNames = QFileDialog::getOpenFileNames(this, QFileDialog::tr("Select files to add"),"~","All Files (*.*)");
    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setNameFilter(tr("Any file (*.*)"));
    fileDialog.setDirectory("/mnt/Data/Computing/Programming/Code/Temp - Test/");

    //fileDialog.setOption(QFileDialog::DontUseNativeDialog, true); // Makes no difference

    if (!fileDialog.exec()) return;

    auto fileNames=fileDialog.selectedFiles();
    if (fileNames.size() == 0 ) return;

    TransformEngine::addSourceUrls(fileNames);
    doTransforms();
}

void MainWindow::on_RemovePushButton_clicked()
{
    auto indexList = ui->fileNamesTableView->selectionModel()->selectedRows();
    int row;
    //As we're deleting, need to iterate backwards or indices will be wrong
    for (auto index = indexList.crbegin(); index != indexList.crend(); index++)
    {
        row = (*index).row();
        TransformEngine::removeSourceUrl(row);
    }
    doTransforms();
}

void MainWindow::on_clearPushButton_clicked()
{
    TransformEngine::clearSourceUrls();
    updateFileNamesTable();
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

    updateFileNamesTable();
}

void MainWindow::on_aboutButton_clicked()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::on_targetComboBox_currentIndexChanged(int index)
{
    // (void)index;
    TransformEngine::selectScope((transformScope)index);
    doTransforms();
}

void MainWindow::tableSortOrderChanged(int logicalIndex, Qt::SortOrder sortOrder)
{
    if (logicalIndex==0)   //Name column header
    {
        // auto val=ui->fileNamesTableView->horizontalHeader()->sortIndicatorOrder();
        if (sortOrder==Qt::SortOrder::AscendingOrder)
        {
            TransformEngine::sortSourceUrls(false);
        }
        else
        {
            TransformEngine::sortSourceUrls(true);
        }
        //Quick but slightly dirty way of reordering target filename list and updating table...
        doTransforms();
    }
}

