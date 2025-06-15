#ifndef TRANSFORMOPERATION_SEARCHREPLACE_H
#define TRANSFORMOPERATION_SEARCHREPLACE_H

#include "TransformOperator.h"
#include <QWidget>
#include <QStackedWidget>

namespace Ui {
class TransformOperator_SearchReplace;
}

class TransformOperator_SearchReplace : public TransformOperator
{
public:
    explicit TransformOperator_SearchReplace(QWidget *parent = nullptr);
    ~TransformOperator_SearchReplace();
    void updateGUIvars();
    QString transform(const QString& inFullUrl, const QString& in, int index, bool& success);

private:
    Ui::TransformOperator_SearchReplace *ui;

    bool caseSensitiveSearch;
    bool regularExpression;
    QString replaceWith;
    QString searchFor;
};

#endif // TRANSFORMOPERATION_SEARCHREPLACE_H
