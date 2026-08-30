#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QString>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	if (QFile file(":/styles/style.qss"); file.open(QFile::ReadOnly | QFile::Text)) {
		a.setStyleSheet(QString::fromUtf8(file.readAll()));
	}

	MainWindow w;
	w.show();

	return QApplication::exec();
}