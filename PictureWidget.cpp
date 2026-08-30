#include "PictureWidget.h"

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

PictureWidget::PictureWidget(QWidget* parent) : QWidget(parent) {
	auto* pageLayout = new QVBoxLayout(this);

	pageLayout->setContentsMargins(0,0,0,0);

	auto* card = new QFrame(this);
	card->setObjectName("card");

	auto* cardLayout = new QVBoxLayout(card);

	cardLayout->setContentsMargins(32, 28, 32, 28);
	cardLayout->setSpacing(12);

	auto* title = new QLabel("Hexadecimal Picture System");
	title->setObjectName("pictureTitle");

	auto* description = new QLabel(
		"Read pixel values from an image or create an image "
		"from hexadecimal pixel data.");

	description->setObjectName("pictureDescription");
	description->setWordWrap(true);

	auto* placeholder = new QLabel("Picture functionality will be implemented here");

	placeholder->setObjectName("picturePlaceholder");
	placeholder->setAlignment(Qt::AlignCenter);
	placeholder->setMinimumHeight(260);

	cardLayout->addWidget(title);
	cardLayout->addWidget(description);
	cardLayout->addSpacing(12);
	cardLayout->addWidget(placeholder);

	pageLayout->addStretch();
	pageLayout->addWidget(card);
	pageLayout->addStretch();
}