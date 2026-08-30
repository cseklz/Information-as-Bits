#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ConverterWidget.h"

class ConvertWidget;
class PictureWidget;
class HomePage;

class QPushButton;
class QStackedWidget;

class MainWindow final : public QMainWindow {
public:
	explicit MainWindow(QWidget* parent = nullptr);

private:
	void createInterface();

	void showHomePage();
	void showConverterPage();
	void showPicturePage();

	QPushButton* backButton_{nullptr};
	QStackedWidget* pages_{nullptr};

	HomePage* homePage_{nullptr};
	ConverterWidget* converterPage_{nullptr};
	PictureWidget* picturePage_{nullptr};
};

#endif