#include "MainWindow.h"
#include "Conversions.h"

#include <QChar>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	createInterface();
}

void MainWindow::createInterface() {
	setWindowTitle("ASCII Converter");
	setMinimumSize(640, 500);
	resize(800, 600);

	auto* centralWidget = new QWidget(this);
	centralWidget->setObjectName("page");

	auto* pageLayout = new QVBoxLayout(centralWidget);
	pageLayout->setContentsMargins(28, 24, 28, 24);

	auto* card = new QFrame;
	card->setObjectName("card");

	auto* cardLayout = new QVBoxLayout(card);
	cardLayout->setContentsMargins(32, 28, 32, 28);
	cardLayout->setSpacing(12);

	auto* title = new QLabel("ASCII Converter");
	title->setObjectName("title");

	auto* subtitle = new QLabel("Enter ASCII to see decimal, binary, octal, and hexadecimal output");
	subtitle->setObjectName("subtitle");
	subtitle->setWordWrap(true);

	auto* inputLabel = new QLabel("Text");
	inputLabel->setObjectName("input");

	inputBox_ = new QLineEdit;
	inputBox_->setPlaceholderText("Enter ASCII Only");
	inputBox_->setClearButtonEnabled(true);

	statusLabel_ = new QLabel("Enter at least one ASCII character.");
	statusLabel_->setObjectName("status");

	convertButton_ = new QPushButton("Convert");
	convertButton_->setEnabled(false);
	convertButton_->setCursor(Qt::PointingHandCursor);

	auto* outputLabel = new QLabel("Conversions");
	outputLabel->setObjectName("output");

	auto createOutputField = [] {
		auto* field = new QLineEdit;
		field->setReadOnly(true);
		field->setPlaceholderText("-");
		return field;
	};

	decimalOutput_ = createOutputField();
	binaryOutput_ = createOutputField();
	octalOutput_ = createOutputField();
	hexadecimalOutput_ = createOutputField();

	auto* outputGrid = new QGridLayout;

	outputGrid->setHorizontalSpacing(12);
	outputGrid->setVerticalSpacing(8);
	outputGrid->setColumnStretch(1, 1);

	outputGrid->addWidget(new QLabel("Decimal"), 0, 0);
	outputGrid->addWidget(decimalOutput_, 0, 1);

	outputGrid->addWidget(new QLabel("Binary"), 1, 0);
	outputGrid->addWidget(binaryOutput_, 1, 1);

	outputGrid->addWidget(new QLabel("Octal"), 2, 0);
	outputGrid->addWidget(octalOutput_, 2, 1);

	outputGrid->addWidget(new QLabel("Hexadecimal"), 3, 0);
	outputGrid->addWidget(hexadecimalOutput_, 3, 1);

	cardLayout->addWidget(title);
	cardLayout->addWidget(subtitle);
	cardLayout->addSpacing(8);

	cardLayout->addWidget(inputLabel);
	cardLayout->addWidget(inputBox_);
	cardLayout->addWidget(statusLabel_);
	cardLayout->addWidget(convertButton_);

	cardLayout->addSpacing(10);
	cardLayout->addWidget(outputLabel);
	cardLayout->addLayout(outputGrid);

	pageLayout->addStretch();
	pageLayout->addWidget(card);
	pageLayout->addStretch();

	setCentralWidget(centralWidget);
}
// 	layout->addWidget(instructions);
// 	layout->addWidget(inputBox_);
// 	layout->addWidget(decimalButton);
// 	layout->addWidget(binaryButton);
// 	layout->addWidget(octalButton);
// 	layout->addWidget(hexadecimalButton);
// 	layout->addWidget(outputLabel_);
// 	layout->addStretch();
//
// 	setCentralWidget(centralWidget);
//
// 	connect(
// 		decimalButton,
// 		&QPushButton::clicked,
// 		this,
// 		&MainWindow::convertDecimal);
//
// 	connect(
// 		binaryButton,
// 		&QPushButton::clicked,
// 		this,
// 		&MainWindow::convertBinary);
//
// 	connect(
// 		octalButton,
// 		&QPushButton::clicked,
// 		this,
// 		&MainWindow::convertOctal);
//
// 	connect(
// 		hexadecimalButton,
// 		&QPushButton::clicked,
// 		this,
// 		&MainWindow::convertHexadecimal);
// }
//
// void MainWindow::convertDecimal() const {
// 	const std::string input = inputBox_->text().toStdString(); // replace with ascii function
// 	const std::string result = Conversions::toDecimal(input);
// 	outputLabel_->setText(QString::fromStdString(result));
// }
//
// void MainWindow::convertBinary() const {
// 	const std::string input = inputBox_->text().toStdString(); // replace with ascii function
// 	const std::string result = Conversions::toBinary(input);
// 	outputLabel_->setText(QString::fromStdString(result));
// }
//
// void MainWindow::convertOctal() const {
// 	const std::string input = inputBox_->text().toStdString();
// 	const std::string result = Conversions::toOctal(input);
// 	outputLabel_->setText(QString::fromStdString(result));
// }
//
// void MainWindow::convertHexadecimal() const {
// 	const std::string input = inputBox_->text().toStdString();
// 	const std::string result = Conversions::toHexadecimal(input);
// 	outputLabel_->setText(QString::fromStdString(result));
// }