#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include "TransformEngine/TransformEngine.h"
#include "TransformProviders/TransformProvider.h"
//#include "FileNameTableModel/FileNameTableModel.h"

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
    void addProvider(TransformProvider* provider);
    void removeProviders();
    TransformEngine transformEngine;
    Ui::MainWindow *ui;

public slots:
    void doTransforms();
    void tableSortOrderChanged(int logicalIndex, Qt::SortOrder sortOrder);

private slots:
    void on_operationComboBox_currentIndexChanged(int index);
    void on_AddPushButton_clicked();
    void on_RemovePushButton_clicked();
    void on_clearPushButton_clicked();
    void on_renamePushButton_clicked();
    void on_aboutButton_clicked();
    void on_targetComboBox_currentIndexChanged(int index);
};
#endif // MAINWINDOW_H
