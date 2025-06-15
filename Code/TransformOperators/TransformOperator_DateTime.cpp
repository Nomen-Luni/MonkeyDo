#include "TransformOperator_DateTime.h"
#include "ui_TransformOperator_DateTime.h"
#include "MainWindow/MainWindow.h" //For signal connection
#include <QDateTime>
#include <QFileInfo>
#include "KExiv2Qt6/kexiv2/kexiv2.h"

TransformOperator_DateTime::TransformOperator_DateTime(QWidget *parent)
    : TransformOperator(parent) //QWidget constructor called via TransformOperator constructor
    , ui(new Ui::TransformOperator_DateTime)
{
    ui->setupUi(this);
    displayName=QObject::tr("Date Time");
    ID="DATETIME";

    //Connect change events of all contained control to Main Window's 'transformChanged' slot to trigger an update
    connect(ui->atPositionSpinBox,QOverload<int>::of(&QSpinBox::valueChanged),(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->dateSelectComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->formatLineEdit,&QLineEdit::textEdited,(MainWindow*)parent, &MainWindow::doTransforms);
    connect(ui->fromComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),(MainWindow*)parent, &MainWindow::doTransforms);

    if (KExiv2Iface::KExiv2::initializeExiv2()==false) displayName="Problem";
}

TransformOperator_DateTime::~TransformOperator_DateTime()
{
    KExiv2Iface::KExiv2::cleanupExiv2();
    delete ui;
}

void TransformOperator_DateTime::updateGUIvars()
{
    atPosition=ui->atPositionSpinBox->value();
    dateSelect=(dateSelections)ui->dateSelectComboBox->currentIndex();
    format=ui->formatLineEdit->text();
    from=(fromLocations)ui->fromComboBox->currentIndex();
}

QString TransformOperator_DateTime::transform(const QString& inFullUrl, const QString& in, int index, bool& success)
{
    QString transformed;
    (void)index;
    QDateTime date;
    int atIndex;
    KExiv2Iface::KExiv2 exiv2fileInfo(inFullUrl);

    QFileInfo fileInfo(inFullUrl);

    switch (dateSelect)
    {
        case current:
            date=QDateTime::currentDateTime();
            break;
        case date_created:
            date=fileInfo.fileTime(QFileDevice::FileBirthTime);
            break;
        case date_accessed:
            date=fileInfo.fileTime(QFileDevice::FileAccessTime);
            break;
        case date_modified:
            date=fileInfo.fileTime(QFileDevice::FileModificationTime);
            break;
        case date_picture_taken:
            date=exiv2fileInfo.getImageDateTime();
            break;
    }

    switch (from)
    {
        case from_front:
            atIndex=atPosition;
            break;
        case from_back:
            atIndex=in.length()-atPosition;
            break;
    }

    QStringView formatStringView(format);
    QString dateString=date.toString(formatStringView);

    transformed=in;
    transformed.insert(atIndex,dateString);

    success=true;
    return transformed;
}
