#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QString>

int main(int argc, char *argv[]) {
	QApplication application(argc, argv);
	QApplication::setApplicationName("Information as Bits");
	QApplication::setOrganizationName("CS240");

	QFile styleFile(":/style/style.qss");
	if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		application.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
	}

	MainWindow window;
	window.show();
	return QApplication::exec();
}