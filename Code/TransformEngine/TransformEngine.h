#ifndef TRANSFORMENGINE_H
#define TRANSFORMENGINE_H

#include <QAbstractTableModel>
#include <QDropEvent>
#include <QModelIndex>
#include <QStringList>
#include "TransformProviders/enum_transformScope.h"
#include "TransformProviders/TransformProvider.h"

class TransformEngine: public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TransformEngine(QObject* parent = nullptr);

    static QStringList getSourceFileNamesList();
    static QStringList getTargetFileNamesList();
    static QString addSourceUrls(QStringList urls);
    static bool removeSourceUrl(int index);
    static void clearSourceUrls();
    static int addProvider(TransformProvider* provider);
    static int selectProvider(int index);
    static int selectScope(transformScope txScope);
    static void doTransform();
    static QString renameFiles();
    static QStringList createTargetUrls();
    static bool sortSourceUrls(bool reverseAlphabetical);
    static bool transformIsOrderDependent();

    // ViewModel Functions
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setFileNames(QStringList sourceFileNames, QStringList targetFileNames);
    Qt::DropActions supportedDropActions() const override;
    bool moveRows(const QModelIndex& parent1, int source_first, int count, const QModelIndex& parent2, int dest) override;

private:
    // TransformEngine() {}    //Static/singleton- not instanced
    static const int maxTransformProviders = 50;
    static int numProviders;
    static int selectedProviderIndex;
    static transformScope scope;
    static TransformProvider* transformProviders[maxTransformProviders];
    static QStringList sourceFileNamesList;
    static QStringList targetFileNamesList;
    static QStringList sourceUrlsList;
};

#endif // TRANSFORMENGINE_H
