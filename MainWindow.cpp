#include "MainWindow.h"

#include "AsciiWidget.h"
#include "BaseConverterWidget.h"
#include "HomePage.h"
#include "PixelWidget.h"

#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    createInterface();
}

void MainWindow::createInterface() {
    setMinimumSize(760, 650);
    resize(980, 760);

    auto* centralWidget = new QWidget(this);
    centralWidget->setObjectName("appRoot");

    auto* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(24, 18, 24, 24);
    mainLayout->setSpacing(10);

    auto* navigation = new QHBoxLayout;
    navigation->setSpacing(10);

    backButton_ = new QPushButton("Back");
    backButton_->setProperty("role", "navigation");
    backButton_->setCursor(Qt::PointingHandCursor);

    auto* moduleLabel = new QLabel("MODULE 1");
    moduleLabel->setProperty("role", "eyebrow");

    navigation->addWidget(backButton_);
    navigation->addStretch(1);
    navigation->addWidget(moduleLabel);

    pages_ = new QStackedWidget;
    homePage_ = new HomePage;
    asciiPage_ = new AsciiWidget;
    baseConverterPage_ = new BaseConverterWidget;
    pixelPage_ = new PixelWidget;
    pages_->addWidget(homePage_);
    pages_->addWidget(asciiPage_);
    pages_->addWidget(baseConverterPage_);
    pages_->addWidget(pixelPage_);

    mainLayout->addLayout(navigation);
    mainLayout->addWidget(pages_, 1);
    setCentralWidget(centralWidget);

    connect(backButton_, &QPushButton::clicked, this, &MainWindow::showHomePage);
    connect(homePage_, &HomePage::asciiRequested, this, &MainWindow::showAsciiPage);
    connect(
        homePage_,
        &HomePage::baseConverterRequested,
        this,
        &MainWindow::showBaseConverterPage);
    connect(homePage_, &HomePage::pixelsRequested, this, &MainWindow::showPixelPage);

    showHomePage();
}

void MainWindow::showHomePage() {
    pages_->setCurrentWidget(homePage_);
    backButton_->setVisible(false);
    setWindowTitle("Information as Bits");
}

void MainWindow::showAsciiPage() {
    pages_->setCurrentWidget(asciiPage_);
    backButton_->setVisible(true);
    setWindowTitle("Information as Bits - ASCII to Decimal");
}

void MainWindow::showBaseConverterPage() {
    pages_->setCurrentWidget(baseConverterPage_);
    backButton_->setVisible(true);
    setWindowTitle("Information as Bits - Number Base Converter");
}

void MainWindow::showPixelPage() {
    pages_->setCurrentWidget(pixelPage_);
    backButton_->setVisible(true);
    setWindowTitle("Information as Bits - Pixel Image Codec");
}
