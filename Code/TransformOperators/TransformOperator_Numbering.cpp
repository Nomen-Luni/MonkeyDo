#include "TransformOperator_Numbering.h"
#include "ui_TransformOperator_Numbering.h"
#include "MainWindow/MainWindow.h" //For signal connection

TransformOperator_Numbering::TransformOperator_Numbering(QWidget *parent)
    : TransformOperator(parent) //QWidget constructor called via TransformOperator constructor
    , ui(new Ui::TransformOperator_Numbering)
{
    ui->setupUi(this);
    displayName=QObject::tr("Numbering");
    ID="NUMBERING";

    // Let's program know that if order of items in list changes, output names need recalculating.
    transformIsOrderDependent=true; // Numbering depends on position.

    //Connect change events of all contained control to Main Window's 'transformChanged' slot to trigger an update
    connect(ui->startNumberSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->spacerComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->padCharacterComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->padWidthSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->locationComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),(MainWindow*)parent, &MainWindow::doTransforms);
}

TransformOperator_Numbering::~TransformOperator_Numbering()
{
    delete ui;
}

void TransformOperator_Numbering::updateGUIvars()
{
    spacerType=(spacerTypes)ui->spacerComboBox->currentIndex();
    padCharacterType=(padCharacterTypes)ui->padCharacterComboBox->currentIndex();
    padWidth=ui->padWidthSpinBox->value();
    startNumber=ui->startNumberSpinBox->value();
    location=(locationTypes)ui->locationComboBox->currentIndex();
}

QString TransformOperator_Numbering::transform(const QString& inFullUrl, const QString& in, int index, bool& success)
{
    QString transformed;
    QString numberString;
    QString spaceChar;
    int number=startNumber+index;
    (void)inFullUrl;

    numberString=QString::number(number);
    switch (padCharacterType)
    {
        case nopad:
            break;
        case zeros:
            numberString=numberString.rightJustified(padWidth,'0');
            break;
        case spaces:
            numberString=numberString.rightJustified(padWidth,' ');
            break;
    }

    switch (spacerType)
    {
        case none:
            spaceChar="";
        break;
        case space:
            spaceChar=" ";
        break;
        case period:
            spaceChar=".";
        break;
        case hyphen:
            spaceChar="-";
        break;
        case underscore:
            spaceChar="_";
        break;
    }

    switch (location)
    {
        case at_start:
            transformed=numberString+spaceChar+in;
            break;
        case at_end:
            transformed=in+spaceChar+numberString;
            break;
    }

    success=true;
    return transformed;
}


