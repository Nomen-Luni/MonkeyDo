#include "TransformItem.h"
#include <QFileInfo>
#include <QFile>

//Definition of variables declared private static in header
//int TransformEngine::numProviders=0;
//int TransformEngine::selectedProviderIndex=0;
//transformScope TransformEngine::scope=transformScope::name_only;
//TransformProvider* TransformEngine::transformProviders[maxTransformProviders];
//QStringList TransformEngine::sourceFileNamesList;
//QStringList TransformEngine::targetFileNamesList;
//QStringList TransformEngine::sourceUrlsList;
//

//bool TransformItem::isDir;
//QString TransformItem::sourceUrl;       // Full source path of the file, including file name if it's a file rather than a directory).
//QString TransformItem::sourceFileName;  // Source file name only (no path included if it's a file, only bottom level folder name if it's a directory)
//QString TransformItem::targetFileName;  // Target file name only (no path included if it's a file, only bottom level folder name if it's a directory)

TransformItem::TransformItem(QString sourceUrl)
{
    this->SetSourceUrl(sourceUrl, true);
}

bool TransformItem::compareBySourceFileNameAscending(const TransformItem& first, const TransformItem& second)
{
    return first.sourceFileName < second.sourceFileName;
}

bool TransformItem::compareBySourceFileNameDescending(const TransformItem& first, const TransformItem& second)
{
    return first.sourceFileName > second.sourceFileName;
}


void TransformItem::SetSourceUrl(QString sourceUrl, bool checkIfDir)
{
    QFileInfo fileInfo;
    this->sourceUrl=sourceUrl;
    fileInfo.setFile(sourceUrl);
    this->sourceFileName=fileInfo.fileName();
    if (checkIfDir)
    {
        this->isDir=fileInfo.isDir();
    }
    this->targetFileName="go";
}

