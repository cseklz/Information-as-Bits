#include "MainWindow.h"
#include "Conversions.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	createInterface();
}

void MainWindow::createInterface() {
	setWindowTitle("Bit Conversion");
	resize(800, 600);

	auto* centralWidget = new QWidget(this);
	auto* layout = new QVBoxLayout(centralWidget);

	auto* instructions = new QLabel("Enter text: ");
	inputBox_ = new QLineEdit;
	inputBox_->setPlaceholderText("ASCII Characters Only");

	auto* decimalButton = new QPushButton("Convert to Decimal");
	auto* binaryButton = new QPushButton("Convert to Binary");
	auto* octalButton = new QPushButton("Convert to Octal");
	auto* hexadecimalButton = new QPushButton("Convert to Hexadecimal");

	outputLabel_ = new QLabel("Output will appear here");
	outputLabel_->setTextInteractionFlags(Qt::TextSelectableByMouse);
	outputLabel_->setWordWrap(true);

	layout->addWidget(instructions);
	layout->addWidget(inputBox_);
	layout->addWidget(decimalButton);
	layout->addWidget(binaryButton);
	layout->addWidget(octalButton);
	layout->addWidget(hexadecimalButton);
	layout->addWidget(outputLabel_);
	layout->addStretch();

	setCentralWidget(centralWidget);

	connect(
		decimalButton,
		&QPushButton::clicked,
		this,
		&MainWindow::convertDecimal);

	connect(
		binaryButton,
		&QPushButton::clicked,
		this,
		&MainWindow::convertBinary);

	connect(
		octalButton,
		&QPushButton::clicked,
		this,
		&MainWindow::convertOctal);

	connect(
		hexadecimalButton,
		&QPushButton::clicked,
		this,
		&MainWindow::convertHexadecimal);
}

void MainWindow::convertDecimal() const {
	const std::string input = inputBox_->text().toStdString(); // replace with ascii function
	const std::string result = Conversions::toDecimal(input);
	outputLabel_->setText(QString::fromStdString(result));
}

void MainWindow::convertBinary() const {
	const std::string input = inputBox_->text().toStdString(); // replace with ascii function
	const std::string result = Conversions::toBinary(input);
	outputLabel_->setText(QString::fromStdString(result));
}

void MainWindow::convertOctal() const {
	const std::string input = inputBox_->text().toStdString();
	const std::string result = Conversions::toOctal(input);
	outputLabel_->setText(QString::fromStdString(result));
}

void MainWindow::convertHexadecimal() const {
	const std::string input = inputBox_->text().toStdString();
	const std::string result = Conversions::toHexadecimal(input);
	outputLabel_->setText(QString::fromStdString(result));
}