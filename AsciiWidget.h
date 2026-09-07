#pragma once

#include <QWidget>

class QLabel;
class QPlainTextEdit;
class QPushButton;
class QString;

class AsciiWidget final : public QWidget {
public:
    explicit AsciiWidget(QWidget* parent = nullptr);

private:
    void createInterface();
    void clearResult() const;
    void convertText() const;
    void setStatus(const QString& message, const char* state) const;

    QPlainTextEdit* input_{nullptr};
    QPlainTextEdit* output_{nullptr};
    QPushButton* convertButton_{nullptr};
    QLabel* statusLabel_{nullptr};
};