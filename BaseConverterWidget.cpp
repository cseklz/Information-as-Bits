#include "BaseConverterWidget.h"

#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QStyle>
#include <QVBoxLayout>
#include <QVariant>

#include <string>

namespace {

void addBaseItems(QComboBox* box) {
    box->addItem("Binary (base 2)", 2U);
    box->addItem("Octal (base 8)", 8U);
    box->addItem("Decimal (base 10)", 10U);
    box->addItem("Hexadecimal (base 16)", 16U);
}

} // namespace

BaseConverterWidget::BaseConverterWidget(QWidget* parent)
    : QWidget(parent) {
    createInterface();
}

void BaseConverterWidget::createInterface() {
    setObjectName("baseConverterPage");

    auto* pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(20, 20, 20, 20);

    auto* card = new QFrame;
    card->setProperty("card", true);
    card->setMaximumWidth(920);

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(26, 24, 26, 24);
    cardLayout->setSpacing(8);

    auto* title = new QLabel("Number Base Converter");
    title->setProperty("role", "pageTitle");

    auto* description = new QLabel(
        "Choose independent input and output bases. Enter multiple values separated "
        "by spaces or new lines, similar to hexc.");
    description->setProperty("role", "description");
    description->setWordWrap(true);

    auto* baseRow = new QHBoxLayout;
    baseRow->setSpacing(10);

    auto* fromColumn = new QVBoxLayout;
    auto* fromLabel = new QLabel("From");
    fromLabel->setProperty("role", "fieldLabel");
    inputBase_ = new QComboBox;
    addBaseItems(inputBase_);
    inputBase_->setCurrentIndex(2);
    fromColumn->addWidget(fromLabel);
    fromColumn->addWidget(inputBase_);

    auto* swapButton = new QPushButton("Swap");
    swapButton->setProperty("role", "secondary");
    swapButton->setCursor(Qt::PointingHandCursor);

    auto* toColumn = new QVBoxLayout;
    auto* toLabel = new QLabel("To");
    toLabel->setProperty("role", "fieldLabel");
    outputBase_ = new QComboBox;
    addBaseItems(outputBase_);
    outputBase_->setCurrentIndex(3);
    toColumn->addWidget(toLabel);
    toColumn->addWidget(outputBase_);

    baseRow->addLayout(fromColumn, 1);
    baseRow->addWidget(swapButton, 0, Qt::AlignBottom);
    baseRow->addLayout(toColumn, 1);

    auto* optionsRow = new QHBoxLayout;
    optionsRow->setSpacing(10);

    auto* representationColumn = new QVBoxLayout;
    auto* representationLabel = new QLabel("Interpretation");
    representationLabel->setProperty("role", "fieldLabel");
    representation_ = new QComboBox;
    representation_->addItem("Unsigned 64-bit", 0);
    representation_->addItem("Two's complement", 1);
    representationColumn->addWidget(representationLabel);
    representationColumn->addWidget(representation_);

    auto* widthColumn = new QVBoxLayout;
    auto* widthLabel = new QLabel("Word size");
    widthLabel->setProperty("role", "fieldLabel");
    bitWidth_ = new QComboBox;
    bitWidth_->addItem("8 bits", 8U);
    bitWidth_->addItem("16 bits", 16U);
    bitWidth_->addItem("32 bits", 32U);
    bitWidth_->addItem("64 bits", 64U);
    widthColumn->addWidget(widthLabel);
    widthColumn->addWidget(bitWidth_);

    optionsRow->addLayout(representationColumn, 1);
    optionsRow->addLayout(widthColumn, 1);

    auto* inputLabel = new QLabel("Values");
    inputLabel->setProperty("role", "fieldLabel");

    input_ = new QPlainTextEdit;
    input_->setProperty("monospace", true);
    input_->setPlaceholderText("Example: 42 255 1024");
    input_->setMinimumHeight(80);
    input_->setMaximumHeight(110);

    statusLabel_ = new QLabel("Enter at least one value.");
    statusLabel_->setProperty("role", "status");
    statusLabel_->setProperty("state", "neutral");
    statusLabel_->setWordWrap(true);

    convertButton_ = new QPushButton("Convert Values");
    convertButton_->setProperty("role", "primary");
    convertButton_->setCursor(Qt::PointingHandCursor);
    convertButton_->setEnabled(false);

    auto* outputLabel = new QLabel("Result");
    outputLabel->setProperty("role", "fieldLabel");

    output_ = new QPlainTextEdit;
    output_->setReadOnly(true);
    output_->setProperty("monospace", true);
    output_->setPlaceholderText("Converted values appear here.");
    output_->setMinimumHeight(90);

    cardLayout->addWidget(title);
    cardLayout->addWidget(description);
    cardLayout->addSpacing(6);
    cardLayout->addLayout(baseRow);
    cardLayout->addLayout(optionsRow);
    cardLayout->addSpacing(4);
    cardLayout->addWidget(inputLabel);
    cardLayout->addWidget(input_);
    cardLayout->addWidget(statusLabel_);
    cardLayout->addWidget(convertButton_);
    cardLayout->addSpacing(4);
    cardLayout->addWidget(outputLabel);
    cardLayout->addWidget(output_);

    pageLayout->addStretch(1);
    pageLayout->addWidget(card, 0, Qt::AlignHCenter);
    pageLayout->addStretch(1);

    connect(input_, &QPlainTextEdit::textChanged, this, &BaseConverterWidget::clearResult);
    connect(convertButton_, &QPushButton::clicked, this, &BaseConverterWidget::convertValues);
    connect(swapButton, &QPushButton::clicked, this, &BaseConverterWidget::swapBases);
    connect(
        representation_,
        &QComboBox::currentIndexChanged,
        this,
        &BaseConverterWidget::updateRepresentationControls);
    connect(inputBase_, &QComboBox::currentIndexChanged, this, &BaseConverterWidget::clearResult);
    connect(outputBase_, &QComboBox::currentIndexChanged, this, &BaseConverterWidget::clearResult);
    connect(bitWidth_, &QComboBox::currentIndexChanged, this, &BaseConverterWidget::clearResult);

    updateRepresentationControls();
}

void BaseConverterWidget::clearResult() {
    output_->clear();
    const bool hasInput = !input_->toPlainText().trimmed().isEmpty();
    convertButton_->setEnabled(hasInput);
    setStatus(hasInput ? "Ready to convert." : "Enter at least one value.", "neutral");
}

void BaseConverterWidget::convertValues() {
    const std::string inputText = input_->toPlainText().toStdString();
    const auto mode = representation_->currentData().toInt() == 0
        ? BaseConverter::Representation::unsignedInteger
        : BaseConverter::Representation::twosComplement;
    const unsigned int width = bitWidth_->currentData().toUInt();

    const BaseConverter::Result result = BaseConverter::convert(
        inputText,
        selectedBase(inputBase_),
        selectedBase(outputBase_),
        mode,
        width);

    if (!result.success) {
        output_->clear();
        setStatus(QString::fromStdString(result.errorMessage), "error");
        return;
    }

    QString outputText;
    for (const BaseConverter::ConvertedValue& value : result.values) {
        if (!outputText.isEmpty()) {
            outputText += '\n';
        }
        outputText += QStringLiteral("%1  →  %2")
            .arg(QString::fromStdString(value.input), QString::fromStdString(value.output));
    }
    output_->setPlainText(outputText);
    setStatus(
        result.values.size() == 1U
            ? "Converted 1 value."
            : QStringLiteral("Converted %1 values.")
                  .arg(static_cast<qulonglong>(result.values.size())),
        "success");
}

void BaseConverterWidget::swapBases() {
    const int previousInput = inputBase_->currentIndex();
    inputBase_->setCurrentIndex(outputBase_->currentIndex());
    outputBase_->setCurrentIndex(previousInput);
    clearResult();
}

void BaseConverterWidget::updateRepresentationControls() {
    const bool usesTwosComplement = representation_->currentData().toInt() != 0;
    bitWidth_->setEnabled(usesTwosComplement);
    clearResult();
}

void BaseConverterWidget::setStatus(const QString& message, const char* state) const {
    statusLabel_->setText(message);
    statusLabel_->setProperty("state", state);
    statusLabel_->style()->unpolish(statusLabel_);
    statusLabel_->style()->polish(statusLabel_);
}

BaseConverter::Base BaseConverterWidget::selectedBase(const QComboBox* box) {
    return static_cast<BaseConverter::Base>(box->currentData().toUInt());
}
