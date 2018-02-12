#include <QApplication>
#include <cpu.h>

int main (int argc, char *argv[]) {
	QApplication app(argc, argv);
    CPU cpu;
    cpu.show();
	return app.exec();
}
