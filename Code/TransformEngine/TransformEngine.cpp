#include "TransformEngine.h"
// #include "./FileSystemOverlay/FileSystemOverlay.h"
#include "./FileOperation/FileOperation.h"
#include <QFileInfo>
#include <QFile>
#include <algorithm>

//Definition of variables declared private static in header
int TransformEngine::selectedProviderIndex=0;
QIcon TransformEngine::folderIcon = QIcon::fromTheme("folder");
//QIcon TransformEngine::fileIcon = QIcon::fromTheme("file");
transformScope TransformEngine::scope=transformScope::name_only;
QList<TransformProvider*> TransformEngine::transformProviders;
QList<TransformItem> TransformEngine::transformItemList;

TransformEngine::TransformEngine(QObject *parent): QAbstractTableModel(parent)
{

}

void TransformEngine::DeleteProviders()
{
    for (int index = transformProviders.size()-1; index >= 0; index--)
    {
        delete transformProviders[index];
    }
    transformProviders.clear();
}

QString TransformEngine::addTransformItems(QStringList sourceUrls)
{
    QFileInfo fileInfo;
    QString errorStr=FileOperation::urlsUnique(sourceUrls);
    if (errorStr!="")
        return QObject::tr("Selected URLs not added:\nSelected URLs contained duplicates.");

    errorStr=FileOperation::urlsWriteable(sourceUrls);
    if (errorStr!="")
        return QObject::tr("Selected URLs not added:\nEnsure user has write access to all selected files and directories.");


    QList<QString> combinedUrlsList=getSourceUrls();
    combinedUrlsList.append(sourceUrls);

    errorStr=FileOperation::urlsUnique(combinedUrlsList);
    if (errorStr!="")
        return QObject::tr("Selected URLs not added:\nCannot re-add urls that have previously been added.");

    beginResetModel();  // =====

    foreach (QString url, sourceUrls)
    {
        TransformItem item=TransformItem(url);
        transformItemList.append(item);
    }

    doTransforms(false);

    endResetModel();  // =====
    return "";
}

bool TransformEngine::removeTransformItems(QList<int> rowIndices)
{
    beginResetModel();  // =====

    foreach (int index, rowIndices)
    {
        transformItemList.removeAt(index);
    }

    if (transformIsOrderDependent())
        doTransforms(false);

    endResetModel();  // =====
    return true;
}

void TransformEngine::clearTransformItems()
{
    beginResetModel();  // =====

    transformItemList.clear();

    endResetModel();  // =====
}

int TransformEngine::addProvider(TransformProvider* provider)
{
    transformProviders.append(provider);
    return 0;
}

int TransformEngine::selectProvider(int index)
{
    selectedProviderIndex=index;
    doTransforms(true);
    return 0;
}

int TransformEngine::selectScope(transformScope scope)
{
    TransformEngine::scope=scope;
    doTransforms(true);
    return 0;
}

void TransformEngine::doTransforms(bool resetModel)
{
    // resetModel should be set true if this method is called alone to marshall table view GUI updates
    // If this method is part of a larger table data update which implements its own reset, resetModel should be set false

    if (resetModel)
        beginResetModel();  // =====

    auto provider=transformProviders[selectedProviderIndex];
    provider->transformMulti(transformItemList, scope);

    if (resetModel)
        endResetModel();    // =====
}

QStringList TransformEngine::getSourceUrls()
{
    QStringList sourceUrls;

    foreach (TransformItem item, transformItemList)
    {
        sourceUrls.append(item.sourceUrl);
    }
    return sourceUrls;
}

QStringList TransformEngine::createTargetUrls()
{
    QStringList targetUrls;
    QFileInfo fileInfo;

    foreach (TransformItem item, transformItemList)
    {
        fileInfo.setFile(item.sourceUrl);
        QString targetUrl=fileInfo.absolutePath()+"/"+item.targetFileName;
        targetUrls.append(targetUrl);
    }
    return targetUrls;
}

QString TransformEngine::renameFiles()
{
    // bool success;
    QStringList sourceUrlsList=TransformEngine::getSourceUrls();
    QStringList targetUrlsList=TransformEngine::createTargetUrls();

    QString errorString=FileOperation::renameFiles(sourceUrlsList, targetUrlsList);
    if (errorString!="") return errorString;

    //Update our stored source file names to match the renamed file names
    transformItemList.clear();

    int ind=0;
    foreach (QString targetUrl,targetUrlsList)
    {
        transformItemList[ind].SetSourceUrl(targetUrl, false);
        ind++;
    }

    return "";
}

bool TransformEngine::sortItemsBySourceFileName(bool reverseAlphabetical)
{
    if (reverseAlphabetical)
    {
        std::sort(transformItemList.begin(), transformItemList.end(), TransformItem::compareBySourceFileNameDescending);
    }
    else
    {
        std::sort(transformItemList.begin(), transformItemList.end(), TransformItem::compareBySourceFileNameAscending);
    }

    return true;
}

bool TransformEngine::transformIsOrderDependent()
{
    auto provider=transformProviders[selectedProviderIndex];
    return provider->transformIsOrderDependent;
}

///////////////////////////////////////////////////////
// Everything below here is view model functionality //
///////////////////////////////////////////////////////

int TransformEngine::rowCount(const QModelIndex & /*parent*/) const
{
    return transformItemList.length();
}

int TransformEngine::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant TransformEngine::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return transformItemList[index.row()].sourceFileName;
        else
            return transformItemList[index.row()].targetFileName;
    }

    if ((role == Qt::DecorationRole) && (index.column() == 0) && (transformItemList[index.row()].isDir))
        return folderIcon;

    return QVariant();
}

QVariant TransformEngine::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Name");
        case 1:
            return QString("New Name");
        }
    }
    return QVariant();
}

Qt::ItemFlags TransformEngine::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flag = QAbstractItemModel::flags(index);

    if (index.isValid()) {
        return (flag | Qt::ItemIsDragEnabled) & (~Qt::ItemIsDropEnabled);
    } else {
        return flag | Qt::ItemIsDropEnabled;
    }
}

void TransformEngine::setFileNames()
{
    beginResetModel();
    //this->sourceFileNamesList = sourceFileNames;
    //this->targetFileNamesList = targetFileNames;
    endResetModel();
}

Qt::DropActions TransformEngine::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

bool TransformEngine::moveRows(const QModelIndex& parent1, int source_first, int count, const QModelIndex& parent2, int dest)
{
    (void) parent1;
    (void) parent2;

    qDebug("source_first (%i) count (%i) dest (%i)", source_first, count, dest);
    if (source_first==dest) return false;

    //beginMoveRows(parent1, source_first, source_last, parent2, dest);

    beginResetModel();  // =====

    QList<TransformItem> removed;
    for (int c=0; c<count; c++)
    {
        auto sourceVal=this->transformItemList[source_first]; // we don't add c since every individual remove shuffles things down
        this->transformItemList.removeAt(source_first);
        removed.append(sourceVal);
    }

    if (dest>source_first)
    {
        dest-=count;
    }
    for (int c=0; c<count; c++)
    {
        this->transformItemList.insert(dest+c,removed[c]);
    }

    if (TransformEngine::transformIsOrderDependent())
    {
        TransformEngine::doTransforms(false);
    }

    endResetModel();    // =====

    return true;
}

