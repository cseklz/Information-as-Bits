#include "MainWindow.h"
#include "ConverterWidget.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	createInterface();
}

void MainWindow::createInterface() {
	setWindowTitle("Information as Bits");
	setMinimumSize(640, 580);
	resize(1000, 720);

	auto* centralWidget = new QWidget(this);
	centralWidget->setObjectName("page");

	auto* mainLayout = new QVBoxLayout(centralWidget);
	mainLayout->setContentsMargins(28, 24, 28, 24);
	mainLayout->setSpacing(12);

	auto* navigationLayout = new QHBoxLayout;

	backButton_ = new QPushButton("Back");
	backButton_->setObjectName("back");
	backButton_->setCursor(Qt::PointingHandCursor);
	backButton_->setVisible(false);

	navigationLayout->addWidget(backButton_);
	navigationLayout->addStretch();

	pages_ = new QStackedWidget;

	converterPage_ = new ConverterWidget;
	pages_->addWidget(converterPage_);

	mainLayout->addLayout(navigationLayout);
	mainLayout->addWidget(pages_);

	setCentralWidget(centralWidget);
}