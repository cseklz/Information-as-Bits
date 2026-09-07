#pragma once

#include <QMainWindow>

class AsciiWidget;
class BaseConverterWidget;
class HomePage;
class PixelWidget;
class QPushButton;
class QStackedWidget;

class MainWindow final : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void createInterface();
    void showHomePage();
    void showAsciiPage();
    void showBaseConverterPage();
    void showPixelPage();

    QPushButton* backButton_{nullptr};
    QStackedWidget* pages_{nullptr};
    HomePage* homePage_{nullptr};
    AsciiWidget* asciiPage_{nullptr};
    BaseConverterWidget* baseConverterPage_{nullptr};
    PixelWidget* pixelPage_{nullptr};
};
