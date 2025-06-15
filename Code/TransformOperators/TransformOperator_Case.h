#ifndef TRANSFORMOPERATION_CASE_H
#define TRANSFORMOPERATION_CASE_H

#include "TransformOperator.h"
#include <QWidget>
#include <QStackedWidget>

namespace Ui {
class TransformOperator_Case;
}

class TransformOperator_Case : public TransformOperator
{
public:
    explicit TransformOperator_Case(QWidget *parent = nullptr);
    ~TransformOperator_Case();
    void updateGUIvars();
    QString transform(const QString& inFullUrl, const QString& in, int index, bool& success);

private:
    enum modes
    {
        lower_case,
        upper_case,
        title_case,
        first_letter_uppercase
    };

    Ui::TransformOperator_Case *ui;
    modes mode;
    static QString toTitleCase(const QString& string);
    static QString capitaliseFirstLetter(const QString& string);
};

#endif // TRANSFORMOPERATION_CASE_H
