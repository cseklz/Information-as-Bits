#include "HomePage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace {
QPushButton* createToolButton(const QString& text, const bool enabled) {
    auto* button = new QPushButton(text);
    button->setProperty("role", "tool");
    button->setCursor(enabled ? Qt::PointingHandCursor : Qt::ArrowCursor);
    button->setMinimumHeight(86);
    button->setEnabled(enabled);
    return button;
}
}

HomePage::HomePage(QWidget* parent) : QWidget(parent) {
    setObjectName("homePage");

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(36, 32, 36, 40);
    layout->setSpacing(12);

    auto* title = new QLabel("Information as Bits");
    title->setProperty("role", "homeTitle");

    auto* description = new QLabel("Explore how computers represent text, numbers, and images.");
    description->setProperty("role", "description");
    description->setWordWrap(true);

    auto* asciiButton = createToolButton(
        "ASCII to Decimal\nConvert Latin-1 characters to values from 0 to 255.", true);
    auto* baseButton = createToolButton(
        "Number Base Converter\nConvert binary, octal, decimal, and hexadecimal.", true);
    auto* pixelButton = createToolButton(
        "Pixel Image Codec\nImport images or pixel text, then export pixel text.", true);

    layout->addStretch(1);
    layout->addWidget(title);
    layout->addWidget(description);
    layout->addSpacing(14);
    layout->addWidget(asciiButton);
    layout->addWidget(baseButton);
    layout->addWidget(pixelButton);
    layout->addStretch(1);

    connect(asciiButton, &QPushButton::clicked, this, &HomePage::asciiRequested);
    connect(
        baseButton,
        &QPushButton::clicked,
        this,
        &HomePage::baseConverterRequested);
    connect(pixelButton, &QPushButton::clicked, this, &HomePage::pixelsRequested);
}
