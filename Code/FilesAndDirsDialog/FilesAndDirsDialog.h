#ifndef FILESANDDIRSDIALOG
#define FILESANDDIRSDIALOG

#include <QFileDialog>

// SelectFileDirDialog
class FilesAndDirsDialog : public QFileDialog
{
    Q_OBJECT
public:
    FilesAndDirsDialog(QWidget* parent = 0, const QString& caption = QString(), const QString& directory = QString(), const QString& filter = QString() );
    //FilesAndDirsDialog(QWidget * parent, Qt::WindowFlags flags );
    //~FilesAndDirsDialog();

public slots:
    void accept ();
};

#endif
