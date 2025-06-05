#include "TransformProvider.h"

bool TransformProvider::transformMulti(QList<TransformItem>& transformItems, transformScope scope)
{
    bool successOne=true;
    bool successAll=true;
    QString transformed;

    int index=0;

    updateGUIvars();
    QString fileNameOnly;
    QString extensionOnly;
    bool splitFound;

    for (int index=0; index<transformItems.length(); index++)
    //foreach (TransformItem item, transformItems)
    {
        switch (scope)
        {    
            case name_and_extension:
                transformed=transform(transformItems[index].sourceUrl, transformItems[index].sourceFileName, index, successOne);
            break;
            case name_only:
                splitFound=splitFileName(transformItems[index].sourceFileName,fileNameOnly,extensionOnly);
                transformed=transform(transformItems[index].sourceUrl,fileNameOnly, index, successOne);
                if (splitFound)
                {
                    transformed=transformed+"."+extensionOnly;
                }
            break;
            case extension_only:
                splitFound=splitFileName(transformItems[index].sourceFileName,fileNameOnly,extensionOnly);
                if (splitFound)
                {
                    transformed=transform(transformItems[index].sourceUrl,extensionOnly, index, successOne);
                    transformed=fileNameOnly+"."+transformed;
                }
                else
                {
                    transformed=transformItems[index].sourceFileName;
                }
            break;
        }

        if (successOne==false)
        {
            transformed="";
            successAll=false;
        }
        transformItems[index].targetFileName=transformed;
    }
    return successAll;
}

bool TransformProvider::splitFileName(const QString& fullFileName, QString& filename, QString& extension, bool greedyExtension)
{
    int offset;
    if (greedyExtension)
    {
        offset=fullFileName.indexOf(".");
    }
    else
    {
        offset=fullFileName.lastIndexOf(".");
    }
    if (offset>=0)
    {
        filename = fullFileName.left(offset);
        extension = fullFileName.mid(offset+1);
    }
    else
    {
        filename=fullFileName;
        extension="";
    }
    return offset>=0;
}





