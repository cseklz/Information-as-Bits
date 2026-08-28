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
	void convertAll();
	void updateInputSlate(const QString& text);
	void clearOutPuts();

	static bool containsOnlyAscii(const QString& text);

	QLineEdit* inputBox_{nullptr};
	QPushButton* convertButton_{nullptr};
	QLabel* statusLabel_{nullptr};

	QLineEdit* decimalOutput_{nullptr};
	QLineEdit* binaryOutput_{nullptr};
	QLineEdit* octalOutput_{nullptr};
	QLineEdit* hexadecimalOutput_{nullptr};
};

#endif