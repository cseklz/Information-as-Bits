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
	void handleConversion() const;
	void updateInputSlate(const QString& s) const;
	void clearOutputs() const;

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