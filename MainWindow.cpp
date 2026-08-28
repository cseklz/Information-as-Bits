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
	inputBox = new QLineEdit;
	inputBox->setPlaceholderText("ASCII Characters Only");

	auto* convertDecimal = new QPushButton("Convert to Decimal");
	auto* convertBinary = new QPushButton("Convert to Binary");

	outputLabel = new QLabel("Output will appear here");
	outputLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
	outputLabel->setWordWrap(true);

	layout->addWidget(instructions);
	layout->addWidget(inputBox);
	layout->addWidget(convertDecimal);
	layout->addWidget(convertBinary);
	layout->addWidget(outputLabel);
	layout->addStretch();

	setCentralWidget(centralWidget);

	connect(
		convertDecimal,
		&QPushButton::clicked,
		this,
		&MainWindow::convertDecimal);

	connect(
		convertBinary,
		&QPushButton::clicked,
		this,
		&MainWindow::convertBinary);
}

void MainWindow::convertDecimal() const {
	std::string input = inputBox->text().toStdString(); // replace with ascii function
	std::string result = Conversions::toDecimal(input);
	outputLabel->setText(QString::fromStdString(result));
}

void MainWindow::convertBinary() const {
	std::string input = inputBox->text().toStdString(); // replace with ascii function
	std::string result = Conversions::toBinary(input);
	outputLabel->setText("Binary: " + QString::fromStdString(result));
}