#include "MainWindow.h"
#include "Conversions.h"

#include <algorithm>
#include <QByteArray>
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

	setStyleSheet(R"(
		QWidget#page {
			background-color: #19191C;
			color: #f4f6fa;
		}

		QFrame#converterCard {
			background-color: #252528;
			border: 1px solid #292b2d;
			border-radius: 18px;
		}

		QLabel#title {
			font-size: 24px;
			font-weight: 700;
			color: #FFFFFF;
		}

		QLabel#subtitle {
			font-size: 13px;
			color: #CFCFCF;
		}

		QLabel#section {
			font-size: 13px;
			font-weight: 600;
			color: #CFCFCF;
		}

		QLabel#status {
			font-size: 12px;
			color: #CFCFCF;
		}

		QLabel#status[invalid="true"] {
			color: #453B23;
		}

		QLineEdit {
			min-height: 22px;
			padding: 9px 11px;
			background-color: #303033;
			border: 1px solid #58585C;
			border-radius: 9px;
			font-size: 14px;
			color: #FFFFFF;
			selection-background-color: #5146EE;
		}

		QLineEdit:focus {
			padding: 8px 10px;
			border: 2px solid #5146EE;
		}

		QLineEdit[invalid="true"] {
			padding: 8px 10px;
			border: 2px solid #453B23;
		}

		QLineEdit[readOnly="true"] {
			background-color: #FFFFFF;
			color: #334155;
		}

		QPushButton {
			min-height: 24px;
			padding: 10px 14px;
			background-color: #5146EE;
			border: none;
			border-radius: 9px;
			font-size: 14px;
			font-weight: 600;
			color: #ffffff;
		}

		QPushButton:hover {
			background-color: #4338ca;
		}

		QPushButton:pressed {
			background-color: #3730a3;
		}

		QPushButton:disabled {
			background-color: #453B23;
			color: #94a3b8;
		}
	)");

	connect(
		convertButton_,
		&QPushButton::clicked,
		this,
		&MainWindow::handleConversion);

	connect(
		inputBox_,
		&QLineEdit::textChanged,
		this,
		&MainWindow::updateInputState);

	connect(
		inputBox_,
		&QLineEdit::returnPressed,
		convertButton_,
		&QPushButton::click);
}

void MainWindow::handleConversion() {
	if (!convertButton_->isEnabled()) {
		return;
	}

	const QByteArray bytes = inputBox_->text().toLatin1();
	const std::string input = bytes.toStdString();

	const auto [decimal, binary, octal, hexadecimal] = Conversions::convertAll(input);

	decimalOutput_->setText(QString::fromStdString(decimal));
	binaryOutput_->setText(QString::fromStdString(binary));
	octalOutput_->setText(QString::fromStdString(octal));
	hexadecimalOutput_->setText(QString::fromStdString(hexadecimal));

	statusLabel_->setText("Conversion complete.");
}

void MainWindow::clearOutputs() {
	decimalOutput_->clear();
	binaryOutput_->clear();
	octalOutput_->clear();
	hexadecimalOutput_->clear();
}

void MainWindow::updateInputState(const QString& s) {
	const bool isEmpty = s.isEmpty();
	const bool isAscii = containsOnlyAscii(s);
	const bool isValid = !isEmpty && isAscii;
	const bool isInvalid = !isEmpty && !isAscii;

	convertButton_->setEnabled(isValid);


	inputBox_->setProperty("invalid", isInvalid);
	statusLabel_->setProperty("invalid", isInvalid);

	inputBox_->style()->unpolish(inputBox_);
	inputBox_->style()->polish(inputBox_);

	statusLabel_->style()->unpolish(statusLabel_);
	statusLabel_->style()->polish(statusLabel_);

	if (isEmpty) {
		statusLabel_->setText("Enter at least one ASCII character.");
	}
	else if (!isAscii) {
		statusLabel_->setText("Enter only ASCII characters.");
	}
	else {
		statusLabel_->setText("Ready to convert.");
	}

	clearOutputs();
}

bool MainWindow::containsOnlyAscii(const QString& s) {
	return std::ranges::all_of(s, [](const QChar c) {
		return c.unicode() <= 255;
	});
}