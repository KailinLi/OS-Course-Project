#include <QApplication>
#include <add.h>

int main (int argc, char *argv[]) {
    QApplication app(argc, argv);
    Add res;
    res.show ();
    return app.exec();
}
