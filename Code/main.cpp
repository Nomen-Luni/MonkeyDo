#include "MainWindow/MainWindow.h"
#include <QApplication>
#include <QLocale>
#include <QMessageBox>
#include <QTranslator>
#include "Styling/SingleClickWorkaroundProxyStyle.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // TODO: Does line below need associated cleanup (from new)
    app.setStyle(new SingleClickWorkaroundProxyStyle()); // Workaround to stop file selector closing after single file click on Plasma 6.

    QTranslator translator;
    QLocale locale=QLocale::system();
    QString translationFileName=":/Translations/MonkeyDo_"+locale.name()+".qm";
    if (translator.load(translationFileName))
    {
        app.installTranslator(&translator);
    }

    QStringList commandLineUrls=app.arguments();
    commandLineUrls.removeFirst();    //First argument is executable name, discard it

    MainWindow mainWindow(nullptr, &app);

    QString error=mainWindow.setInitialTransformItems(commandLineUrls);
    if (error!="")
    {
        QMessageBox msg;
        msg.setText(error);
        msg.exec();
        return 1;
    }

    mainWindow.show();
    return app.exec();
}
