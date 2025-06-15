#include "TransformOperator_Case.h"
#include "ui_TransformOperator_Case.h"
#include "MainWindow/MainWindow.h" //For signal connection

TransformOperator_Case::TransformOperator_Case(QWidget *parent)
    : TransformOperator(parent) //QWidget constructor called via TransformOperator constructor
    , ui(new Ui::TransformOperator_Case)
{
    ui->setupUi(this);
    displayName=QObject::tr("Uppercase / Lowercase");
    ID="CASE";

    //Connect change events of all contained control to Main Window's 'doTransforms' slot to trigger an update
    connect(ui->modeComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),(MainWindow*)parent, &MainWindow::doTransforms);
}

TransformOperator_Case::~TransformOperator_Case()
{
    delete ui;
}

void TransformOperator_Case::updateGUIvars()
{
    mode=(modes)(ui->modeComboBox->currentIndex());
}

QString TransformOperator_Case::transform(const QString& inFullUrl, const QString& in, int index, bool& success)
{
    QString transformed;
    (void)inFullUrl;
    (void)index;

    switch (mode)
    {
    case upper_case:
        transformed=in.toUpper();
        break;
    case lower_case:
        transformed=in.toLower();
        break;
    case title_case:
        transformed=toTitleCase(in);
        break;
    case first_letter_uppercase:
        transformed=capitaliseFirstLetter(in);
        break;
    }

    success=true;
    return transformed;
}

QString TransformOperator_Case::toTitleCase(const QString& string)
{
    QStringList parts = string.split(' ', Qt::SkipEmptyParts);
    for (int i = 0; i < parts.size(); ++i)
    {
        parts[i]=parts[i].toLower();
        parts[i].replace(0, 1, parts[i][0].toUpper());
    }
    return parts.join(" ");
}

QString TransformOperator_Case::capitaliseFirstLetter(const QString& string)
{
    QString result=string.toLower();
    int index=0;
    foreach (QChar character,result)
    {
        if (character.isLetter())
        {
            result[index]=character.toUpper();
            break;
        }
        index++;
    }
    return result;
}


