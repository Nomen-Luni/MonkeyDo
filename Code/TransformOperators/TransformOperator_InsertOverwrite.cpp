#include "TransformOperator_InsertOverwrite.h"
#include "ui_TransformOperator_InsertOverwrite.h"
#include "MainWindow/MainWindow.h" //For signal connection

TransformOperator_InsertOverwrite::TransformOperator_InsertOverwrite(QWidget *parent)
    : TransformOperator(parent) //QWidget constructor called via TransformOperator constructor
    , ui(new Ui::TransformOperator_InsertOverwrite)
{
    ui->setupUi(this);
    displayName=QObject::tr("Insert / Overwrite");
    ID="INSERTOVERWRITE";

    //Connect change events of all contained control to Main Window's 'transformChanged' slot to trigger an update
    connect(ui->fromComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->modeComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->positionSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->textLineEdit,&QLineEdit::textEdited,(MainWindow*)parent, &MainWindow::doTransforms);
}

TransformOperator_InsertOverwrite::~TransformOperator_InsertOverwrite()
{
    delete ui;
}

void TransformOperator_InsertOverwrite::updateGUIvars()
{
    from=(fromLocations)ui->fromComboBox->currentIndex();
    mode=(modes)ui->modeComboBox->currentIndex();
    position=ui->positionSpinBox->value();
    text=ui->textLineEdit->text();
}

QString TransformOperator_InsertOverwrite::transform(const QString& inFullUrl, const QString& in, int index, bool& success)
{
    QString transformed=in;
    int atIndex;
    (void)inFullUrl;
    (void)index;


    switch (from)
    {
        case from_front:
            atIndex=position;
            break;
        case from_back:
            atIndex=in.length()-position;
            break;
    }

    switch(mode)
    {
        case insert:
            transformed.insert(atIndex,text);
            break;
        case overwrite:
            transformed.replace(atIndex,text.length(), text);
            break;
    }
    success=true;
    return transformed;
}


