#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    //: QDialog(parent, (Qt::Dialog | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint)
    //:QDialog(parent, (windowFlags() | Qt::CustomizeWindowHint) & ~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint))
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    // Hide close button
    auto flags = (windowFlags() | Qt::CustomizeWindowHint) & ~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setWindowFlags(flags);

    ui->setupUi(this); //TODO should this be first or last here??
    ui->providersTableView->setModel(&settingsController);
    connect(ui->providersTableView->horizontalHeader(),
    &QHeaderView::sortIndicatorChanged, this, &SettingsDialog::tableSortOrderChanged);

    connect(&settingsController,
            SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(operationsTableChanged(QModelIndex,QModelIndex)));

    ui->providersTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}

void SettingsDialog::Initialise(QList<TransformOperatorInfo> transformOperatorInfoList)
{
    settingsController.Initialise(transformOperatorInfoList);
    updateDefaultOperationSelectComboboxFromSettings(true);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::showEvent( QShowEvent* event )
{
    QDialog::showEvent(event);  // Call the parent implementation
    //auto defaultOperatorSelection=ui->defaultOperatorSelect->currentIndex();
    //auto defaultScopeSelection=ui->defaultScopeSelect->currentIndex();
    settingsController.pushSettingsState();
}

void SettingsDialog::accept()
{
    // Save updated settings to file
    int defaultOperatorEngineIndex=ui->defaultOperatorSelect->currentData().toInt(); // Yields -1 if <remember last> is selected
    int defaultScopeIndex=ui->defaultScopeSelect->currentIndex()-1; // We want first item, <remember last> to give -1, the rest follow.

    settingsController.saveSettings(defaultOperatorEngineIndex, defaultScopeIndex);
    QDialog::accept(); // Call the parent implementation
}

void SettingsDialog::reject()
{
    // Revert any changes made to the internal settings in the SettingsController
    settingsController.popSettingsState();
    QDialog::reject(); // Call the parent implementation
}

void SettingsDialog::tableSortOrderChanged(int logicalIndex, Qt::SortOrder sortOrder)
{
    if (logicalIndex==0)
    {
        if (sortOrder==Qt::SortOrder::AscendingOrder)
        {
            settingsController.sortItemsByProviderName(false);
        }
        else
        {
            settingsController.sortItemsByProviderName(true);
        }
    }
}

void SettingsDialog::updateDefaultOperationSelectComboboxFromSettings(bool init)
{
    int comboIndex;

    auto visibleProviderList=settingsController.getOrderedVisibleOperatorsList();
    int currentDefaultEngineIndex=-1;
    if (!init)
        currentDefaultEngineIndex=ui->defaultOperatorSelect->currentData().toInt();

    ui->defaultOperatorSelect->blockSignals(true);  //====

    ui->defaultOperatorSelect->clear();
    ui->defaultOperatorSelect->addItem("<Last used>");
    ui->defaultOperatorSelect->setItemData(0,-1);
    int index=1;
    foreach (auto item, visibleProviderList)
    {
        ui->defaultOperatorSelect->addItem(item.displayName);
        ui->defaultOperatorSelect->setItemData(index,item.operatorEngineIndex);
        index++;
    }

    if (init)
        comboIndex=settingsController.getDefaultOperatorIndex_settingsCombo(); // From loaded settings
    else
    {
        comboIndex=settingsController.getOperatorIndexFromEngineIndex_settingsCombo(currentDefaultEngineIndex);
    }

    ui->defaultOperatorSelect->setCurrentIndex(comboIndex);

    ui->defaultOperatorSelect->blockSignals(false); //====

}

void SettingsDialog::operationsTableChanged(QModelIndex a,QModelIndex b)
{
    (void) a;
    (void) b;
    SettingsDialog::updateDefaultOperationSelectComboboxFromSettings(false);
}

