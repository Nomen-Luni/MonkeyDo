#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include "TransformEngine/TransformEngine.h"
#include "TransformOperators/TransformOperator.h"
#include "SettingsController/TransformOperatorInfo.h"
#include "SettingsDialog/SettingsDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, QApplication* app = nullptr);
    ~MainWindow();
    QString setInitialTransformItems(QStringList urls);

private:
    static SettingsDialog* pSettingsDialog;
    void addOperation(TransformOperator* pTransformOperator, int transformEngineIndex);
    void removeProviders();
    TransformEngine transformEngine;
    QList<TransformOperatorInfo> transformOperatorInfoList;
    QList<int> orderedActiveProviderIndexList; // Is this still needed?
    void updateOperationSelectComboboxFromSettings(bool init);
    Ui::MainWindow *ui;

public slots:
    void doTransforms();
    void tableSortOrderChanged(int logicalIndex, Qt::SortOrder sortOrder);

private slots:
    void on_operationSelect_currentIndexChanged(int index);
    void on_AddButton_clicked();
    void on_RemoveButton_clicked();
    void on_ClearButton_clicked();
    void on_RenameButton_clicked();
    void on_AboutButton_clicked();
    void on_scopeSelect_currentIndexChanged(int index);
    void on_SettingsButton_clicked();
};
#endif // MAINWINDOW_H
