#include "AsciiWidget.h"

#include "AsciiConverter.h"

#include <QFrame>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QStyle>
#include <QVBoxLayout>

#include <string>

AsciiWidget::AsciiWidget(QWidget* parent) : QWidget(parent) {
    createInterface();
}

void AsciiWidget::createInterface() {
    setObjectName("asciiPage");

    auto* pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(16, 16, 16, 16);

    auto* card = new QFrame;
    card->setProperty("card", true);
    card->setMaximumWidth(920);

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(32, 30, 32, 32);
    cardLayout->setSpacing(10);

    auto* title = new QLabel("ASCII to Decimal");
    title->setProperty("role", "pageTitle");

    auto* description = new QLabel(
        "Convert Latin-1 characters (values 0 through 255) into decimal bytes.");
    description->setProperty("role", "description");
    description->setWordWrap(true);

    auto* inputLabel = new QLabel("Latin-1 text");
    inputLabel->setProperty("role", "fieldLabel");

    input_ = new QPlainTextEdit;
    input_->setProperty("monospace", true);
    input_->setPlaceholderText("Example: café");
    input_->setMinimumHeight(105);
    input_->setMaximumHeight(145);

    statusLabel_ = new QLabel("Enter at least one Latin-1 character.");
    statusLabel_->setProperty("role", "status");
    statusLabel_->setProperty("state", "neutral");
    statusLabel_->setWordWrap(true);

    convertButton_ = new QPushButton("Convert to Decimal");
    convertButton_->setProperty("role", "primary");
    convertButton_->setCursor(Qt::PointingHandCursor);
    convertButton_->setEnabled(false);

    auto* outputLabel = new QLabel("Decimal values");
    outputLabel->setProperty("role", "fieldLabel");

    output_ = new QPlainTextEdit;
    output_->setReadOnly(true);
    output_->setProperty("monospace", true);
    output_->setPlaceholderText("Results appear here");
    output_->setMinimumHeight(105);
    output_->setMaximumHeight(145);

    cardLayout->addWidget(title);
    cardLayout->addWidget(description);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(inputLabel);
    cardLayout->addWidget(input_);
    cardLayout->addWidget(statusLabel_);
    cardLayout->addWidget(convertButton_);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(outputLabel);
    cardLayout->addWidget(output_);

    pageLayout->addStretch(1);
    pageLayout->addWidget(card, 0, Qt::AlignHCenter);
    pageLayout->addStretch(1);

    connect(input_, &QPlainTextEdit::textChanged, this, &AsciiWidget::clearResult);
    connect(convertButton_, &QPushButton::clicked, this, &AsciiWidget::convertText);
}

void AsciiWidget::clearResult() {
    output_->clear();
    const bool hasInput = !input_->toPlainText().isEmpty();
    convertButton_->setEnabled(hasInput);
    setStatus(
        hasInput ? "Ready to convert." : "Enter at least one Latin-1 character.",
        "neutral");
}

void AsciiWidget::convertText() {
    const QList<uint> codePoints = input_->toPlainText().toUcs4();
    std::u32string text;
    text.reserve(static_cast<std::size_t>(codePoints.size()));

    for (const uint codePoint : codePoints) {
        text.push_back(static_cast<char32_t>(codePoint));
    }

    const AsciiConverter::Result result = AsciiConverter::toDecimal(text);
    if (!result.success) {
        output_->clear();
        setStatus(QString::fromStdString(result.errorMessage), "error");
        return;
    }

    output_->setPlainText(QString::fromStdString(result.decimalValues));
    setStatus("Conversion complete.", "success");
}

void AsciiWidget::setStatus(const QString& message, const char* state) {
    statusLabel_->setText(message);
    statusLabel_->setProperty("state", state);
    statusLabel_->style()->unpolish(statusLabel_);
    statusLabel_->style()->polish(statusLabel_);
}
