#ifndef TRANSFORMOPERATION_INSERTOVERWRITE_H
#define TRANSFORMOPERATION_INSERTOVERWRITE_H

#include "TransformOperator.h"
#include <QWidget>
#include <QStackedWidget>

namespace Ui {
class TransformOperator_InsertOverwrite;
}

class TransformOperator_InsertOverwrite : public TransformOperator
{
public:
    explicit TransformOperator_InsertOverwrite(QWidget *parent = nullptr);
    ~TransformOperator_InsertOverwrite();
    void updateGUIvars();
    QString transform(const QString& inFullUrl, const QString& in, int index, bool& success);

private:
    enum fromLocations
    {
        from_front,
        from_back
    };
    enum modes
    {
        insert,
        overwrite
    };

    Ui::TransformOperator_InsertOverwrite *ui;

    fromLocations from;
    modes mode;
    int position;
    QString text;
};

#endif // TRANSFORMOPERATION_INSERTOVERWRITE_H
