#ifndef TRANSFORMENGINE_H
#define TRANSFORMENGINE_H

#include <QAbstractTableModel>
#include <QDropEvent>
#include <QModelIndex>
#include <QStringList>
#include "TransformItem.h"
#include "TransformOperators/enum_transformScope.h"
#include "TransformOperators/TransformOperator.h"

class TransformEngine: public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TransformEngine(QObject* parent = nullptr);
    static int addProvider(TransformOperator* provider);

    static QStringList getSourceUrls();
    static QStringList createTargetUrls();
    static bool transformIsOrderDependent();

    static void DeleteAllProviders();

    // Instance methods
    bool sortItemsBySourceFileName(bool reverseAlphabetical);
    int selectProvider(int index);
    int selectScope(transformScope txScope);
    void doTransforms(bool resetModel);
    QString addTransformItems(QStringList sourceUrls, int insertAtIndex);
    bool removeTransformItems(QList<int> rowIndices);
    void clearTransformItems();
    QString renameFiles();

    // ViewModel Functions
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const override;
    bool moveRows(const QModelIndex& parent1, int source_first, int count, const QModelIndex& parent2, int dest) override;

private:
    // TransformEngine() {}    //Static/singleton- not instanced
    static int selectedEnabledProviderIndex;
    static QIcon folderIcon;
    static transformScope scope;
    static QList<TransformOperator*> transformOperators;
    static QList<TransformItem> transformItemList;
};

#endif // TRANSFORMENGINE_H
