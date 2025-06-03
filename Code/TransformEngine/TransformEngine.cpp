#include "TransformEngine.h"
// #include "./FileSystemOverlay/FileSystemOverlay.h"
#include "./FileOperation/FileOperation.h"
#include <QFileInfo>
#include <QFile>
#include <algorithm>

//Definition of variables declared private static in header
int TransformEngine::numProviders=0;
int TransformEngine::selectedProviderIndex=0;
transformScope TransformEngine::scope=transformScope::name_only;
TransformProvider* TransformEngine::transformProviders[maxTransformProviders];
QStringList TransformEngine::sourceFileNamesList;
QStringList TransformEngine::targetFileNamesList;
QStringList TransformEngine::sourceUrlsList;
//

TransformEngine::TransformEngine(QObject *parent): QAbstractTableModel(parent)
{
}

QStringList TransformEngine::getSourceFileNamesList()
{
    return sourceFileNamesList;
}

QStringList TransformEngine::getTargetFileNamesList()
{
    return targetFileNamesList;
}

QString TransformEngine::addSourceUrls(QStringList urls)
{
    QFileInfo fileInfo;
    QString errorStr=FileOperation::urlsUnique(urls);
    if (errorStr!="")
        return QObject::tr("Selected URLs not added:\nSelected URLs contained duplicates.");

    errorStr=FileOperation::urlsWriteable(urls);
    if (errorStr!="")
        return QObject::tr("Selected URLs not added:\nEnsure user has write access to all selected files and directories.");

    QList<QString> combinedUrlsList(sourceUrlsList);
    combinedUrlsList.append(urls);

    errorStr=FileOperation::urlsUnique(combinedUrlsList);
    if (errorStr!="")
        return QObject::tr("Selected URLs not added:\nCannot re-add urls that have previously been added.");

    sourceUrlsList.append(urls);
    foreach (QString url, urls)
    {
        fileInfo.setFile(url);
        sourceFileNamesList.push_back(fileInfo.fileName());
    }
    return "";
}

bool TransformEngine::removeSourceUrl(int index)
{
    sourceUrlsList.removeAt(index);
    sourceFileNamesList.removeAt(index);
    return true;
}

void TransformEngine::clearSourceUrls()
{
    sourceUrlsList.clear();
    sourceFileNamesList.clear();
    targetFileNamesList.clear();
}

int TransformEngine::addProvider(TransformProvider* provider)
{
    if (numProviders<maxTransformProviders)
    {
        transformProviders[numProviders]=provider;
        numProviders++;
    }
    return 0;
}

int TransformEngine::selectProvider(int index)
{
    selectedProviderIndex=index;
    return 0;
}

int TransformEngine::selectScope(transformScope scope)
{
    TransformEngine::scope=scope;
    return 0;
}

void TransformEngine::doTransform()
{
    targetFileNamesList.clear();
    auto provider=transformProviders[selectedProviderIndex];
    provider->transformMulti(sourceUrlsList, sourceFileNamesList, targetFileNamesList, scope);
}

QStringList TransformEngine::createTargetUrls()
{
    QStringList targetUrls;
    QFileInfo fileInfo;
    for (int index=0; index<sourceUrlsList.length(); index++)
    {
        fileInfo.setFile(sourceUrlsList[index]);
        QString targetUrl=fileInfo.absolutePath()+"/"+targetFileNamesList[index];
        targetUrls.append(targetUrl);
    }
    return targetUrls;
}

QString TransformEngine::renameFiles()
{
    // bool success;
    QStringList targetUrlsList=TransformEngine::createTargetUrls();

    QString errorString=FileOperation::renameFiles(sourceUrlsList, targetUrlsList);
    if (errorString!="") return errorString;

    //Update our stored source file names to match the renamed file names
    sourceUrlsList.clear();
    sourceFileNamesList.clear();
    foreach (QString targetUrl,targetUrlsList)
    {
        sourceUrlsList.append(targetUrl);
    }
    foreach (QString targetFileName,targetFileNamesList)
    {
        sourceFileNamesList.append(targetFileName);
    }

    return "";
}

bool TransformEngine::sortSourceUrls(bool reverseAlphabetical)
{
    QFileInfo fileInfo;

    sourceUrlsList.sort(Qt::CaseInsensitive);  //Sorts alphabetically
    if (reverseAlphabetical)
    {
        std::reverse(sourceUrlsList.begin(), sourceUrlsList.end());
    }

    //Now update the source and target filenames
    sourceFileNamesList.clear();
    foreach (QString url, sourceUrlsList)
    {
        fileInfo.setFile(url);
        sourceFileNamesList.push_back(fileInfo.fileName());
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
    return sourceFileNamesList.length();
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
            return sourceFileNamesList[index.row()];
        else
            return targetFileNamesList[index.row()];
    }
    else if ((role == Qt::DecorationRole) && (index.column() == 0))
    {
        return QIcon::fromTheme("folder");
    }

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

void TransformEngine::setFileNames(QStringList sourceFileNames, QStringList targetFileNames)
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

    qDebug("source_first (%i) count (%i) dest (%i)", source_first, count, dest);
    if (source_first==dest) return false;

    //beginMoveRows(parent1, source_first, source_last, parent2, dest);

    beginResetModel();

    /*
    this->sourceFileNames.clear();
    this->sourceFileNames.append({"00","01","02","03","04","05"});
    qDebug("moveRows: %i -> %i", source_first, dest);
    qDebug("%s:%s:%s:%s:%s:%s->",
           qPrintable(this->sourceFileNames[0]),
           qPrintable(this->sourceFileNames[1]),
           qPrintable(this->sourceFileNames[2]),
           qPrintable(this->sourceFileNames[3]),
           qPrintable(this->sourceFileNames[4]),
           qPrintable(this->sourceFileNames[5])
           );
    */

    // Re-order source file names list

    QStringList removed;
    for (int c=0; c<count; c++)
    {
        auto sourceVal=this->sourceFileNamesList[source_first]; // we don't add c since every individual remove shuffles things down
        this->sourceFileNamesList.removeAt(source_first);
        removed.append(sourceVal);
    }

    if (dest>source_first)
    {
        dest-=count;
    }
    for (int c=0; c<count; c++)
    {
        this->sourceFileNamesList.insert(dest+c,removed[c]);
    }

    if (TransformEngine::transformIsOrderDependent())
    {
        TransformEngine::doTransform();
        //this->targetFileNames=TransformEngine::getTargetFileNamesList();
    }
    else
    {
        // Re-order target file names list
        // sourceVal=this->targetFileNamesList[source_first];
        // this->targetFileNamesList.removeAt(source_first);
        // this->targetFileNamesList.insert(dest,sourceVal);
    }

    /*
    qDebug("%s:%s:%s:%s:%s:%s",
           qPrintable(this->sourceFileNames[0]),
           qPrintable(this->sourceFileNames[1]),
           qPrintable(this->sourceFileNames[2]),
           qPrintable(this->sourceFileNames[3]),
           qPrintable(this->sourceFileNames[4]),
           qPrintable(this->sourceFileNames[5])
           );
    */

    endResetModel();

    return true;
}

/*
QModelIndex index(int row, int column, const QModelIndex &parent)
{
    return new QModelIndex(row, column);
}
*/
