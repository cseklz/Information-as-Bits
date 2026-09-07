#ifndef BASE_CONVERTER_WIDGET_H
#define BASE_CONVERTER_WIDGET_H

#include "BaseConverter.h"

#include <QWidget>

class QComboBox;
class QLabel;
class QPlainTextEdit;
class QPushButton;
class QString;

class BaseConverterWidget final : public QWidget {
public:
    explicit BaseConverterWidget(QWidget* parent = nullptr);

private:
    void createInterface();
    void clearResult();
    void convertValues();
    void swapBases();
    void updateRepresentationControls();
    void setStatus(const QString& message, const char* state) const;

    [[nodiscard]] static BaseConverter::Base selectedBase(const QComboBox* box);

    QComboBox* inputBase_{nullptr};
    QComboBox* outputBase_{nullptr};
    QComboBox* representation_{nullptr};
    QComboBox* bitWidth_{nullptr};
    QPlainTextEdit* input_{nullptr};
    QPlainTextEdit* output_{nullptr};
    QPushButton* convertButton_{nullptr};
    QLabel* statusLabel_{nullptr};
};

#endif
