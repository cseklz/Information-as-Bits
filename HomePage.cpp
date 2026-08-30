#include "HomePage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

HomePage::HomePage(QWidget* parent) : QWidget(parent) {
	auto* layout = new QVBoxLayout(this);

	layout->setContentsMargins(32, 32, 32, 32);
	layout->setSpacing(16);

	auto* title = new QLabel("Information as Bits");
	title->setObjectName("homeTitle");

	auto* description = new QLabel("Choose between conversion and image hex code import and export.");
	description->setObjectName("homeDescription");
	description-> setWordWrap(true);

	auto* conversionButton = new QPushButton("Conversions\n"
		"Convert ASCII, decimal, binary, octal, and hexadecimal.");

	conversionButton->setProperty("toolOption", true);
	conversionButton->setCursor(Qt::PointingHandCursor);
	conversionButton->setMinimumHeight(110);

	auto* pictureButton = new QPushButton("Picture\n"
		"Read/write pixel values or create an image from writen pixel data.");

	pictureButton->setProperty("toolOption", true);
	pictureButton->setCursor(Qt::PointingHandCursor);
	pictureButton->setMinimumHeight(110);

	layout->addStretch();
	layout->addWidget(title);
	layout->addWidget(description);
	layout->addSpacing(16);

	layout->addWidget(conversionButton);
	layout->addWidget(pictureButton);
	layout->addStretch();

	connect(
		conversionButton,
		&QPushButton::clicked,
		this,
		&HomePage::conversionRequested);

	connect(
		pictureButton,
		&QPushButton::clicked,
		this,
		&HomePage::pictureRequested);
}