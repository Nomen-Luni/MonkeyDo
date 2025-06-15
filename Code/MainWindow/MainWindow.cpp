#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QListView>
#include "FilesAndDirsDialog/FilesAndDirsDialog.h"
#include "AboutDialog/AboutDialog.h"

//Transform providers
#include "TransformOperators/TransformOperator_Case.h"
#include "TransformOperators/TransformOperator_RemoveChars.h"
#include "TransformOperators/TransformOperator_Numbering.h"
#include "TransformOperators/TransformOperator_InsertOverwrite.h"
#include "TransformOperators/TransformOperator_SearchReplace.h"
#include "TransformOperators/TransformOperator_DateTime.h"

//Definition of variables declared private static in header
SettingsDialog* MainWindow::pSettingsDialog;

MainWindow::MainWindow(QWidget *parent, QApplication* app)
    //: QMainWindow(parent, ((Qt::Window | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint))
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pSettingsDialog= new SettingsDialog(this);  // TODO: CLEANUP

    connect(ui->closePushButton, &QPushButton::clicked, app, &QCoreApplication::quit, Qt::QueuedConnection);
    ui->fileNamesTableView->setModel(&transformEngine);
    connect(ui->fileNamesTableView->horizontalHeader(),
                &QHeaderView::sortIndicatorChanged, this, &MainWindow::tableSortOrderChanged);

    //These transform providers are deleted in the MainWindow destructor
    addOperation(new TransformOperator_Case(this),0);
    addOperation(new TransformOperator_RemoveChars(this),1);
    addOperation(new TransformOperator_Numbering(this),2);
    addOperation(new TransformOperator_InsertOverwrite(this),3);
    addOperation(new TransformOperator_SearchReplace(this),4);
    addOperation(new TransformOperator_DateTime(this),5);

    pSettingsDialog->Initialise(transformOperatorInfoList);
    //QStringList orderedProviderDescriptionList=pSettingsDialog->settingsController.getOrderedProvidersDescriptionList();
    //orderedActiveProviderIndexList=pSettingsDialog->settingsController.getOrderedActiveOperatorIndexList();

    updateOperationSelectComboboxFromSettings(true);
    int scopeIndex=pSettingsDialog->settingsController.getUpdatedScopeIndex();
    ui->scopeSelect->setCurrentIndex(scopeIndex);
}

MainWindow::~MainWindow()
{
    int transformEngineIndex=ui->operationSelect->currentData().toInt();
    int scopeIndex=ui->scopeSelect->currentIndex();
    pSettingsDialog->settingsController.saveCurrentSelections(transformEngineIndex, scopeIndex);
    removeProviders();
    TransformEngine::DeleteAllProviders();
    delete ui;
}

QString MainWindow::setInitialTransformItems(QStringList urls)
{
    return transformEngine.addTransformItems(urls, -1);
}

void MainWindow::addOperation(TransformOperator* pTransformOperator, int transformEngineIndex)
{
    TransformEngine::addProvider(pTransformOperator);
    TransformOperatorInfo transformOperatorInfo(transformEngineIndex, pTransformOperator->displayName, true); //TODO - memory cleanup? Notice we add *pointer - how is this managed??
    // TransformOperatorInfo.ID=provider->ID;
    // TransformOperatorInfo.displayName=provider->displayName;
    transformOperatorInfoList.append(transformOperatorInfo);
    ui->TransformPagesStackedWidget->addWidget(pTransformOperator);
    //ui->operationComboBox->addItem(provider->displayName);
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

void MainWindow::on_operationSelect_currentIndexChanged(int index)
{
    //int providerIndex=orderedActiveProviderIndexList[index];
    int providerIndex=ui->operationSelect->itemData(index).toInt();
    ui->TransformPagesStackedWidget->setCurrentIndex(providerIndex);
    transformEngine.selectProvider(providerIndex);
}

// This slot is called by TransformOperators when their controls change to initiate
// recalculation of transforms and update of file table display
void MainWindow::doTransforms()
{
    transformEngine.doTransforms(true);
}

void MainWindow::on_AddButton_clicked()
{
    FilesAndDirsDialog fileDialog(this, tr("Add files"));
    //fileDialog.setLabelText(QFileDialog::Accept, tr("Add"));
    //fileDialog.setLabelText(QFileDialog::LookIn, tr("LookIn"));
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setDirectory("/mnt/Data/Computing/Programming/Code/Temp - Test/");
    fileDialog.setNameFilter(tr("All files and folders (*)"));
    fileDialog.setOption(QFileDialog::DontUseNativeDialog, false);

    if (!fileDialog.exec()) return;

    auto fileUrls=fileDialog.selectedFiles();
    if (fileUrls.size() == 0 ) return;

    QModelIndexList selectedRows= ui->fileNamesTableView->selectionModel()->selectedRows();
    if (selectedRows.size()>0)
    {
        transformEngine.addTransformItems(fileUrls,selectedRows.first().row());
    }
    else
    {
        transformEngine.addTransformItems(fileUrls, -1);
    }
}

void MainWindow::on_RemoveButton_clicked()
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

void MainWindow::on_ClearButton_clicked()
{
    transformEngine.clearTransformItems();
}

void MainWindow::on_RenameButton_clicked()
{
    QString errorString=transformEngine.renameFiles();
    if (errorString!="")
    {
        QMessageBox msg;
        msg.setText(errorString);
        msg.exec();
        return;
    }
}

void MainWindow::on_scopeSelect_currentIndexChanged(int index)
{
    transformEngine.selectScope((transformScope)index);
}

void MainWindow::tableSortOrderChanged(int logicalIndex, Qt::SortOrder sortOrder)
{
    if (logicalIndex==0)   //Name column header
    {
        if (sortOrder==Qt::SortOrder::AscendingOrder)
        {
            transformEngine.sortItemsBySourceFileName(false);
        }
        else
        {
            transformEngine.sortItemsBySourceFileName(true);
        }
    }
}

void MainWindow::on_AboutButton_clicked()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::on_SettingsButton_clicked()
{
    //SettingsDialog settingsDialog(this);
    int settingsChanged = pSettingsDialog->exec();
    if (settingsChanged == QDialog::Accepted) // Can do better check
    {
        updateOperationSelectComboboxFromSettings(false);
    }
}

// If init is true, operation select combo value will be loaded from default or remembered,
// Otherwise it will be modified inr reponse to rearranged operator order/settings
void MainWindow::updateOperationSelectComboboxFromSettings(bool init)
{
    int comboIndex;

    auto visibleProviderList=pSettingsDialog->settingsController.getOrderedVisibleOperatorsList();
    int currentEngineIndex=0;
    if (!init)
        currentEngineIndex=ui->operationSelect->currentData().toInt();


    ui->operationSelect->blockSignals(true);  //====

    ui->operationSelect->clear();
    int index=0;
    foreach (auto item, visibleProviderList)
    {
        ui->operationSelect->addItem(item.displayName);
        ui->operationSelect->setItemData(index,item.operatorEngineIndex);
        index++;
    }

    if (init)
    {
        comboIndex=pSettingsDialog->settingsController.getUpdatedOperatorIndex_userCombo(); // Loaded from file
    }
    else
    {
        comboIndex=pSettingsDialog->settingsController.getOperatorIndexFromEngineIndex_userCombo(currentEngineIndex);
    }

    ui->operationSelect->setCurrentIndex(comboIndex);

    ui->operationSelect->blockSignals(false); //====

    // Should be possible to trigger this automatically by initiating previous 'setCurrentIndex' outside blocksignals
    // but it doesn't seem to work in certain circumstances - so we invoke it explicitly
    on_operationSelect_currentIndexChanged(comboIndex);
}

