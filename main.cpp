#include "ConverterWidget.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	QFile file(":/styles/style.qss");

	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		const QByteArray qss = file.readAll();

		a.setStyleSheet(QString::fromUtf8(qss));
	}

	ConverterWidget c;

	c.setWindowTitle("Information as Bits");
	c.setMinimumSize(640, 500);
	c.resize(1000, 720);
	c.show();
	return QApplication::exec();
}