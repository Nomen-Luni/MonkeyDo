#ifndef TRANSFORMOPERATION_NUMBERING_H
#define TRANSFORMOPERATION_NUMBERING_H

#include "TransformOperator.h"
#include <QWidget>
#include <QStackedWidget>

namespace Ui {
class TransformOperator_Numbering;
}

class TransformOperator_Numbering : public TransformOperator
{
public:
    explicit TransformOperator_Numbering(QWidget *parent = nullptr);
    ~TransformOperator_Numbering();
    void updateGUIvars();
    QString transform(const QString& inFullUrl, const QString& in, int index, bool& success);

private:
    enum locationTypes
    {
        at_start,
        at_end
    };

    enum spacerTypes
    {
        none,
        space,
        period,
        hyphen,
        underscore
    };

    enum padCharacterTypes
    {
        nopad,
        zeros,
        spaces
    };

    Ui::TransformOperator_Numbering *ui;

    //QString startWith;
    spacerTypes spacerType;
    padCharacterTypes padCharacterType;
    int padWidth;

    int startNumber;
    locationTypes location;
};

#endif // TRANSFORMOPERATION_NUMBERING_H
