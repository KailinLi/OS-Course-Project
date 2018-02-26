#include "copyfile.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CopyFile w;
    w.show();

    return a.exec();
}
