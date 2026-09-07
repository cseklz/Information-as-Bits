#include "MainWindow.h"

// #include "AsciiWidget.h"
// #include "BaseConverterWidget.h"
#include "HomePage.h"
// #include "PixelWidget.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

namespace {
	QScrollArea* createScrollablePage(QWidget* page) {
		auto* scrollArea = new QScrollArea;
		scrollArea->setFrameShape(QFrame::NoFrame);
		scrollArea->setWidgetResizable(true);
		scrollArea->viewport()->setAutoFillBackground(true);
		scrollArea->setWidget(page);
		return scrollArea;
	}
} // namespace

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	createInterface();
}

void MainWindow::createInterface() {
	setMinimumSize(820, 720);
	resize(1080, 800);

	auto* centralWidget = new QWidget(this);
	centralWidget->setObjectName("appRoot");

	auto* mainLayout = new QVBoxLayout(centralWidget);
	mainLayout->setContentsMargins(24, 18, 24, 24);
	mainLayout->setSpacing(8);

	auto* navigation = new QHBoxLayout;
	navigation->setSpacing(10);

	backButton_ = new QPushButton("Back");
	backButton_->setProperty("role", "navigation");
	backButton_->setCursor(Qt::PointingHandCursor);

	auto*moduleLabel = new QLabel("MODULE 1");
	moduleLabel->setProperty("role", "eyebrow");

	navigation->addWidget(backButton_);
	navigation->addStretch();
	navigation->addWidget(moduleLabel);

	pages_ = new QStackedWidget;
	homePage_ = new HomePage;

	// baseConverterPage_ = new BaseConverterWidget;
	// pixelPage_ = new PixelWidget;
	// asciiContainer_ = createScrollablePage(asciiPage_);
	// baseConverterContainer_ = createScrollablePage(baseConverterPage_);
	// pixelContainer_ = createScrollablePage(pixelPage_);

	pages_->addWidget(homePage_);
	// pages_->addWidget(asciiContainer_);
	// pages_->addWidget(baseConverterContainer_);
	// pages_->addWidget(pixelContainer_);

	mainLayout->addLayout(navigation);
	mainLayout->addWidget(pages_, 1);

	setCentralWidget(centralWidget);

	connect(
		backButton_,
		&QPushButton::clicked,
		this,
		&MainWindow::showHomePage);

	// connect(
	// 	homePage_,
	// 	&HomePage::asciiRequested,
	// 	this,
	// 	[this] {
	// 		showFeaturePage(asciiContainer_, "ASCII to Decimal");
	// 	});
	//
	// connect(
	// 	homePage_,
	// 	&HomePage::baseConverterRequested,
	// 	this,
	// 	[this] {
	// 		showFeaturePage(baseConverterContainer_, "Numnber Base Converter");
	// 	});
	//
	// connect(
	// 	homePage_,
	// 	&HomePage::pixelsRequested,
	// 	this,
	// 	[this] {
	// 		showFeaturePage(pixelContainer_, "Pixel Image Codec");
	// 	});

	showHomePage();
}

void MainWindow::showHomePage() {
	pages_->setCurrentWidget(homePage_);
	backButton_->setVisible(false);

	setWindowTitle("Information as Bits");
}

void MainWindow::showFeaturePage(QWidget* page, const QString& title) {
	pages_->setCurrentWidget(page);
	backButton_->setVisible(true);

	setWindowTitle("Information as Bits - " + title);
}