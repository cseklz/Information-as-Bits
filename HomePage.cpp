#include "HomePage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace {
	QPushButton* createToolButton(const QString& text) {
		auto* button = new QPushButton(text);
		button->setProperty("role", "tool");
		button->setCursor(Qt::PointingHandCursor);
		button->setMinimumHeight(94);
		return button;
	}
} //namespace

HomePage::HomePage(QWidget* parent) : QWidget(parent) {
	auto* layout = new QVBoxLayout(this);
	layout->setContentsMargins(32, 28, 32, 36);
	layout->setSpacing(12);

	auto* title = new QLabel("Information as Bits");
	title->setProperty("role", "homeTitle");

	auto* description = new QLabel("Explore different ways to store and read data");
	description->setProperty("role", "description");
	description-> setWordWrap(true);

	auto* asciiButton = createToolButton(
		 "ASCII to Decimal\nConvert ASCII characters to decimal.");
	auto* baseButton = createToolButton(
		 "Number Base Converter\nConvert binary, octal, decimal, and hexadecimal.");
	auto* pixelButton = createToolButton(
		 "Pixel Image Codec\nImport and export images or import pixel text.");

	layout->addStretch();
	layout->addWidget(title);
	layout->addWidget(description);
	layout->addSpacing(14);

	layout->addWidget(asciiButton);
	layout->addWidget(baseButton);
	layout->addWidget(pixelButton);
	layout->addStretch();

	connect(
		asciiButton,
		&QPushButton::clicked,
		this,
		&HomePage::asciiRequested);

	connect(
		baseButton,
		&QPushButton::clicked,
		this,
		&HomePage::baseConverterRequested);

	connect(
		pixelButton,
		&QPushButton::clicked,
		this,
		&HomePage::pixelsRequested);
}