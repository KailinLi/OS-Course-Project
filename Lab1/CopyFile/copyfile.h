#ifndef COPYFILE_H
#define COPYFILE_H

#include <QDialog>

namespace Ui {
class CopyFile;
}

class CopyFile : public QDialog
{
    Q_OBJECT

public:
    explicit CopyFile(QWidget *parent = 0);
    ~CopyFile();

private:
    Ui::CopyFile *ui;
    char fileName[4096];
    char toFileName[4096];
    void chooseFile();
    void cpFile();
    void chooseDir();
};

#endif // COPYFILE_H
