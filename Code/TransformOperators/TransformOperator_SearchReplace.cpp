#include "TransformOperator_SearchReplace.h"
#include "ui_TransformOperator_SearchReplace.h"
#include "MainWindow/MainWindow.h" //For signal connection
#include <QRegularExpression>

TransformOperator_SearchReplace::TransformOperator_SearchReplace(QWidget *parent)
    : TransformOperator(parent) //QWidget constructor called via TransformOperator constructor
    , ui(new Ui::TransformOperator_SearchReplace)
{
    ui->setupUi(this);
    displayName=QObject::tr("Search & Replace");
    ID="SEARCHREPLACE";

    //Connect change events of all contained control to Main Window's 'transformChanged' slot to trigger an update
    connect(ui->caseSensitiveSearchCheckBox,&QCheckBox::toggled,(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->regularExpressionCheckBox,&QCheckBox::toggled,(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->replaceWithLineEdit,&QLineEdit::textEdited,(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->searchForLineEdit,&QLineEdit::textEdited,(MainWindow*)parent, &MainWindow::doTransforms);
}

TransformOperator_SearchReplace::~TransformOperator_SearchReplace()
{
    delete ui;
}

void TransformOperator_SearchReplace::updateGUIvars()
{
    caseSensitiveSearch=ui->caseSensitiveSearchCheckBox->isChecked();
    regularExpression=ui->regularExpressionCheckBox->isChecked();
    replaceWith=ui->replaceWithLineEdit->text();
    searchFor= ui->searchForLineEdit->text();
}

QString TransformOperator_SearchReplace::transform(const QString& inFullUrl, const QString& in, int index, bool& success)
{
    QString transformed=in;
    (void)inFullUrl;
    (void)index;

    if (regularExpression)
    {
        QRegularExpression::PatternOption patternOption=caseSensitiveSearch? QRegularExpression::PatternOption::NoPatternOption : QRegularExpression::PatternOption::CaseInsensitiveOption;
        QRegularExpression regexp(searchFor, patternOption);
        transformed.replace(regexp,replaceWith);
    }
    else
    {
        Qt::CaseSensitivity cs=caseSensitiveSearch?Qt::CaseSensitive : Qt::CaseInsensitive;
        transformed.replace(searchFor,replaceWith,cs);
    }
    success=true;
    return transformed;
}

