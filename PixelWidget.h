#ifndef PIXEL_WIDGET_H
#define PIXEL_WIDGET_H

#include "PixelCodec.h"

#include <QImage>
#include <QPixmap>
#include <QWidget>

class QLabel;
class QPlainTextEdit;
class QPushButton;
class QResizeEvent;
class QString;

class PixelWidget final : public QWidget {
public:
    explicit PixelWidget(QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void createInterface();
    void importText();
    void importImage();
    void exportText();
    void updatePreview();
    void setCurrentImage(const QImage& image, const QString& pixelText);
    void setStatus(const QString& message, const char* state);
    void showError(const QString& message);

    [[nodiscard]] static PixelImage extractPixels(const QImage& image);
    [[nodiscard]] static QImage createImage(const PixelImage& image);

    QImage currentImage_;
    QPixmap currentPixmap_;
    QLabel* preview_{nullptr};
    QLabel* metadataLabel_{nullptr};
    QLabel* statusLabel_{nullptr};
    QPlainTextEdit* pixelText_{nullptr};
    QPushButton* exportButton_{nullptr};
};

#endif
