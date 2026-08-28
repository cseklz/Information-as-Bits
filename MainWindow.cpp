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
	setWindowTitle("Information as Bits");
	resize(800, 600);

	auto* centralWidget = new QWidget(this);
	auto* layout = new QVBoxLayout(centralWidget);

	auto* instructions = new QLabel("Enter text: ");
	inputBox = new QLineEdit;
	inputBox->setPlaceholderText("ASCII Characters Only");

	auto* convertButton = new QPushButton("Convert to Decimal");

	outputLabel = new QLabel("Output will appear here");
	outputLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
	outputLabel->setWordWrap(true);

	layout->addWidget(instructions);
	layout->addWidget(inputBox);
	layout->addWidget(convertButton);
	layout->addWidget(outputLabel);
	layout->addStretch();

	setCentralWidget(centralWidget);

	connect(
		convertButton,
		&QPushButton::clicked,
		this,
		&MainWindow::convertInput);
}

void MainWindow::convertInput() const {
	std::string input = inputBox->text().toStdString();

	std::string result = Conversions::toDecimal(input);

	outputLabel->setText(QString::fromStdString(result));
}
