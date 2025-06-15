#ifndef TRANSFORMITEM_H
#define TRANSFORMITEM_H

#include <QString>

class TransformItem
{
public:
    TransformItem(QString sourceUrl);
    static bool compareBySourceFileNameAscending(const TransformItem& first, const TransformItem& second);
    static bool compareBySourceFileNameDescending(const TransformItem& first, const TransformItem& second);
    void SetSourceUrl(QString sourceUrl, bool checkIfDir=false);

    bool isDir;
    QString sourceUrl;       // Full source path of the file, including file name if it's a file rather than a directory).
    QString sourceFileName;  // Source file name only (no path included if it's a file, only bottom level folder name if it's a directory)
    QString targetFileName;  // Target file name only (no path included if it's a file, only bottom level folder name if it's a directory)



    //static QStringList getSourceFileNamesList();
private:
    // TransformEngine() {}    //Static/singleton- not instanced
    //static const int maxTransformOperators = 50;
};

#endif // TRANSFORMITEM_H
