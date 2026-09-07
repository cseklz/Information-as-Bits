#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class AsciiWidget;
class BaseConverterWidget;
class HomePage;
class PixelWidget;

class QPushButton;
class QScrollArea;
class QStackedWidget;
class QString;
class QWidget;

class MainWindow final : public QMainWindow {
	public:
		explicit MainWindow(QWidget* parent = nullptr);

	private:
		void createInterface();
		void showHomePage();
		void showFeaturePage(QWidget* page, const QString& title);

		QPushButton* backButton_{nullptr};
		QStackedWidget* pages_{nullptr};
		HomePage* homePage_{nullptr};
		AsciiWidget* asciiPage_{nullptr};
		BaseConverterWidget* baseConverterPage_{nullptr};
		PixelWidget* pixelPage_{nullptr};
		QScrollArea* asciiContainer_{nullptr};
		QScrollArea* baseConverterContainer_{nullptr};
		QScrollArea* pixelContainer_{nullptr};
};

#endif
