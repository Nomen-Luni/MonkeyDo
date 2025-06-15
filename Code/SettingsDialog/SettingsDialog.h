#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "SettingsController/SettingsController.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();
    void Initialise(QList<TransformOperatorInfo> transformOperatorInfoList);
    SettingsController settingsController{this};
    //void updateDefaultOperationSelectComboboxFromSettings();
    void updateDefaultOperationSelectComboboxFromSettings(bool init);

protected:
    void showEvent(QShowEvent* event) override;
    void accept() override;
    void reject() override;

private:
    Ui::SettingsDialog *ui;

public slots:
    void tableSortOrderChanged(int logicalIndex, Qt::SortOrder sortOrder);
    //void updateDefaultOperationSelectComboboxFromSettings(bool init);
    void operationsTableChanged(QModelIndex a,QModelIndex b);
};

#endif // SETTINGSDIALOG_H
