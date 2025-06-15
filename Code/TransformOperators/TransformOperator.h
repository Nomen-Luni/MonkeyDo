#ifndef TRANSFORMOPERATION_H
#define TRANSFORMOPERATION_H

#include <QWidget>
#include "enum_transformScope.h"
#include "TransformEngine/TransformItem.h"

namespace Ui {
class TransformOperator;
}

class TransformOperator : public QWidget
{
    Q_OBJECT

public:
    explicit TransformOperator(QWidget* parent = nullptr):QWidget(parent){}
    QString displayName;
    QString ID;
    bool transformMulti(QList<TransformItem>& transformItems, transformScope txScope);
    bool transformIsOrderDependent=false;
    bool transformVisible=true;
private:
    virtual QString transform(const QString& inFullUrl, const QString& in, int index, bool& success) = 0;
    virtual void updateGUIvars() = 0;
    bool splitFileName(const QString& fullFileName, QString& filename, QString& extension, bool greedyExtension=false);
};

#endif // TRANSFORMOPERATION_H
