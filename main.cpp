#include "MainWindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	QFile file(":/styles/style.qss");

	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		const QByteArray qss = file.readAll();

		a.setStyleSheet(QString::fromUtf8(qss));
	}

	MainWindow w;
	w.show();

	return QApplication::exec();
}