#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QLineEdit;

class MainWindow : public QMainWindow {
public:
	explicit MainWindow(QWidget *parent = nullptr);
private:
	QLineEdit* inputBox{};
	QLabel* outputLabel{};
	void createInterface();
	void convertDecimal() const;
	void convertBinary() const;
	void convertOctal() const;
	void convertHexadecimal() const;
};

#endif