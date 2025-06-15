#ifndef TRANSFORMOPERATION_REMOVECHARS_H
#define TRANSFORMOPERATION_REMOVECHARS_H

#include "TransformOperator.h"
#include <QWidget>
#include <QStackedWidget>

namespace Ui {
class TransformOperator_RemoveChars;
}

class TransformOperator_RemoveChars : public TransformOperator
{
public:
    explicit TransformOperator_RemoveChars(QWidget *parent = nullptr);
    ~TransformOperator_RemoveChars();
    void updateGUIvars();
    QString transform(const QString& inFullUrl, const QString& in, int index, bool& success);

private:
    enum fromToLocations
    {
        fromto_front,
        fromto_back
    };

    Ui::TransformOperator_RemoveChars *ui;

    int fromChars;
    int toChars;
    fromToLocations from;
    fromToLocations to;
    //QPalette paletteSpinboxError;
    //QPalette paletteSpinboxDefault;
};

#endif // TRANSFORMOPERATION_REMOVECHARS_H
