#include "FileOperation.h"
#include <QFileInfo>

FileOperation::FileOperation() {}

// Checks all urls in <urls> are unique - no repetitions.
QString FileOperation::urlsUnique(QStringList urlsList)
{
    QSet<QString> uniqueUrls;
    foreach (QString url, urlsList)
    {
        if (uniqueUrls.contains(url)) return url;
        uniqueUrls.insert(url);
    }
    return "";
}

// Checks all urls in set encompassing <uniqueUrlsSet> and <urlsList> are unique - no repetitions.
// If "" (i.e. success) is returned, all the urls in <urlsList> are added to the set <uniqueUrls>.
// Note the state of <uniqueUrlsSet> cannot be relied upon if anything other than ("" = success) is returned.
QString FileOperation::urlsUnique(QSet<QString>* uniqueUrlsSet, QStringList urlsList)
{
    foreach (QString url, urlsList)
    {
        if (uniqueUrlsSet->contains(url)) return url;
        uniqueUrlsSet->insert(url);
    }
    return "";
}

/*
// Checks the urls in <urls>, when combined with the urls in <existingUniqueUrls>, are all unique.
static QString urlsUnique(QSet<QString> existingUniqueUrls,  QStringList urls)
{

    foreach (QString url, urls)
    {
        if (existingUniqueUrls.contains(url)) return url;
        existingUniqueUrls.insert(url);
    }
    return "";
}
*/

QString FileOperation::urlsWriteable(QStringList urls)
{
    QFileInfo fileInfo;
    foreach (QString url, urls)
    {
        fileInfo.setFile(url);
        if (fileInfo.isWritable()==false) return url;
    }
    return "";
}

QString FileOperation::findNonClashingRenameOrder(QStringList sourceUrlsList, QStringList targetUrlsList)
{
    return "hello";
}

QString FileOperation::renameFiles(QStringList sourceUrlsList, QStringList targetUrlsList)
{
    QSet<QString> uniqueTargetUrlsSet;

    if (sourceUrlsList.size()!=targetUrlsList.size()) return ("Internal error. Restart program.\nBulk rename not attempted.");

    if (sourceUrlsList.size()==0) return ""; // Not an error - no files in list - nothing needs doing.

    int i;
    // Discard any files that aren't actually being renamed.
    for (i=sourceUrlsList.size()-1; i>=0; i--)
    {
        if (sourceUrlsList[i]==targetUrlsList[i])
        {
            sourceUrlsList.removeAt(i);
            targetUrlsList.removeAt(i);
        }
    }

    if (sourceUrlsList.size()==0) return ""; // Not an error - no files need renaming.

    if (urlsUnique(&uniqueTargetUrlsSet, targetUrlsList)!="")
        return "One or more target paths are identical - cannot rename multiple file to same name.\nBulk rename not attempted.";

    QSet<QString> uniqueSourceAndTargetUrlsSet=uniqueTargetUrlsSet;
    if (urlsUnique(&uniqueSourceAndTargetUrlsSet, sourceUrlsList)=="")
    {
        //No potential clashes between file source names and file target names - we can perform a simple set of renames
        //if (urlsWriteable(targetUrlsList)!="")
        //return "Some target paths are not writeable.\nBulk rename not attempted.";

        return renameFilesNoClashes(sourceUrlsList, targetUrlsList);
    }

    // We can't just do a simple file-by-file rename because some of the target file names clash with existing source file names..
    // Let's see if there's an order we can perform the operation in to avoid clashes..

    return "";
}

// Renames each file in sourceUrlsList to the filename in rargetUrlsList at the same position
// If the source and target url for one and the same file are identical, the file is skipped and no attempt is made to rename.
// Caller to ensure all target urls are empty and writeable - the sum of all sourceUrls and all targetUrls must be unique, no clashes,
// because this function performs no cleverness to avoid problems where there is an attempt to rename one or more files to the same
// name as one of the source files before rename. For example, you cannot 'swap filenames' such as 'x->y' and 'y->x'.
QString FileOperation::renameFilesNoClashes(QStringList sourceUrlsList, QStringList targetUrlsList)
{
    bool success;

    for (int index=0; index<sourceUrlsList.length(); index++)
    {
        QFile sourcefile(sourceUrlsList[index]);
        //if (sourceUrlsList[index]==targetUrlsList[index]) continue; //No change to filename
        success=sourcefile.rename(targetUrlsList[index]);
        if (success==false) return "Error attempting to rename file";
    }

    return "";
}

