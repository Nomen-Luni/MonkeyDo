#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QAbstractTableModel>
#include <QDropEvent>
#include <QModelIndex>
#include <QStringList>
#include "TransformOperatorInfo.h"

class SettingsController: public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SettingsController(QObject* parent = nullptr);

    void Initialise(QList<TransformOperatorInfo> TransformOperatorInfoList);

    // Instance methods
    bool sortItemsByProviderName(bool reverseAlphabetical);
    QList<TransformOperatorInfo> getOrderedVisibleOperatorsList(); // or static?
    int getOperationComboIndex(); // or static?
    void saveSettings(int defaultOperatorEngineIndex, int defaultScopeIndex); // or static?

    int getUpdatedOperatorEngineIndex();
    int getUpdatedOperatorIndex_userCombo();
    int getUpdatedScopeIndex();
    int getOperatorIndexFromEngineIndex_userCombo(int engineIndex);
    int getDefaultOperatorIndex_settingsCombo();
    int getOperatorIndexFromEngineIndex_settingsCombo(int engineIndex);

    void loadSettings(); // or static?
    void saveCurrentSelections(int operationSelectionIndex, int scopeSelectionIndex); // or static?
    void loadCurrentSelections(bool loadCurrentOperator, bool loadCurrentScope); // or static?
    void pushSettingsState(); // or static?
    void popSettingsState(); // or static?
    int getDefaultOperatorEngineIndex();

    // ViewModel Functions
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const override;
    bool moveRows(const QModelIndex& parent1, int source_first, int count, const QModelIndex& parent2, int dest) override;

//signals:
    //void dataChanged(QModelIndex,QModelIndex) ;

private:
    static QList<TransformOperatorInfo> transformOperatorInfoList;
    static QList<TransformOperatorInfo> transformOperatorInfoList_pushed;
    static int defaultScopeIndex_pushed;
    static int defaultOperatorEngineIndex_pushed;
    static int defaultScopeIndex;
    static int defaultOperatorEngineIndex;
    static int updatedOperatorEngineIndex;
    static int updatedScopeIndex;

    //const QString orgName = "nomenluni";
    //const QString appName = "MonkeyDo Batch Renamer";
    static QString savedSettingsPath;
    static QString savedSelectionsPath;
    static int getOperatorEngineIndexFromSettingsListIndex(int settingsListIndex);
    static int getOperatorSettingsListIndexFromEngineIndex(int engineIndex);
    static int getSettingsOperatorListIndexFromEngineIndex(int engineIndex, bool onlyIncludeVisibleOperators);

    // TransformEngine() {}    //Static/singleton- not instanced
};

#endif // SETTINGSCONTROLLER_H
