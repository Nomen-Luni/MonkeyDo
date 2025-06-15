#include "TransformOperator_RemoveChars.h"
#include "ui_TransformOperator_RemoveChars.h"
#include "MainWindow/MainWindow.h" //For signal connection

TransformOperator_RemoveChars::TransformOperator_RemoveChars(QWidget *parent)
    : TransformOperator(parent) //QWidget constructor called via TransformOperator constructor
    , ui(new Ui::TransformOperator_RemoveChars)
{
    ui->setupUi(this);
    displayName=QObject::tr("Remove Characters");
    ID="REMOVECHARS";

    //Connect change events of all contained control to Main Window's 'transformChanged' slot to trigger an update
    connect(ui->fromCharsSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->toCharsSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->fromComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->toComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),(MainWindow*)parent, &MainWindow::doTransforms);

    //Create palettes for GUI value error highlighting
    //paletteSpinboxDefault=ui->fromCharsSpinBox->palette();
    //paletteSpinboxError=paletteSpinboxDefault;
    //paletteSpinboxError.setColor(QPalette::Base, Qt::red);
}

TransformOperator_RemoveChars::~TransformOperator_RemoveChars()
{
    delete ui;
}

void TransformOperator_RemoveChars::updateGUIvars()
{
    //Read GUI variables
    fromChars=ui->fromCharsSpinBox->value();
    toChars=ui->toCharsSpinBox->value();
    from=(fromToLocations)ui->fromComboBox->currentIndex();
    to=(fromToLocations)ui->toComboBox->currentIndex();

    /*
    bool rangeError=false;
    if ((from==fromto_front)&&(to==fromto_front)&&(fromChars>=toChars)) rangeError=true;
    if ((from==fromto_back)&&(to==fromto_back)&&(fromChars<=toChars)) rangeError=true;

    if (rangeError)
    {
       ui->fromCharsSpinBox->setPalette(paletteSpinboxError);
    }
    else
    {
        ui->fromCharsSpinBox->setPalette(paletteSpinboxDefault);
    }
    */
}

QString TransformOperator_RemoveChars::transform(const QString& inFullUrl, const QString& in, int index, bool& success)
{
    QString transformed=in;
    int fromIndex=0;
    int toIndex=0;
    (void)inFullUrl;
    (void)index;

    switch (from)
    {
        case fromto_front:
            fromIndex=fromChars;
            break;
        case fromto_back:
            fromIndex=in.length()-fromChars;
            break;
    }

    switch (to)
    {
        case fromto_front:
            toIndex=toChars;
            break;
        case fromto_back:
            toIndex=in.length()-toChars;
            break;
    }

    transformed.remove(fromIndex, toIndex);

    success=true;
    return transformed;
}
