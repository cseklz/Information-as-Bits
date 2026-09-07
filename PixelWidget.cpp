#include "PixelWidget.h"

#include <QByteArray>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QImageReader>
#include <QLabel>
#include <QLocale>
#include <QMessageBox>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QSaveFile>
#include <QSize>
#include <QSplitter>
#include <QString>
#include <QStringList>
#include <QStyle>
#include <QVBoxLayout>

#include <cstddef>
#include <cstdint>
#include <string_view>

PixelWidget::PixelWidget(QWidget* parent)
    : QWidget(parent) {
    createInterface();
}

void PixelWidget::createInterface() {
    setObjectName("pixelPage");

    auto* pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(20, 20, 20, 20);

    auto* card = new QFrame;
    card->setProperty("card", true);
    card->setMaximumWidth(1100);

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(30, 28, 30, 30);
    cardLayout->setSpacing(10);

    auto* title = new QLabel("Pixel Image Codec");
    title->setProperty("role", "pageTitle");

    auto* description = new QLabel(
        "Read image pixels as text or turn valid pixel text back into an image. "
        "RGBA values use web-style #RRGGBBAA notation.");
    description->setProperty("role", "description");
    description->setWordWrap(true);

    auto* actions = new QHBoxLayout;
    actions->setSpacing(8);

    auto* importTextButton = new QPushButton("Import Text");
    importTextButton->setProperty("role", "secondary");
    importTextButton->setCursor(Qt::PointingHandCursor);

    auto* importImageButton = new QPushButton("Import Image");
    importImageButton->setProperty("role", "secondary");
    importImageButton->setCursor(Qt::PointingHandCursor);

    exportButton_ = new QPushButton("Export Text");
    exportButton_->setProperty("role", "primary");
    exportButton_->setCursor(Qt::PointingHandCursor);
    exportButton_->setEnabled(false);

    actions->addWidget(importTextButton);
    actions->addWidget(importImageButton);
    actions->addStretch();
    actions->addWidget(exportButton_);

    metadataLabel_ = new QLabel("No image loaded");
    metadataLabel_->setProperty("role", "metadata");

    auto* splitter = new QSplitter(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);

    preview_ = new QLabel("Import an image or pixel text to begin.");
    preview_->setProperty("preview", true);
    preview_->setAlignment(Qt::AlignCenter);
    preview_->setMinimumSize(250, 250);
    preview_->setWordWrap(true);

    pixelText_ = new QPlainTextEdit;
    pixelText_->setReadOnly(true);
    pixelText_->setProperty("monospace", true);
    pixelText_->setPlaceholderText("Pixel values appear here.");
    pixelText_->setMinimumSize(300, 250);
    pixelText_->setLineWrapMode(QPlainTextEdit::NoWrap);

    splitter->addWidget(preview_);
    splitter->addWidget(pixelText_);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    statusLabel_ = new QLabel("Import text or an image to begin.");
    statusLabel_->setProperty("role", "status");
    statusLabel_->setProperty("state", "neutral");
    statusLabel_->setWordWrap(true);

    cardLayout->addWidget(title);
    cardLayout->addWidget(description);
    cardLayout->addSpacing(4);
    cardLayout->addLayout(actions);
    cardLayout->addWidget(metadataLabel_);
    cardLayout->addWidget(splitter, 1);
    cardLayout->addWidget(statusLabel_);

    pageLayout->addWidget(card, 1, Qt::AlignHCenter);

    connect(importTextButton, &QPushButton::clicked, this, &PixelWidget::importText);
    connect(importImageButton, &QPushButton::clicked, this, &PixelWidget::importImage);
    connect(exportButton_, &QPushButton::clicked, this, &PixelWidget::exportText);
    connect(splitter, &QSplitter::splitterMoved, this, [this](int, int) {
        updatePreview();
    });
}

void PixelWidget::importText() {
    const QString fileName = QFileDialog::getOpenFileName(
        this,
        "Import Pixel Text",
        {},
        "Pixel text (*.txt *.iab);;All files (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        showError("Could not open the selected text file: " + file.errorString());
        return;
    }
    if (file.size() > static_cast<qint64>(PixelCodec::maximumTextBytes)) {
        showError("The selected file exceeds the 12 MB safety limit.");
        return;
    }

    const QByteArray bytes = file.read(
        static_cast<qint64>(PixelCodec::maximumTextBytes + 1U));
    if (file.error() != QFileDevice::NoError) {
        showError("Could not finish reading the selected text file: " + file.errorString());
        return;
    }
    if (static_cast<std::uint64_t>(bytes.size()) > PixelCodec::maximumTextBytes) {
        showError("The selected file exceeds the 12 MB safety limit.");
        return;
    }
    const auto byteCount = static_cast<std::size_t>(bytes.size());
    const PixelCodec::DecodeResult decoded = PixelCodec::decode(
        std::string_view{bytes.constData(), byteCount});
    if (!decoded.success) {
        showError(QString::fromStdString(decoded.errorMessage));
        return;
    }

    const PixelCodec::EncodeResult canonical = PixelCodec::encode(decoded.image);
    if (!canonical.success) {
        showError(QString::fromStdString(canonical.errorMessage));
        return;
    }

    const QImage image = createImage(decoded.image);
    if (image.isNull()) {
        showError("The image could not be allocated.");
        return;
    }

    setCurrentImage(image, QString::fromStdString(canonical.text));
    setStatus("Pixel text imported and rendered as an image.", "success");
}

void PixelWidget::importImage() {
    QStringList patterns;
    for (const QByteArray& format : QImageReader::supportedImageFormats()) {
        patterns.append(QStringLiteral("*.") + QString::fromLatin1(format).toLower());
    }
    patterns.removeDuplicates();
    const QString imageFilter = patterns.isEmpty()
        ? QStringLiteral("All files (*)")
        : QStringLiteral("Images (%1);;All files (*)").arg(patterns.join(' '));

    const QString fileName = QFileDialog::getOpenFileName(
        this,
        "Import Image",
        {},
        imageFilter);
    if (fileName.isEmpty()) {
        return;
    }

    QImageReader reader(fileName);
    reader.setAutoTransform(true);

    const QSize declaredSize = reader.size();
    if (declaredSize.isValid()) {
        const std::uint64_t declaredPixelCount
            = static_cast<std::uint64_t>(declaredSize.width())
            * static_cast<std::uint64_t>(declaredSize.height());
        if (declaredPixelCount > PixelCodec::maximumPixelCount) {
            showError("The image exceeds the 1,000,000-pixel limit.");
            return;
        }
    }

    const QImage image = reader.read();
    if (image.isNull()) {
        showError("Could not read the selected image: " + reader.errorString());
        return;
    }

    const std::uint64_t pixelCount = static_cast<std::uint64_t>(image.width())
        * static_cast<std::uint64_t>(image.height());
    if (pixelCount > PixelCodec::maximumPixelCount) {
        showError("The image exceeds the 1,000,000-pixel limit.");
        return;
    }

    const PixelImage pixels = extractPixels(image);
    const PixelCodec::EncodeResult encoded = PixelCodec::encode(pixels);
    if (!encoded.success) {
        showError(QString::fromStdString(encoded.errorMessage));
        return;
    }

    setCurrentImage(
        image.convertToFormat(QImage::Format_RGBA8888),
        QString::fromStdString(encoded.text));
    setStatus("Image imported and converted to pixel text.", "success");
}

void PixelWidget::exportText() {
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Export Pixel Text",
        "pixels.txt",
        "Text files (*.txt);;All files (*)");
    if (fileName.isEmpty()) {
        return;
    }
    if (QFileInfo(fileName).suffix().isEmpty()) {
        fileName += ".txt";
    }

    QSaveFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        showError("Could not open the export file: " + file.errorString());
        return;
    }

    const QByteArray bytes = pixelText_->toPlainText().toUtf8();
    if ((file.write(bytes) != bytes.size()) || !file.commit()) {
        showError("Could not finish writing the pixel text file.");
        return;
    }
    setStatus("Pixel text exported successfully.", "success");
}

void PixelWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    updatePreview();
}

void PixelWidget::updatePreview() {
    if (currentImage_.isNull()) {
        return;
    }

    QSize targetSize = preview_->contentsRect().size() - QSize(24, 24);
    targetSize.setWidth(qMax(targetSize.width(), 1));
    targetSize.setHeight(qMax(targetSize.height(), 1));
    const QPixmap pixmap = currentPixmap_.scaled(
        targetSize,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    preview_->setText({});
    preview_->setPixmap(pixmap);
}

void PixelWidget::setCurrentImage(const QImage& image, const QString& pixelText) {
    currentImage_ = image;
    currentPixmap_ = QPixmap::fromImage(currentImage_);
    pixelText_->setPlainText(pixelText);
    exportButton_->setEnabled(true);

    const auto pixelCount = static_cast<qulonglong>(image.width())
        * static_cast<qulonglong>(image.height());
    metadataLabel_->setText(
        QStringLiteral("%1 × %2  •  RGBA8  •  %3 pixels")
            .arg(image.width())
            .arg(image.height())
            .arg(QLocale().toString(pixelCount)));
    updatePreview();
}

void PixelWidget::setStatus(const QString& message, const char* state) {
    statusLabel_->setText(message);
    statusLabel_->setProperty("state", state);
    statusLabel_->style()->unpolish(statusLabel_);
    statusLabel_->style()->polish(statusLabel_);
}

void PixelWidget::showError(const QString& message) {
    setStatus(message, "error");
    QMessageBox::warning(this, "Pixel Image Codec", message);
}

PixelImage PixelWidget::extractPixels(const QImage& image) {
    const QImage rgbaImage = image.convertToFormat(QImage::Format_RGBA8888);
    PixelImage result;
    result.width = static_cast<std::uint32_t>(rgbaImage.width());
    result.height = static_cast<std::uint32_t>(rgbaImage.height());
    result.pixels.reserve(
        static_cast<std::size_t>(result.width) * static_cast<std::size_t>(result.height));

    for (int row = 0; row < rgbaImage.height(); ++row) {
        const auto* rowBytes = rgbaImage.constScanLine(row);
        for (int column = 0; column < rgbaImage.width(); ++column) {
            const int offset = column * 4;
            result.pixels.push_back({
                rowBytes[offset],
                rowBytes[offset + 1],
                rowBytes[offset + 2],
                rowBytes[offset + 3]
            });
        }
    }
    return result;
}

QImage PixelWidget::createImage(const PixelImage& image) {
    QImage result(
        static_cast<int>(image.width),
        static_cast<int>(image.height),
        QImage::Format_RGBA8888);
    if (result.isNull()) {
        return {};
    }

    for (std::uint32_t row = 0U; row < image.height; ++row) {
        auto* rowBytes = result.scanLine(static_cast<int>(row));
        for (std::uint32_t column = 0U; column < image.width; ++column) {
            const std::size_t pixelIndex = (static_cast<std::size_t>(row) * image.width)
                + column;
            const Pixel& pixel = image.pixels[pixelIndex];
            const std::size_t offset = static_cast<std::size_t>(column) * 4U;
            rowBytes[offset] = pixel.red;
            rowBytes[offset + 1U] = pixel.green;
            rowBytes[offset + 2U] = pixel.blue;
            rowBytes[offset + 3U] = pixel.alpha;
        }
    }
    return result;
}
