#include "ConverterWidget.h"
#include "Conversions.h"

#include <algorithm>
#include <QButtonGroup>
#include <QByteArray>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QStyle>
#include <QWidget>

ConverterWidget::ConverterWidget(QWidget *parent) : QWidget(parent) {
	createInterface();
}

void ConverterWidget::createInterface() {
	setObjectName("ConverterWidget");
	auto* pageLayout = new QVBoxLayout(this);
	pageLayout->setContentsMargins(28, 24, 28, 24);

	auto* card = new QFrame;
	card->setObjectName("card");


	auto* cardLayout = new QVBoxLayout(card);
	cardLayout->setContentsMargins(32, 28, 32, 28);
	cardLayout->setSpacing(12);

	auto* modeLabel = new QLabel("Input Type");
	modeLabel->setObjectName("mode");

	auto* modeLayout = new QHBoxLayout;
	modeLayout->setSpacing(8);

	inputLabel_ = new QLabel("ASCII");
	inputLabel_->setObjectName("input");

	inputModeGroup_ = new QButtonGroup(this);
	inputModeGroup_->setExclusive(true);

	auto addModeButton = [this, modeLayout](const QString& label, const InputMode mode) {
		auto* button = new QPushButton(label);

		button->setCheckable(true);
		button->setProperty("modeButton", true);

		inputModeGroup_->addButton(button, static_cast<int>(mode));
		modeLayout->addWidget(button);

		return button;
	};

	auto* asciiButton = addModeButton("ASCII", InputMode::ascii);
	addModeButton("Decimal", InputMode::decimal);
	addModeButton("Binary", InputMode::binary);
	addModeButton("Octal", InputMode::octal);
	addModeButton("Hexadecimal", InputMode::hexadecimal);

	inputBox_ = new QLineEdit;
	inputBox_->setPlaceholderText("Enter value(s) to convert");
	inputBox_->setClearButtonEnabled(true);

	statusLabel_ = new QLabel("Enter at least one value");
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

	asciiOutput_ = createOutputField();
	decimalOutput_ = createOutputField();
	binaryOutput_ = createOutputField();
	octalOutput_ = createOutputField();
	hexadecimalOutput_ = createOutputField();

	asciiOutputLabel_ = new QLabel("ASCII");
	decimalOutputLabel_ = new QLabel("Decimal");
	binaryOutputLabel_ = new QLabel("Binary");
	octalOutputLabel_ = new QLabel("Octal");
	hexadecimalOutputLabel_ = new QLabel("Hexadecimal");

	auto* outputGrid = new QGridLayout;

	outputGrid->setHorizontalSpacing(12);
	outputGrid->setVerticalSpacing(8);
	outputGrid->setColumnStretch(1, 1);

	outputGrid->addWidget(asciiOutputLabel_, 0, 0);
	outputGrid->addWidget(asciiOutput_, 0, 1);

	outputGrid->addWidget(decimalOutputLabel_, 1, 0);
	outputGrid->addWidget(decimalOutput_, 1, 1);

	outputGrid->addWidget(binaryOutputLabel_, 2, 0);
	outputGrid->addWidget(binaryOutput_, 2, 1);

	outputGrid->addWidget(octalOutputLabel_, 3, 0);
	outputGrid->addWidget(octalOutput_, 3, 1);

	outputGrid->addWidget(hexadecimalOutputLabel_, 4, 0);
	outputGrid->addWidget(hexadecimalOutput_, 4, 1);

	cardLayout->addWidget(inputLabel_);
	cardLayout->addSpacing(8);

	cardLayout->addWidget(modeLabel);
	cardLayout->addLayout(modeLayout);
	cardLayout->addSpacing(8);

	cardLayout->addWidget(inputBox_);
	cardLayout->addWidget(statusLabel_);
	cardLayout->addWidget(convertButton_);

	cardLayout->addSpacing(10);
	cardLayout->addWidget(outputLabel);
	cardLayout->addLayout(outputGrid);

	pageLayout->addStretch();
	pageLayout->addWidget(card);
	pageLayout->addStretch();

	connect(
		convertButton_,
		&QPushButton::clicked,
		this,
		&ConverterWidget::handleConversion);

	connect(
		inputBox_,
		&QLineEdit::textChanged,
		this,
		&ConverterWidget::updateInputState);

	connect(
		inputBox_,
		&QLineEdit::returnPressed,
		convertButton_,
		&QPushButton::click);

	connect(
		inputModeGroup_,
		&QButtonGroup::idClicked,
		this,
		&ConverterWidget::handleInputMode);

	asciiButton->setChecked(true);
	setInputMode(InputMode::ascii);
}

void ConverterWidget::updateOutputVisibility() {
	const bool showAscii = inputMode_ != InputMode::ascii;
	const bool showDecimal = inputMode_ != InputMode::decimal;
	const bool showBinary = inputMode_ != InputMode::binary;
	const bool showOctal = inputMode_ != InputMode::octal;
	const bool showHexadecimal = inputMode_ != InputMode::hexadecimal;

	asciiOutputLabel_->setVisible(showAscii);
	asciiOutput_->setVisible(showAscii);

	decimalOutputLabel_->setVisible(showDecimal);
	decimalOutput_->setVisible(showDecimal);

	binaryOutputLabel_->setVisible(showBinary);
	binaryOutput_->setVisible(showBinary);

	octalOutputLabel_->setVisible(showOctal);
	octalOutput_->setVisible(showOctal);

	hexadecimalOutputLabel_->setVisible(showHexadecimal);
	hexadecimalOutput_->setVisible(showHexadecimal);
}

void ConverterWidget::clearOutputs() {
	asciiOutput_->clear();
	decimalOutput_->clear();
	binaryOutput_->clear();
	octalOutput_->clear();
	hexadecimalOutput_->clear();
}


void ConverterWidget::setInputMode(const InputMode mode) {
	inputMode_ = mode;

	inputBox_->clear();
	clearOutputs();
	updateOutputVisibility();

	switch (inputMode_) {
		case InputMode::ascii:
			inputLabel_->setText("ASCII");
			inputBox_->setPlaceholderText("Enter ASCII character.");
			statusLabel_->setText("Enter at least one character.");
			break;
		case InputMode::decimal:
			inputLabel_->setText("Decimal");
			inputBox_->setPlaceholderText("Enter decimal value.");
			statusLabel_->setText("Enter an unsigned integer.");
			break;
		case InputMode::binary:
			inputLabel_->setText("Binary");
			inputBox_->setPlaceholderText("Enter binary value.");
			statusLabel_->setText("Enter a number using 0 and 1.");
			break;
		case InputMode::octal:
			inputLabel_->setText("Octal");
			inputBox_->setPlaceholderText("Enter octal value.");
			statusLabel_->setText("Enter a number using 0 through 7.");
			break;
		case InputMode::hexadecimal:
			inputLabel_->setText("Hexadecimal");
			inputBox_->setPlaceholderText("Enter hexadecimal value.");
			statusLabel_->setText("Enter a number using 0-9 and A-F.");
			break;
	}

	convertButton_->setEnabled(false);
}

void ConverterWidget::updateInputState(const QString& s) {
	clearOutputs();

	const bool isEmpty = s.isEmpty();

	if (inputMode_ != InputMode::ascii) {
		convertButton_->setEnabled(false);

		inputBox_->setProperty("invalid", false);
		statusLabel_->setProperty("invalid", false);

		if (isEmpty) {
			statusLabel_->setText("Enter a number to convert.");
		}
		else {
			statusLabel_->setText(
				"This input mode is not implemented yet."
			);
		}

		inputBox_->style()->unpolish(inputBox_);
		inputBox_->style()->polish(inputBox_);

		statusLabel_->style()->unpolish(statusLabel_);
		statusLabel_->style()->polish(statusLabel_);

		return;
	}

	const bool containsOnlyValidAscii = containsOnlyAscii(s);
	const bool isValid = !isEmpty && containsOnlyValidAscii;
	const bool isInvalid = !isEmpty && !containsOnlyValidAscii;

	convertButton_->setEnabled(isValid);

	inputBox_->setProperty("invalid", isInvalid);
	statusLabel_->setProperty("invalid", isInvalid);

	inputBox_->style()->unpolish(inputBox_);
	inputBox_->style()->polish(inputBox_);

	statusLabel_->style()->unpolish(statusLabel_);
	statusLabel_->style()->polish(statusLabel_);

	if (isEmpty) {
		statusLabel_->setText(
			"Enter at least one ASCII character."
		);
	}
	else if (isInvalid) {
		statusLabel_->setText(
			"Enter only ASCII characters."
		);
	}
	else {
		statusLabel_->setText("Ready to convert.");
	}
}

void ConverterWidget::handleInputMode(const int id) {
	const auto mode = static_cast<InputMode>(id);
	setInputMode(mode);
}

void ConverterWidget::handleConversion() {
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

bool ConverterWidget::containsOnlyAscii(const QString& s) {
	return std::ranges::all_of(s, [](const QChar c) {
		return c.unicode() <= 255;
	});
}