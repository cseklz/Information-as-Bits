#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QLineEdit;
class QPushButton;
class QString;

class MainWindow : public QMainWindow {
public:
	explicit MainWindow(QWidget* parent = nullptr);

private:
	void createInterface();
	void handleConversion();
	void updateInputState(const QString& s);
	void clearOutputs();

	static bool containsOnlyAscii(const QString& s);

	QLineEdit* inputBox_{nullptr};
	QPushButton* convertButton_{nullptr};
	QLabel* statusLabel_{nullptr};

	QLineEdit* decimalOutput_{nullptr};
	QLineEdit* binaryOutput_{nullptr};
	QLineEdit* octalOutput_{nullptr};
	QLineEdit* hexadecimalOutput_{nullptr};
};

#endif