#ifndef TRANSFORMOPERATION_DATETIME_H
#define TRANSFORMOPERATION_DATETIME_H

#include "TransformOperator.h"
#include <QWidget>
#include <QStackedWidget>

namespace Ui {
class TransformOperator_DateTime;
}

class TransformOperator_DateTime : public TransformOperator
{
public:
    explicit TransformOperator_DateTime(QWidget *parent = nullptr);
    ~TransformOperator_DateTime();
    void updateGUIvars();
    QString transform(const QString& inFullUrl, const QString& in, int index, bool& success);

private:
    enum dateSelections
    {
        current,
        date_created,
        date_accessed,
        date_modified,
        date_picture_taken
    };

    enum fromLocations
    {
        from_front,
        from_back
    };

    Ui::TransformOperator_DateTime *ui;

    int atPosition;
    dateSelections dateSelect;
    QString format;
    fromLocations from;
};

#endif // TRANSFORMOPERATION_DATETIME_H
