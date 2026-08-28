#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QLineEdit;

class MainWindow : public QMainWindow {
public:
	explicit MainWindow(QWidget* parent = nullptr);

private:
	void createInterface();
	void convertDecimal() const;
	void convertBinary() const;
	void convertOctal() const;
	void convertHexadecimal() const;

	QLineEdit* inputBox_{nullptr};
	QLabel* outputLabel_{nullptr};
};

#endif